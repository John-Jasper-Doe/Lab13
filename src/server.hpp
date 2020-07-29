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
#include <libasync/async/async.hpp>
#include "client.hpp"

namespace bulk_server {
namespace ba = boost::asio;

class server : public std::enable_shared_from_this<server> {
  ba::io_service context_{};
  ba::ip::tcp::endpoint ep_;
  ba::ip::tcp::acceptor acceptor_;
  libasync::async::handle_t handle_;

public:
  server() = delete;
  explicit server(unsigned short port, unsigned int bulk_size) noexcept;

  ~server() noexcept;

  server(const server&) = delete;
  server(server&&) = delete;

  server& operator=(const server&) = delete;
  server& operator=(server&&) = delete;

  void start();

protected:
  void close() noexcept;
  void signal_handler(const boost::system::error_code&, int);
  void handle_accept(const std::shared_ptr<bulk_server::client>& client,
                     const boost::system::error_code& err);
};

} /* bulk_server */

#endif /* SERVER_HPP_ */
