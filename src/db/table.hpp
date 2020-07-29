/**
 * @file table.hpp
 * @brief Definition of the class "Table".
 *
 * @details
 * Description of a class that implements a simple table view for storing test
 * task data.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <string>
#include "internals/type.hpp"

namespace join_server {
namespace db {

/** @brief The table class */
class table final {
  std::string name_{""};
  internals::storage_t storage_;

public:
  table() = delete;
  explicit table(std::string name) noexcept;

  ~table() = default;

  table(const table&) = delete;
  table(table&&) = delete;

  table& operator=(const table&) = delete;
  table& operator=(table&&) = delete;

  /**
   * @brief Insert value.
   * @details
   * Insert value only if key not exist.
   *
   * @param [in] key - primary key on table.
   * @param [in] value - added data.
   * @return "True" on success, "False" - otherwise
   */
  bool insert(int key, const row_t& value) noexcept;
};

} /* db:: */
} /* join_server:: */

#endif /* TABLE_HPP_ */
