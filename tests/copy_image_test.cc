#include "base/pixel.h"
#include "tkc/color.h"
#include "base/bitmap.h"
#include "blend/image_g2d.h"
#include "gtest/gtest.h"
#include "common.h"

static void test_copy_image(uint32_t stride, bitmap_format_t fmt) {
  uint32_t w = 8;
  uint32_t h = 8;
  rect_t r = rect_init(2, 2, 3, 3);
  uint32_t bpp = bitmap_get_bpp_of_format(fmt);
  bitmap_t* bg = bitmap_create_ex(w, h, bpp * (w + stride), fmt);
  bitmap_t* fg = bitmap_create_ex(w, h, bpp * (w + stride), fmt);

  color_t c = color_init(0x40, 0x60, 0x80, 0xff);

  ASSERT_EQ(image_fill(fg, &r, c), RET_OK);
  ASSERT_EQ(image_copy(bg, fg, &r, r.x, r.y), RET_OK);
  bitmap_dump(bg);
  bitmap_check(bg, &r, c.rgba);

  r = rect_init(0, 0, w, h);
  ASSERT_EQ(image_fill(fg, &r, c), RET_OK);
  ASSERT_EQ(image_copy(bg, fg, &r, r.x, r.y), RET_OK);
  bitmap_dump(bg);
  bitmap_check(bg, &r, c.rgba);

  bitmap_destroy(bg);
  bitmap_destroy(fg);
}

TEST(CopyImage, BITMAP_FMT_BGRA8888) {
  test_copy_image(0, BITMAP_FMT_BGRA8888);
}

TEST(CopyImage, BITMAP_FMT_BGR888) {
  test_copy_image(0, BITMAP_FMT_BGR888);
}

TEST(CopyImage, BITMAP_FMT_BGR565) {
  test_copy_image(0, BITMAP_FMT_BGR565);
}

TEST(CopyImage, BITMAP_FMT_BGRA8888_stride) {
  test_copy_image(1, BITMAP_FMT_BGRA8888);
  test_copy_image(2, BITMAP_FMT_BGRA8888);
  test_copy_image(3, BITMAP_FMT_BGRA8888);
  test_copy_image(30, BITMAP_FMT_BGRA8888);
}

TEST(CopyImage, BITMAP_FMT_BGR888_stride) {
  test_copy_image(1, BITMAP_FMT_BGR888);
  test_copy_image(2, BITMAP_FMT_BGR888);
  test_copy_image(3, BITMAP_FMT_BGR888);
  test_copy_image(40, BITMAP_FMT_BGR888);
}

TEST(CopyImage, BITMAP_FMT_BGR565_stride) {
  test_copy_image(1, BITMAP_FMT_BGR565);
  test_copy_image(2, BITMAP_FMT_BGR565);
  test_copy_image(3, BITMAP_FMT_BGR888);
  test_copy_image(50, BITMAP_FMT_BGR888);
}
