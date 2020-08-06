#include <gtest/gtest.h>
#include "common/split.hpp"

using namespace join_server::common;

TEST(test_split, test_split_six_elem) {
  std::vector<std::string> res = split("0 0 0 0 0 0 ");
  std::vector<std::string> expected{"0", "0", "0", "0", "0", "0"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}

TEST(test_split, test_split_one_elem) {
  std::vector<std::string> res = split("1 ");
  std::vector<std::string> expected{"1"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}

TEST(test_split, test_split_one_elem_without_seporator) {
  std::vector<std::string> res = split("1");
  std::vector<std::string> expected{"1"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}

TEST(test_split, test_split_sep_beg_end) {
  std::vector<std::string> res = split(" 2 3 4 5 6 { a ");
  std::vector<std::string> expected{"", "2", "3", "4", "5", "6", "{", "a"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}

TEST(test_split, test_split_sep_end) {
  std::vector<std::string> res = split("b c d } 89 ");
  std::vector<std::string> expected{"b", "c", "d", "}", "89"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}

TEST(test_split, test_split_sep_end_2) {
  std::vector<std::string> res = split("1 2 3 4 5 6 ");
  std::vector<std::string> expected{"1", "2", "3", "4", "5", "6"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}

TEST(test_split, test_split_many_sep) {
  std::vector<std::string> res = split(" 2    3 4    5 6 {         a ");
  std::vector<std::string> expected{"", "2", "3", "4", "5", "6", "{", "a"};

  ASSERT_TRUE(res.size() == expected.size());
  ASSERT_TRUE(std::equal(res.begin(), res.end(), expected.begin()));
}
