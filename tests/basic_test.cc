#include "base/types_def.h"
#include "gtest/gtest.h"

TEST(Basic, round) {
  uint32_t n = 4;
  uint32_t i = 0;
  for (i = 1; i < n; i++) {
    ASSERT_EQ(TK_ROUND_TO(i, n), n);
  }

  n = 8;
  for (i = 1; i < n; i++) {
    ASSERT_EQ(TK_ROUND_TO(i, n), n);
  }

  n = 16;
  for (i = 1; i < n; i++) {
    ASSERT_EQ(TK_ROUND_TO(i, n), n);
  }

  n = 32;
  for (i = 1; i < n; i++) {
    ASSERT_EQ(TK_ROUND_TO(i, n), n);
  }
}
