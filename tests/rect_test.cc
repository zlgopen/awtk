#include "base/rect.h"
#include "gtest/gtest.h"

TEST(Rect, merge) {
  rect_t dr;
  rect_t r;
  rect_init(dr, 50, 240, 80, 30);
  rect_init(r, 50, 200, 80, 30);
  ASSERT_EQ(rect_merge(&dr, &r), RET_OK);
  ASSERT_EQ(dr.x, 50);
  ASSERT_EQ(dr.y, 200);
  ASSERT_EQ(dr.h, 70);
  ASSERT_EQ(dr.w, 80);
}

TEST(Rect, contains) {
  rect_t r;
  rect_init(r, 50, 200, 80, 30);
  ASSERT_EQ(rect_contains(&r, 50, 200), TRUE);
  ASSERT_EQ(rect_contains(&r, 55, 200), TRUE);
  ASSERT_EQ(rect_contains(&r, 55, 240), TRUE);
  ASSERT_EQ(rect_contains(&r, 5, 24), FALSE);
  ASSERT_EQ(rect_contains(&r, 5, 240), FALSE);
}
