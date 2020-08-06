/**
 * @file server.hpp
 * @brief Definition of the class "Server".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio.hpp>

#include "client.hpp"
#include "common/taskpool.hpp"
#include "proto/parser.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the "Core" */
namespace core {

namespace ba = boost::asio;
namespace bs = boost::system;

/** @brief The server class */
class server final : public std::enable_shared_from_this<server> {
  ba::io_service context_{};
  ba::ip::tcp::endpoint ep_;
  ba::ip::tcp::acceptor acceptor_;
  common::taskpool_ptr_t pool_;
  std::shared_ptr<db::dualstor> storage_;

public:
  server() = delete;
  explicit server(unsigned short port, common::taskpool_ptr_t pool) noexcept;

  ~server() noexcept;

  server(const server&) = delete;
  server(server&&) = delete;

  server& operator=(const server&) = delete;
  server& operator=(server&&) = delete;

  void exec();

protected:
  void close() noexcept;
  void signal_handler(const bs::error_code&, int);
  void handle_accept(const client_ptr_t& client, const bs::error_code& err);
};

} /* core:: */
} /* join_server:: */

#endif /* SERVER_HPP_ */
