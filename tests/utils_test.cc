#include "base/utils.h"
#include "gtest/gtest.h"

TEST(Utils, basic) {
  char str[32];

  ASSERT_EQ(ftk_atoi("100"), 100);
  ASSERT_EQ(strcmp(ftk_itoa(str, sizeof(str), ftk_atoi("100")), "100"), 0);
}
