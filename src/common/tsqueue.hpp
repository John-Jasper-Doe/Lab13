/**
 * @file tsqueue.hpp
 * @brief Implementation of the class "TSQueue".
 *
 * @details
 * Implementation of a thread-safe queue with fine granular locks. The
 * implementation was borrowed from the book:
 *   Anthony Williams, C++ Concurrency in Action, 2012, ISBN: 978-5-97060-194-5
 *   https://dmkpress.com/catalog/computer/programming/c/978-5-94074-448-1/
 *
 * The implementation is slightly modified to the requirements of the program.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef TSQUEUE_HPP_
#define TSQUEUE_HPP_

#include <condition_variable>
#include <mutex>

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the Common */
namespace common {

/** @brief The namespace "detail". */
namespace detail {

/**
 * @brief Representation of a queue node.
 * @tparam Data type.
 */
template <class T>
struct node {
  /** @brief Stored data. */
  std::shared_ptr<T> data;

  /** @brief Pointer to the next node. */
  std::unique_ptr<node> next;
};

} /* detail:: */

/**
 * @brief Thread-safe queue with blocking and waiting.
 * @tparam Data type.
 */
template <class T>
class tsqueue final {
  /** @brief Pointer to the head of the queue. */
  std::unique_ptr<detail::node<T>> head_;
  std::mutex mtx_head_;

  /** @brief Pointer to the tail of the queue. */
  detail::node<T>* tail_;
  std::mutex mtx_tail_;

  std::condition_variable cond_data_;

public:
  tsqueue() : head_(new detail::node<T>), tail_(head_.get()) {}

  tsqueue(const tsqueue& other) = delete;
  tsqueue& operator=(const tsqueue& other) = delete;

  /**
   * @brief Adding new values.
   * @param [in] new_value - new value.
   */
  void push(T&& new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<detail::node<T>> p(new detail::node<T>);

    {
      std::lock_guard<std::mutex> tail_lock(mtx_tail_);

      tail_->data = new_data;
      detail::node<T>* const new_tail = p.get();
      tail_->next = std::move(p);
      tail_ = new_tail;
    }

    cond_data_.notify_one();
  }

  /**
   * @brief Wait and pop from head.
   * @return Pointer of the data.
   */
  std::shared_ptr<T> wait_and_pop() {
    std::unique_ptr<detail::node<T>> const old_head = wait_pop_head();
    return old_head->data;
  }

  /**
   * @brief Wait and pop from head.
   * @param [out] value - data.
   */
  void wait_and_pop(T& value) {
    std::unique_ptr<detail::node<T>> const old_head = wait_pop_head(value);
  }

  /**
   * @brief Try pop of the data from head.
   * @return Pointer of the data.
   */
  std::shared_ptr<T> try_pop() {
    std::unique_ptr<detail::node<T>> old_head = try_pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  /**
   * @brief Try pop of the data from head.
   * @param [out] value - data.
   * @return "False" or "True"
   */
  bool try_pop(T& value) {
    std::unique_ptr<detail::node<T>> const old_head = try_pop_head(value);
    return (old_head != nullptr);
  }

  /**
   * @brief Test on empty.
   * @return "False" or "True"
   */
  bool empty() {
    std::lock_guard<std::mutex> head_lock(mtx_head_);
    return (head_.get() == tail());
  }

protected:
  detail::node<T>* tail() {
    std::lock_guard<std::mutex> tail_lock(mtx_tail_);
    return tail_;
  }

  std::unique_ptr<detail::node<T>> pop_head() {
    std::unique_ptr<detail::node<T>> old_head = std::move(head_);
    head_ = std::move(old_head->next);
    return std::move(old_head);
  }

  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock(mtx_head_);
    cond_data_.wait(head_lock, [&] { return head_.get() != tail(); });
    return head_lock;
  }

  std::unique_ptr<detail::node<T>> wait_pop_head() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  std::unique_ptr<detail::node<T>> wait_pop_head(T& value) {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    value = std::move(*head_->data);
    return pop_head();
  }

  std::unique_ptr<detail::node<T>> try_pop_head() {
    std::lock_guard<std::mutex> head_lock(mtx_head_);
    if (head_.get() == tail())
      return std::unique_ptr<detail::node<T>>();

    return pop_head();
  }

  std::unique_ptr<detail::node<T>> try_pop_head(T& value) {
    std::lock_guard<std::mutex> head_lock(mtx_head_);
    if (head_.get() == tail() || head_ == nullptr)
      return std::unique_ptr<detail::node<T>>();

    value = std::move(*head_->data);
    return pop_head();
  }
};

} /* common:: */
} /* join_server:: */

#endif /* TSQUEUE_HPP_ */
