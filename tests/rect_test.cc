#include "tkc/rect.h"
#include "gtest/gtest.h"

TEST(Rect, basic) {
  rect_t* r = rect_create(1, 2, 3, 4);
  rect_t r1 = rect_init(1, 2, 3, 4);

  ASSERT_EQ(r->x, r1.x);
  ASSERT_EQ(r->y, r1.y);
  ASSERT_EQ(r->w, r1.w);
  ASSERT_EQ(r->h, r1.h);

  rect_destroy(r);
}

TEST(Rect, merge) {
  rect_t dr;
  rect_t r;
  dr = rect_init(50, 240, 80, 30);
  r = rect_init(50, 200, 80, 30);
  ASSERT_EQ(rect_merge(&dr, &r), RET_OK);
  ASSERT_EQ(dr.x, 50);
  ASSERT_EQ(dr.y, 200);
  ASSERT_EQ(dr.h, 70);
  ASSERT_EQ(dr.w, 80);
}

TEST(Rect, contains) {
  rect_t r;
  r = rect_init(50, 200, 80, 30);
  ASSERT_EQ(rect_contains(&r, 50, 200), TRUE);
  ASSERT_EQ(rect_contains(&r, 55, 200), TRUE);
  ASSERT_EQ(rect_contains(&r, 55, 220), TRUE);
  ASSERT_EQ(rect_contains(&r, 5, 24), FALSE);
  ASSERT_EQ(rect_contains(&r, 5, 240), FALSE);
}

static bool test_rect_eq(rect_t& r, int32_t x, int32_t y, int32_t w, int32_t h) {
  return r.x == x && r.y == y && r.w == w && r.h == h;
}

TEST(Rect, diff) {
  rect_t r1, r2, out_r1, out_r2, out_r3, out_r4;

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(125, 125, 50, 50);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 25), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 125, 25, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 175, 125, 25, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 175, 100, 25), true);

  r1 = rect_init(125, 125, 50, 50);
  r2 = rect_init(100, 100, 100, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), FALSE);
  ASSERT_EQ(test_rect_eq(out_r1, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r3, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r4, 0, 0, 0, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(100, 100, 100, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), FALSE);
  ASSERT_EQ(test_rect_eq(out_r1, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r3, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r4, 0, 0, 0, 0), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 50, 50, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 100), true);
  ASSERT_EQ(test_rect_eq(out_r2, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r3, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r4, 0, 0, 0, 0), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 200, 50, 50);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 100), true);
  ASSERT_EQ(test_rect_eq(out_r2, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r3, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r4, 0, 0, 0, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(200, 50, 50, 50);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 100), true);
  ASSERT_EQ(test_rect_eq(out_r2, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r3, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r4, 0, 0, 0, 0), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 50, 100, 50);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 100), true);
  ASSERT_EQ(test_rect_eq(out_r2, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r3, 0, 0, 0, 0), true);
  ASSERT_EQ(test_rect_eq(out_r4, 0, 0, 0, 0), true);

  /* r2 叠在 r1 的左上角 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 50, 100, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 100, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 50, 100, 210);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 100), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 100, 50, 100), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 50, 210, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  /* r2 叠在 r1 的左下角 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 150, 100, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 150, 100, 210);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 150, 210, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  /* r2 叠在 r1 的左中区域 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 150, 100, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 150, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 175, 100, 25), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 150, 300, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 175, 100, 25), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 100, 100, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 100, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 125, 100, 75), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 100, 200, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 125, 100, 75), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 175, 100, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 75), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 175, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 150, 175, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(50, 175, 200, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 75), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 175, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 175, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  /* r2 叠在 r1 的右上角 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 50, 100, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 50, 100, 210);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 100), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 100), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 50, 210, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  /* r2 叠在 r1 的上中区域 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 50, 25, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 175, 100, 25, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 50, 25, 300);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 100), true);
  ASSERT_EQ(test_rect_eq(out_r3, 175, 100, 25, 100), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(100, 50, 25, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 125, 100, 75, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(100, 50, 25, 300);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 0, 100), true);
  ASSERT_EQ(test_rect_eq(out_r3, 125, 100, 75, 100), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);
  
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(175, 50, 25, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 75, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 150, 100, 50), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(175, 50, 25, 300);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 75, 100), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 100), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  /* r2 叠在 r1 的右下角 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 100, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 100, 210);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 210, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  /* r2 叠在 r1 的下中区域 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 25, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 175, 150, 25, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 25, 300);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 175, 150, 25, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(100, 150, 25, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 125, 150, 75, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(100, 150, 25, 300);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 125, 150, 75, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(175, 150, 25, 100);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 75, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(175, 150, 25, 300);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 75, 50), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 50), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 200, 100, 0), true);

  /* r2 叠在 r1 的右中区域 */
  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 100, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 175, 100, 25), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 150, 300, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 50), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 150, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 150, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 175, 100, 25), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 100, 100, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 125, 100, 75), true);

  r1 = rect_init(100, 100, 100, 100);
  r2 = rect_init(150, 100, 300, 25);

  ASSERT_EQ(rect_diff(&r1, &r2, &out_r1, &out_r2, &out_r3, &out_r4), TRUE);
  ASSERT_EQ(test_rect_eq(out_r1, 100, 100, 100, 0), true);
  ASSERT_EQ(test_rect_eq(out_r2, 100, 100, 50, 25), true);
  ASSERT_EQ(test_rect_eq(out_r3, 200, 100, 0, 25), true);
  ASSERT_EQ(test_rect_eq(out_r4, 100, 125, 100, 75), true);
}

TEST(Rect, intersect) {
  rect_t r;
  rect_t r1;
  rect_t r2;

  r1 = rect_init(0, 0, 10, 10);
  r2 = rect_init(10, 10, 10, 10);
  r = rect_intersect(&r1, &r2);
  ASSERT_EQ(r.x, 10);
  ASSERT_EQ(r.y, 10);
  ASSERT_EQ(r.w, 0);
  ASSERT_EQ(r.h, 0);

  r1 = rect_init(0, 0, 11, 11);
  r2 = rect_init(10, 10, 10, 10);
  r = rect_intersect(&r1, &r2);
  ASSERT_EQ(r.x, 10);
  ASSERT_EQ(r.y, 10);
  ASSERT_EQ(r.w, 1);
  ASSERT_EQ(r.h, 1);

  r1 = rect_init(0, 0, 14, 14);
  r2 = rect_init(10, 10, 10, 10);
  r = rect_intersect(&r1, &r2);
  ASSERT_EQ(r.x, 10);
  ASSERT_EQ(r.y, 10);
  ASSERT_EQ(r.w, 4);
  ASSERT_EQ(r.h, 4);
}

TEST(Point, init) {
  point_t p = point_init(10, 20);
  ASSERT_EQ(p.x, 10);
  ASSERT_EQ(p.y, 20);
}

TEST(PointF, init) {
  pointf_t p = pointf_init(10, 20);
  ASSERT_EQ(p.x, 10);
  ASSERT_EQ(p.y, 20);
}
