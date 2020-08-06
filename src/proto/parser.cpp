/**
 * @file parser.cpp
 * @brief Implementation of the "Parser" class.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include "parser.hpp"
#include "common/split.hpp"
#include "core/client.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the "Proto" */
namespace proto {

namespace {

static const char* MSG_OK = "OK\n";

static const char* CMD_INSERT = "INSERT";
static const char* CMD_TRUNCATE = "TRUNCATE";
static const char* CMD_INTERSECT = "INTERSECTION";
static const char* CMD_SYMDIFF = "SYMMETRIC_DIFFERENCE";

} /* :: */

parser::parser(std::string&& data_string, std::shared_ptr<core::client> client) noexcept
  : data_string_(std::move(data_string)), client_(std::move(client)) {}

parser::~parser() noexcept {}

void parser::exec() noexcept {
  tokens_t tokens = common::split(data_string_);

  if (!tokens.empty()) {
    if (tokens.at(0) == CMD_INSERT && tokens.size() == 4) {
      proc_insert(tokens);
    }
    else if (tokens.at(0) == CMD_TRUNCATE && tokens.size() == 2) {
      proc_truncate(tokens);
    }
    else if (tokens.at(0) == CMD_INTERSECT && tokens.size() == 1) {
      proc_intersect(tokens);
    }
    else if (tokens.at(0) == CMD_SYMDIFF && tokens.size() == 1) {
      proc_symdiff(tokens);
    }
    else {
      client_->write("Err: wrong command format\n");
    }
  }
}

void parser::proc_insert(parser::tokens_t& tokens) {
  /* get tabel name */
  db::tbl_name table{db::tbl_name::table_a};
  try {
    table = db::get_tabel_name(tokens[1]);
  }
  catch (std::runtime_error& e) {
    client_->write(e.what());
    return;
  }

  /* inserted into tabel */
  int id = std::stoi(tokens[2]);
  std::string& text = tokens[3];
  bool success = client_->get_stor()->insertion(table, id, std::move(text));
  if (success)
    client_->write(MSG_OK);
  else
    client_->write(std::string("Err: adding data into ") + tokens[1] + "\n");
}

void parser::proc_truncate(parser::tokens_t& tokens) {
  using namespace std::literals;

  /* get tabel name */
  db::tbl_name table{db::tbl_name::table_a};
  try {
    table = db::get_tabel_name(tokens[1]);
  }
  catch (std::runtime_error& e) {
    client_->write("Err: "s + e.what());
    return;
  }

  client_->get_stor()->truncate(table);
  client_->write(MSG_OK);
}

void parser::proc_intersect(parser::tokens_t&) {
  client_->write(client_->get_stor()->intersection().as_str() + MSG_OK);
}

void parser::proc_symdiff(parser::tokens_t&) {
  client_->write(client_->get_stor()->symmetric_difference().as_str() + MSG_OK);
}

} /* proto:: */
} /* join_server:: */
