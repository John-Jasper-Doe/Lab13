/**
 * @file table.cpp
 * @brief Definition of the class "Table".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include "table.hpp"

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

} /* db:: */
} /* join_server:: */
