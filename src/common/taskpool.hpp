/**
 * @file taskpool.hpp
 * @brief Task pool implementation.
 *
 * @details
 * The implementation was borrowed from the repository:
 *    Vitaliy Vitsentiy
 *    https://github.com/vit-vit/CTPL
 * The implementation is slightly modified to the requirements of the program.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef TASKPOOL_HPP_
#define TASKPOOL_HPP_

#include <array>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include "tsqueue.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the Common */
namespace common {

/**
 * @brief Class "Task Pool".
 * @tparam [in] THREADS - Pool size, number of threads processing tasks.
 */
template <std::size_t THREADS = 1>
class taskpool final {
  /** @brief Flag alias. */
  using flag_t = std::atomic<bool>;
  using flags_t = std::array<std::shared_ptr<flag_t>, THREADS>;

  /** @brief Thread poll alias. */
  using thread_ptr_t = std::unique_ptr<std::thread, std::function<void(std::thread*)>>;
  using threads_t = std::array<thread_ptr_t, THREADS>;

  /** @brief Pool task types alias */
  using task_t = std::function<void()>;
  using tasks_t = common::tsqueue<task_t*>;

  std::mutex mtx_;
  std::condition_variable cond_;

  flag_t is_done_{false};

  /** @brief Thread pool */
  threads_t threads_;

  /** @brief Threads statuses. */
  flags_t threads_stats_;

  /** @brief Task queue. */
  tasks_t tasks_;

public:
  /** @brief Constructor by default. */
  taskpool() = default;

  /**
   * @brief Distructor.
   * @details
   * The destructor stops the threads in case the user suddenly forgets to stop
   * them.
   */
  ~taskpool() {
    stop();
  }

  taskpool(const taskpool&) = delete;
  taskpool(taskpool&&) = delete;

  taskpool& operator=(const taskpool&) = delete;
  taskpool& operator=(taskpool&&) = delete;

  /** @brief Get pool size. */
  std::size_t size() const noexcept {
    return threads_.size();
  }

  /**
   * @brief start.
   * @details
   * The function initializes a given number of task processing threads. It
   * also sets the control flags of the threads.
   */
  void start() {
    if (!is_done_) {
      for (std::size_t i = 0; i < threads_.size(); ++i) {
        threads_stats_[i] = std::make_shared<flag_t>(false);
        init_thread(i);
      }
    }
  }

  /**
   * @brief stop.
   * @details
   * Function stop the work of threads. This is where the control flags are
   * set for threads to stop.
   */
  void stop() {
    if (is_done_)
      return;

    is_done_ = true;
    for (size_t i = 0, n = size(); i < n; ++i) {
      *threads_stats_[i] = true; /* command the threads - "stop!" */
    }

    {
      /* It is necessary to “kick” the disconnected streams to exit the blackout.
       * Blinding Area Limit */
      std::unique_lock<std::mutex> lock(mtx_);
      cond_.notify_all();
    }
  }

  /**
   * @brief Push
   * @tparam [in] T - The type of task to add.
   * @param [in] f - Pointer to a function of the form: libasync::common::counter()
   * @details
   * The function takes a pointer to a function of a given type. Wraps in
   * lambda and puts in line.
   */
  template <class T>
  void push(T&& f) {
    auto func = new task_t([f]() { f(); });

    tasks_.push(std::move(func));
    std::unique_lock<std::mutex> lock(mtx_);
    cond_.notify_one();
  }

protected:
  /**
   * @brief Initialization of threads.
   * @param [in] num - num thread.
   */
  void init_thread(std::size_t num) {
    std::shared_ptr<flag_t> threads_stat(threads_stats_[num]);

    auto worker = [this, threads_stat](std::size_t) {
      flag_t& thrd_stat = *threads_stat;
      task_t* task;
      bool is_pop = tasks_.try_pop(task);

      while (true) {
        while (is_pop) {
          (*task)();
          if (thrd_stat)
            return;
          else
            is_pop = tasks_.try_pop(task);
        }

        std::unique_lock<std::mutex> lock(mtx_);

        cond_.wait(lock, [this, &task, &is_pop, &thrd_stat]() {
          is_pop = tasks_.try_pop(task);
          return is_pop || this->is_done_ || thrd_stat;
        });

        if (!is_pop)
          return;
      }
    };
    threads_[num] = thread_ptr_t(new std::thread(worker, num), [](std::thread* t) { t->join(); });
  }
};

using taskpool_t = taskpool<2>;
using taskpool_ptr_t = std::shared_ptr<taskpool_t>;

} /* common:: */
} /* join_server:: */

#endif /* TASKPOOL_HPP_ */
