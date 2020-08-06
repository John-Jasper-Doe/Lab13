/**
 * @file client.cpp
 * @brief Implimentation of the class "Client".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include <boost/bind.hpp>

#include "client.hpp"
#include "proto/parser.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the "Core" */
namespace core {

client::client(ba::io_service& context, common::taskpool_ptr_t pool,
               std::shared_ptr<db::dualstor> storage) noexcept
  : context_(context), socket_(context_), pool_(std::move(pool)), storage_(std::move(storage)) {}

void client::start() noexcept {
  stopped_ = false;
  do_read();
}

void client::stop() noexcept {
  if (!stopped_) {
    stopped_ = true;
    socket_.close();
  }
}

void client::write(const std::string& str) noexcept {
  do_write(str);
}

ba::ip::tcp::socket& client::socket() noexcept {
  return socket_;
}

std::shared_ptr<db::dualstor> client::get_stor() {
  return storage_;
}

void client::do_read() noexcept {
  if (!stopped_) {
    ba::async_read_until(socket_, buffer_, '\n',
                         boost::bind(&client::on_read, shared_from_this(), ba::placeholders::error,
                                     ba::placeholders::bytes_transferred));
  }
}

void client::on_read(const bs::error_code& ec, std::size_t sz) noexcept {
  if (ec == ba::error::eof || ec == ba::error::connection_reset)
    stop();
  else if (!stopped_) {
    if (sz) {
      std::istream out_strm(&buffer_);
      std::string cmd;
      getline(out_strm, cmd);

      std::shared_ptr<proto::parser> prs =
          std::make_shared<proto::parser>(std::move(cmd), shared_from_this());
      pool_->push([prs]() { prs->exec(); });
    }
  }
}

void client::do_write(const std::string& str) noexcept {
  if (stopped_)
    return;
  else {
    ba::async_write(socket_, ba::buffer(str),
                    boost::bind(&client::on_write, shared_from_this(), ba::placeholders::error,
                                ba::placeholders::bytes_transferred));
  }
}

void client::on_write(const bs::error_code& ec, std::size_t) noexcept {
  if (!stopped_) {
    if (ec == ba::error::eof || ec == ba::error::connection_reset)
      stop();
    else
      do_read();
  }
}

} /* core:: */
} /* join_server:: */
