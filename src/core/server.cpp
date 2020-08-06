/**
 * @file server.cpp
 * @brief Implimentation of the class "Server".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include "server.hpp"
#include <boost/bind.hpp>

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the "Core" */
namespace core {

server::server(unsigned short port, common::taskpool_ptr_t pool) noexcept
  : ep_(ba::ip::tcp::v4(), port)
  , acceptor_(context_, ep_)
  , pool_(std::move(pool))
  , storage_{std::make_shared<db::dualstor>()} {}

server::~server() noexcept {
  close();
}

void server::exec() {
  pool_->start();

  ba::signal_set signal(context_, SIGINT, SIGABRT);
  signal.async_wait(boost::bind(&server::signal_handler, shared_from_this(),
                                ba::placeholders::error, ba::placeholders::signal_number()));

  auto client = std::make_shared<client_t>(context_, pool_, storage_);
  acceptor_.async_accept(
      client->socket(),
      boost::bind(&server::handle_accept, shared_from_this(), client, ba::placeholders::error));
  client = nullptr;
  context_.run();
}

void server::close() noexcept {
  pool_->stop();
}

void server::signal_handler(const bs::error_code&, int) {
  acceptor_.close();
  close();
}

void server::handle_accept(const client_ptr_t& client, const bs::error_code& err) {
  using namespace std::string_literals;

  if (err == boost::system::errc::operation_canceled)
    return;
  else if (err)
    throw std::runtime_error("Unknown  error: "s + err.message());

  client->start();

  auto waited_client = std::make_shared<client_t>(context_, pool_, storage_);
  acceptor_.async_accept(waited_client->socket(),
                         boost::bind(&server::handle_accept, shared_from_this(), waited_client,
                                     ba::placeholders::error));
}

} /* core:: */
} /* join_server:: */
