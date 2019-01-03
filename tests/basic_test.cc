#include "tkc/types_def.h"
#include "gtest/gtest.h"

TEST(Basic, round_to) {
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

TEST(Basic, round) {
  ASSERT_EQ(tk_roundi(1.1), 1);
  ASSERT_EQ(tk_roundi(1.5), 2);
  ASSERT_EQ(tk_roundi(1.9), 2);

  ASSERT_EQ(tk_roundi(-1.1), -1);
  ASSERT_EQ(tk_roundi(-1.5), -2);
  ASSERT_EQ(tk_roundi(-1.9), -2);
}

TEST(Basic, clamp) {
  ASSERT_EQ(tk_clampi(1, 5, 8), 5);
  ASSERT_EQ(tk_clampi(10, 5, 8), 8);
  ASSERT_EQ(tk_clampi(5, 5, 8), 5);
  ASSERT_EQ(tk_clampi(8, 5, 8), 8);
  ASSERT_EQ(tk_clampi(6, 5, 8), 6);
}
