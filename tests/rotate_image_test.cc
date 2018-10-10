#include "base/pixel.h"
#include "base/color.h"
#include "base/bitmap.h"
#include "blend/image_g2d.h"
#include "gtest/gtest.h"
#include "common.h"

static void test_rotate_image(bitmap_format_t fmt) {
  uint32_t w = 8;
  uint32_t h = 6;
  rect_t r = rect_init(2, 2, 4, 2);
  bitmap_t* b = bitmap_create_ex(h, w, fmt);
  bitmap_t* fg = bitmap_create_ex(w, h, fmt);

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
  test_rotate_image(BITMAP_FMT_BGRA8888);
}

TEST(RotateImage, BITMAP_FMT_BGR888) {
  test_rotate_image(BITMAP_FMT_BGR888);
}

TEST(RotateImage, BITMAP_FMT_BGR565) {
  test_rotate_image(BITMAP_FMT_BGR565);
}

TEST(RotateImage, BITMAP_FMT_RGB565) {
  test_rotate_image(BITMAP_FMT_RGB565);
}
