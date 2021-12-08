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
