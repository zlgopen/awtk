#include "tkc/easing.h"
#include "gtest/gtest.h"

static float_t easing_linear(float_t k) {
  return k;
}


TEST(easing, easingreg) {
  ASSERT_EQ(easing_register("test1", easing_linear), EASING_FUNC_NR + 1);
  ASSERT_EQ(easing_register("test2", easing_linear), EASING_FUNC_NR + 2);
  ASSERT_EQ(easing_register("test3", easing_linear), EASING_FUNC_NR + 3);
  ASSERT_EQ(easing_register("test4", easing_linear), EASING_FUNC_NR + 4);
  ASSERT_EQ(easing_register("test5", easing_linear), EASING_FUNC_NR + 5);
}
