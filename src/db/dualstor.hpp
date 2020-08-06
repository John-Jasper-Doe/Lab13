/**
 * @file dualstor.hpp
 * @brief Definition of the class "Dualstor".
 *
 * @details
 * Description of storage for working with two tables.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef DUALSTOR_HPP_
#define DUALSTOR_HPP_

#include "table.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the "DataBase" */
namespace db {

enum class tbl_name { table_a, table_b };
using table_name_t = db::tbl_name;

/** @brief The dualstor class */
class dualstor final {
  using table_t = db::table;

  table_t tbl_a_{"A"};
  table_t tbl_b_{"B"};

public:
  dualstor() = default;
  ~dualstor() = default;

  dualstor(const dualstor&) = delete;
  dualstor(dualstor&&) = delete;

  dualstor& operator=(const dualstor&) = delete;
  dualstor& operator=(dualstor&&) = delete;

  bool insertion(const table_name_t table_name, int key, std::string&& str);
  void truncate(const table_name_t table_name);

  table_t intersection();
  table_t symmetric_difference();
};

inline tbl_name get_tabel_name(const std::string& str) {
  if (str.compare("A") == 0)
    return tbl_name::table_a;
  else if (str.compare("B") == 0)
    return tbl_name::table_b;
  else
    throw "Error table name";
}

} /* db:: */
} /* join_server:: */

#endif /* DUALSTOR_HPP_ */
