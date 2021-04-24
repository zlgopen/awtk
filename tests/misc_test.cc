#include <string>
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"

using std::string;

TEST(Misc, round_to4) {
  ASSERT_EQ(TK_ROUND_TO4(1), 4);
  ASSERT_EQ(TK_ROUND_TO4(2), 4);
  ASSERT_EQ(TK_ROUND_TO4(3), 4);
  ASSERT_EQ(TK_ROUND_TO4(4), 4);
  ASSERT_EQ(TK_ROUND_TO4(5), 8);
  ASSERT_EQ(TK_ROUND_TO4(6), 8);
  ASSERT_EQ(TK_ROUND_TO4(7), 8);
  ASSERT_EQ(TK_ROUND_TO4(8), 8);
  ASSERT_EQ(TK_ROUND_TO4(9), 12);
  ASSERT_EQ(TK_ROUND_TO4(10), 12);
  ASSERT_EQ(TK_ROUND_TO4(13), 16);
  ASSERT_EQ(TK_ROUND_TO4(15), 16);
}

TEST(Misc, round_to8) {
  ASSERT_EQ(TK_ROUND_TO8(1), 8);
  ASSERT_EQ(TK_ROUND_TO8(2), 8);
  ASSERT_EQ(TK_ROUND_TO8(3), 8);
  ASSERT_EQ(TK_ROUND_TO8(4), 8);
  ASSERT_EQ(TK_ROUND_TO8(5), 8);
  ASSERT_EQ(TK_ROUND_TO8(6), 8);
  ASSERT_EQ(TK_ROUND_TO8(7), 8);
  ASSERT_EQ(TK_ROUND_TO8(8), 8);
  ASSERT_EQ(TK_ROUND_TO8(9), 16);
  ASSERT_EQ(TK_ROUND_TO8(10), 16);
  ASSERT_EQ(TK_ROUND_TO8(13), 16);
  ASSERT_EQ(TK_ROUND_TO8(15), 16);
  ASSERT_EQ(TK_ROUND_TO8(16), 16);
}

TEST(Misc, round_to) {
  ASSERT_EQ(TK_ROUND_TO(1, 4), 4);
  ASSERT_EQ(TK_ROUND_TO(1, 8), 8);
  ASSERT_EQ(TK_ROUND_TO(1, 16), 16);

  ASSERT_EQ(TK_ROUND_TO(4, 4), 4);
  ASSERT_EQ(TK_ROUND_TO(4, 8), 8);
  ASSERT_EQ(TK_ROUND_TO(4, 16), 16);

  ASSERT_EQ(TK_ROUND_TO(8, 4), 8);
  ASSERT_EQ(TK_ROUND_TO(8, 8), 8);
  ASSERT_EQ(TK_ROUND_TO(8, 16), 16);
}

TEST(Misc, round_to_mach) {
  uint32_t i = 0;
  for (i = 0; i < 1000; i++) {
    if (sizeof(void*) == 4) {
      ASSERT_EQ(TK_ROUND_TO_MACH(i), TK_ROUND_TO4(i));
    } else {
      ASSERT_EQ(TK_ROUND_TO_MACH(i), TK_ROUND_TO8(i));
    }
  }
}
