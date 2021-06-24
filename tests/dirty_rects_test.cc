#include "gtest/gtest.h"

#define TK_MAX_DIRTY_RECT_NR 3
#include "base/dirty_rects.h"

TEST(DirtyRects, basic) {
  rect_t r;
  dirty_rects_t dr;
  dirty_rects_init(&dr);

  r = rect_init(0, 0, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 1);

  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 1);

  r = rect_init(3, 3, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 1);

  r = rect_init(0, 0, 8, 8);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);

  r = rect_init(10, 10, 10, 10);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 2);
  ASSERT_EQ(memcmp(&r, dr.rects + 1, sizeof(rect_t)), 0);

  r = rect_init(10, 10, 10, 10);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 2);
  ASSERT_EQ(memcmp(&r, dr.rects + 1, sizeof(rect_t)), 0);

  r = rect_init(20, 20, 10, 10);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 3);
  ASSERT_EQ(memcmp(&r, dr.rects + 2, sizeof(rect_t)), 0);

  r = rect_init(100, 20, 10, 10);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);

  ASSERT_EQ(dr.nr, 2);
  ASSERT_EQ(memcmp(&r, dr.rects + 1, sizeof(rect_t)), 0);

  dirty_rects_reset(&dr);
  ASSERT_EQ(dr.nr, 0);

  dirty_rects_deinit(&dr);
}

TEST(DirtyRects, add_remove) {
  rect_t r;
  dirty_rects_t dr;
  dirty_rects_init(&dr);

  r = rect_init(0, 0, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 1);

  ASSERT_EQ(dirty_rects_remove(&dr, 0), RET_OK);
  ASSERT_EQ(dr.nr, 0);

  r = rect_init(0, 0, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 1);

  r = rect_init(10, 0, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 2);

  r = rect_init(10, 10, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 3);

  ASSERT_EQ(dirty_rects_remove(&dr, 1), RET_OK);
  ASSERT_EQ(dr.nr, 2);

  ASSERT_EQ(dirty_rects_remove(&dr, 1), RET_OK);
  ASSERT_EQ(dr.nr, 1);

  ASSERT_EQ(dirty_rects_remove(&dr, 1), RET_BAD_PARAMS);
  ASSERT_EQ(dr.nr, 1);

  ASSERT_EQ(dirty_rects_remove(&dr, 0), RET_OK);
  ASSERT_EQ(dr.nr, 0);

  dirty_rects_deinit(&dr);
}

TEST(DirtyRects, overlap1) {
  rect_t r;
  dirty_rects_t dr;
  dirty_rects_init(&dr);

  r = rect_init(0, 0, 20, 20);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 1);

  r = rect_init(25, 0, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects + 1, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 2);

  r = rect_init(15, 10, 20, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 1);

  r = rect_init(0, 0, 35, 20);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
  ASSERT_EQ(memcmp(&r, &(dr.max), sizeof(rect_t)), 0);
}

TEST(DirtyRects, overlap2) {
  rect_t r;
  dirty_rects_t dr;
  dirty_rects_init(&dr);

  r = rect_init(0, 0, 20, 20);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 1);

  r = rect_init(50, 0, 5, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, dr.rects + 1, sizeof(rect_t)), 0);
  ASSERT_EQ(dr.nr, 2);

  r = rect_init(15, 10, 20, 5);
  ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
  ASSERT_EQ(dr.nr, 2);

  r = rect_init(0, 0, 35, 20);
  ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);

  r = rect_init(0, 0, 55, 20);
  ASSERT_EQ(memcmp(&r, &(dr.max), sizeof(rect_t)), 0);

  dirty_rects_deinit(&dr);
}

TEST(DirtyRects, overlap3) {
  rect_t r;
  uint32_t i = 0;
  dirty_rects_t dr;
  dirty_rects_init(&dr);

  for (i = 1; i < 100; i++) {
    r = rect_init(0, 0, i, i);
    ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);
    ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
    ASSERT_EQ(dr.nr, 1);
  }
  dirty_rects_deinit(&dr);
}

TEST(DirtyRects, overlap4) {
  rect_t r;
  int32_t i = 0;
  dirty_rects_t dr;
  dirty_rects_init(&dr);

  for (i = 1000; i > 0; i--) {
    r = rect_init(0, 0, i, i);
    ASSERT_EQ(dirty_rects_add(&dr, &r), RET_OK);

    r = rect_init(0, 0, 1000, 1000);
    ASSERT_EQ(memcmp(&r, dr.rects, sizeof(rect_t)), 0);
    ASSERT_EQ(dr.nr, 1);
  }
  dirty_rects_deinit(&dr);
}
