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

#include <libasync/async/async.hpp>

namespace bulk_server {
namespace ba = boost::asio;

class client : public std::enable_shared_from_this<client> {
  ba::io_service& context_;
  ba::ip::tcp::socket socket_;
  libasync::async::handle_t handle_;
  ba::streambuf buffer_;
  std::atomic<bool> stopped_{true};

public:
  client() = delete;
  explicit client(ba::io_service& context, libasync::async::handle_t handle) noexcept;

  ~client() = default;

  client(const client&) = delete;
  client(client&&) = delete;

  client& operator=(const client&) = delete;
  client& operator=(client&&) = delete;

  void start() noexcept;
  void stop() noexcept;

  ba::ip::tcp::socket& socket() noexcept;

protected:
  void do_read() noexcept;
  void read(const boost::system::error_code& ec, std::size_t sz) noexcept;
};

} /* bulk_server */

#endif /* CLIENT_HPP_ */
