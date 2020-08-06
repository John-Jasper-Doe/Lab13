/**
 * @file dualstor.cpp
 * @brief Implementation of the class "Dualstor".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#include "dualstor.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the DataBase */
namespace db {

bool dualstor::insertion(const table_name_t table_name, int key, std::string&& str) {
  switch (table_name) {
  case db::tbl_name::table_a:
    return tbl_a_.insert(key, row_t{std::move(str)});

  case db::tbl_name::table_b:
    return tbl_b_.insert(key, row_t{std::move(str)});
  }

  return false;
}

void dualstor::truncate(const table_name_t table_name) {
  switch (table_name) {
  case db::tbl_name::table_a:
    return tbl_a_.truncate();

  case db::tbl_name::table_b:
    return tbl_b_.truncate();
  }
}

db::table dualstor::intersection() {
  db::table res("intersection");
  auto tbl_a_it = tbl_a_.cbegin();
  auto tbl_b_it = tbl_b_.cbegin();

  while (tbl_a_it != tbl_a_.cend() && tbl_b_it != tbl_b_.cend()) {
    if (tbl_a_it->first < tbl_b_it->first)
      ++tbl_a_it;
    else if (tbl_a_it->first > tbl_b_it->first)
      ++tbl_b_it;
    else {
      row_t new_row = tbl_a_it->second;
      std::copy(tbl_b_it->second.begin(), tbl_b_it->second.end(), std::back_inserter(new_row));
      res.insert(tbl_a_it->first, new_row);

      ++tbl_a_it;
      ++tbl_b_it;
    }
  }
  return res;
}

db::table dualstor::symmetric_difference() {
  db::table res("symmetric_difference");

  auto tbl_a_it = tbl_a_.cbegin();
  auto tbl_b_it = tbl_b_.cbegin();

  while (tbl_a_it != tbl_a_.cend() && tbl_b_it != tbl_b_.cend()) {
    if (tbl_a_it->first < tbl_b_it->first) {
      row_t new_row = tbl_a_it->second;
      new_row.emplace_back("");
      res.insert(tbl_a_it->first, new_row);

      ++tbl_a_it;
    }
    else if (tbl_a_it->first > tbl_b_it->first) {
      row_t new_row = tbl_b_it->second;
      new_row.emplace(new_row.begin(), "");
      res.insert(tbl_b_it->first, new_row);

      ++tbl_b_it;
    }
    else {
      ++tbl_a_it;
      ++tbl_b_it;
    }
  }

  while (tbl_a_it != tbl_a_.cend()) {
    row_t new_row = tbl_a_it->second;
    new_row.emplace_back("");
    res.insert(tbl_a_it->first, new_row);
    ++tbl_a_it;
  }
  while (tbl_b_it != tbl_b_.cend()) {
    row_t new_row = tbl_b_it->second;
    new_row.emplace(new_row.begin(), "");
    res.insert(tbl_b_it->first, new_row);

    ++tbl_b_it;
  }

  return res;
}

} /* db:: */
} /* join_server:: */
