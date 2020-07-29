/**
 * @file server.cpp
 * @brief Implimentation of the class "Server".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include "server.hpp"
#include <boost/bind.hpp>

namespace bulk_server {

server::server(unsigned short port, unsigned int bulk_size) noexcept
  : ep_(ba::ip::tcp::v4(), port)
  , acceptor_(context_, ep_)
  , handle_(libasync::async::connect(bulk_size)) {}

server::~server() noexcept {
  close();
}

void server::start() {
  ba::signal_set signal(context_, SIGINT, SIGABRT);
  signal.async_wait(boost::bind(&server::signal_handler, shared_from_this(),
                                ba::placeholders::error, ba::placeholders::signal_number()));

  auto client = std::make_shared<bulk_server::client>(context_, handle_);
  acceptor_.async_accept(
      client->socket(),
      boost::bind(&server::handle_accept, shared_from_this(), client, ba::placeholders::error));
  client = nullptr;
  context_.run();
}

void server::close() noexcept {
  if (handle_) {
    libasync::async::disconnect(handle_);
    handle_ = nullptr;
  }
}

void server::signal_handler(const boost::system::error_code&, int) {
  acceptor_.close();
  close();
}

void server::handle_accept(const std::shared_ptr<bulk_server::client>& client,
                           const boost::system::error_code& err) {
  using namespace std::string_literals;

  if (err == boost::system::errc::operation_canceled)
    return;
  else if (err)
    throw std::runtime_error("Unknown  error: "s + err.message());

  client->start();

  auto waited_client = std::make_shared<bulk_server::client>(context_, handle_);
  acceptor_.async_accept(waited_client->socket(),
                         boost::bind(&server::handle_accept, shared_from_this(), waited_client,
                                     ba::placeholders::error));
}

} /* bulk_server */
