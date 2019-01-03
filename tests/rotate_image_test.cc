#include "base/pixel.h"
#include "tkc/color.h"
#include "base/bitmap.h"
#include "blend/image_g2d.h"
#include "gtest/gtest.h"
#include "common.h"

static void test_rotate_image(uint32_t stride, bitmap_format_t fmt) {
  uint32_t w = 8;
  uint32_t h = 6;
  rect_t r = rect_init(2, 2, 4, 2);
  uint32_t bpp = bitmap_get_bpp_of_format(fmt);
  bitmap_t* b = bitmap_create_ex(h, w, bpp * (w + stride), fmt);
  bitmap_t* fg = bitmap_create_ex(w, h, bpp * (w + stride), fmt);

  color_t bg = color_init(0x40, 0x60, 0x80, 0xff);

  ASSERT_EQ(image_fill(fg, &r, bg), RET_OK);
  bitmap_dump(fg);

  ASSERT_EQ(image_rotate(b, fg, &r, LCD_ORIENTATION_90), RET_OK);
  bitmap_dump(b);

  r = rect_init(2, 2, 2, 4);
  bitmap_check(b, &r, bg.rgba);

  bitmap_destroy(b);
  bitmap_destroy(fg);
}

TEST(RotateImage, BITMAP_FMT_BGRA8888) {
  test_rotate_image(0, BITMAP_FMT_BGRA8888);
}

TEST(RotateImage, BITMAP_FMT_BGR888) {
  test_rotate_image(0, BITMAP_FMT_BGR888);
}

TEST(RotateImage, BITMAP_FMT_BGR565) {
  test_rotate_image(0, BITMAP_FMT_BGR565);
}

TEST(RotateImage, BITMAP_FMT_RGB565) {
  test_rotate_image(0, BITMAP_FMT_RGB565);
}

TEST(RotateImage, BITMAP_FMT_BGRA8888_stride) {
  test_rotate_image(1, BITMAP_FMT_BGRA8888);
}

TEST(RotateImage, BITMAP_FMT_BGR888_stride) {
  test_rotate_image(1, BITMAP_FMT_BGR888);
}

TEST(RotateImage, BITMAP_FMT_BGR565_stride) {
  test_rotate_image(1, BITMAP_FMT_BGR565);
}

TEST(RotateImage, BITMAP_FMT_RGB565_stride) {
  test_rotate_image(1, BITMAP_FMT_RGB565);
}
