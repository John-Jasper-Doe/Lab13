#include <gtest/gtest.h>
#include "db/table.hpp"

TEST(test_table, test_table_name) {
  join_server::db::table tbl("table_name");
  std::string expected{"table_name"};

  ASSERT_EQ(tbl.name(), expected);
}

TEST(test_table, test_table_size) {
  join_server::db::table tbl("table_name");

  tbl.insert(1, "str1");
  tbl.insert(2, "str2");
  tbl.insert(3, "str3");
  tbl.insert(4, "str4");
  tbl.insert(5, "str5");
  ASSERT_EQ(tbl.size(), 5);
}

TEST(test_table, test_table_clear) {
  join_server::db::table tbl("table_name");

  tbl.insert(1, "str1");
  tbl.insert(2, "str2");
  tbl.insert(3, "str3");
  tbl.insert(4, "str4");
  tbl.insert(5, "str5");

  tbl.clear();
  ASSERT_EQ(tbl.size(), 0);
}

TEST(test_table, test_table_empty) {
  join_server::db::table tbl("table_name");

  ASSERT_TRUE(tbl.empty());

  tbl.insert(1, "str1");
  tbl.insert(2, "str2");
  ASSERT_FALSE(tbl.empty());
}

TEST(test_table, test_table_insert) {
  join_server::db::table tbl("table_name");

  ASSERT_TRUE(tbl.insert(1, "str1"));
  ASSERT_FALSE(tbl.empty());
  ASSERT_EQ((*tbl.cbegin()).second, "str1");
}

TEST(test_table, test_table_update) {
  join_server::db::table tbl("table_name");

  ASSERT_TRUE(tbl.insert(1, "str1"));
  ASSERT_FALSE(tbl.empty());
  ASSERT_EQ((*tbl.cbegin()).second, "str1");

  tbl.update(1, "str3");
  ASSERT_FALSE(tbl.empty());
  ASSERT_EQ((*tbl.cbegin()).second, "str3");
}
