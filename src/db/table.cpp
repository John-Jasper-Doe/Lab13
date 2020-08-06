/**
 * @file table.cpp
 * @brief Definition of the class "Table".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include "table.hpp"
#include <algorithm>

namespace join_server {
namespace db {

table::table(std::string name) noexcept : name_(std::move(name)) {}

bool table::insert(int key, const row_t& value) noexcept {
  auto it = storage_.find(key);
  if (it == storage_.end()) {
    storage_[key] = value;
    return true;
  }

  return false;
}

void table::update(int key, const row_t& value) noexcept {
  auto it = storage_.find(key);
  if (it != storage_.end())
    storage_[key] = value;
}

void table::truncate() noexcept {
  storage_.clear();
}

std::string table::name() const noexcept {
  return name_;
}

size_t table::size() const noexcept {
  return storage_.size();
}

bool table::empty() const noexcept {
  return storage_.empty();
}

storage_t::const_iterator table::cbegin() const noexcept {
  return storage_.cbegin();
}
storage_t::const_iterator table::cend() const noexcept {
  return storage_.cend();
}

std::string table::as_str() {
  std::string res{""};

  std::for_each(cbegin(), cend(), [&res](const std::pair<int, row_t>& p) {
    res.append(std::to_string(p.first));
    for (const std::string& s : p.second) {
      res.append("," + s);
    }
    res.append("\n");
  });

  return res;
}

} /* db:: */
} /* join_server:: */
