/**
 * @file client.hpp
 * @brief Definition of the class "Client".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <boost/asio.hpp>
#include <memory>

#include "common/taskpool.hpp"
#include "db/dualstor.hpp"
#include "proto/parser.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the Core */
namespace core {

namespace ba = boost::asio;
namespace bs = boost::system;

class client final : public std::enable_shared_from_this<client> {
  ba::io_service& context_;
  ba::ip::tcp::socket socket_;
  ba::streambuf buffer_;
  std::atomic<bool> stopped_{true};

  common::taskpool_ptr_t pool_;
  std::shared_ptr<db::dualstor> storage_;

public:
  client() = delete;
  explicit client(ba::io_service& context, common::taskpool_ptr_t pool,
                  std::shared_ptr<db::dualstor> storage) noexcept;

  ~client() = default;

  client(const client&) = delete;
  client(client&&) = delete;

  client& operator=(const client&) = delete;
  client& operator=(client&&) = delete;

  void start() noexcept;
  void stop() noexcept;
  void write(const std::string& str) noexcept;

  ba::ip::tcp::socket& socket() noexcept;

  std::shared_ptr<db::dualstor> get_stor();

protected:
  void do_read() noexcept;
  void on_read(const bs::error_code& ec, std::size_t sz) noexcept;

  void do_write(const std::string&) noexcept;
  void on_write(const bs::error_code& ec, std::size_t) noexcept;
};

using client_t = join_server::core::client;
using client_ptr_t = std::shared_ptr<client_t>;

} /* core:: */
} /* join_server:: */

#endif /* CLIENT_HPP_ */
