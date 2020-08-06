#include <gtest/gtest.h>
#include "common/tsqueue.hpp"

TEST(test_tsqueue, test_pop_push1) {
  join_server::common::tsqueue<int> queue;

  ASSERT_EQ(queue.empty(), true);

  queue.push(1);
  queue.push(2);
  queue.push(3);
  queue.push(4);
  queue.push(5);

  ASSERT_EQ(queue.empty(), false);

  int test_val = 0;
  queue.try_pop(test_val);
  ASSERT_EQ(test_val, 1);

  queue.try_pop(test_val);
  ASSERT_EQ(test_val, 2);
}

TEST(test_tsqueue, test_pop_push2) {
  join_server::common::tsqueue<int> queue;

  ASSERT_EQ(queue.empty(), true);

  queue.push(1);
  queue.push(2);
  queue.push(3);
  queue.push(4);
  queue.push(5);

  ASSERT_EQ(queue.empty(), false);
  ASSERT_EQ(*queue.try_pop(), 1);
  ASSERT_EQ(*queue.try_pop(), 2);
}
