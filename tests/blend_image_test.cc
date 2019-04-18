#include "base/pixel.h"
#include "tkc/color.h"
#include "base/bitmap.h"
#include "blend/image_g2d.h"
#include "gtest/gtest.h"
#include "common.h"

static void test_blend_image(uint32_t stride, bitmap_format_t bgfmt, bitmap_format_t fgfmt) {
  uint32_t w = 8;
  uint32_t h = 8;
  rect_t r = rect_init(2, 2, 3, 3);
  rect_t r_all = rect_init(0, 0, w, h);
  uint32_t bpp = bitmap_get_bpp_of_format(bgfmt);
  bitmap_t* bg = bitmap_create_ex(w, h, bpp * (w + stride), bgfmt);
  bitmap_t* fg = bitmap_create_ex(w, h, bpp * (w + stride), fgfmt);

  color_t c = color_init(0x40, 0x60, 0x80, 0xff);
  color_t c_clear = color_init(0x0, 0x0, 0x0, 0xff);

  ASSERT_EQ(image_clear(fg, &r, c), RET_OK);
  ASSERT_EQ(image_blend(bg, fg, &r, &r, 0xff), RET_OK);

  bitmap_dump(bg);
  bitmap_check(bg, &r, c.rgba);

  ASSERT_EQ(image_clear(bg, &r_all, c_clear), RET_OK);
  bitmap_dump(bg);

  ASSERT_EQ(image_blend(bg, fg, &r_all, &r, 0xff), RET_OK);
  bitmap_dump(bg);
  bitmap_check(bg, &r_all, c.rgba);

  bitmap_destroy(bg);
  bitmap_destroy(fg);
}

TEST(BlendImage, BITMAP_FMT_BGRA8888) {
  test_blend_image(0, BITMAP_FMT_BGRA8888, BITMAP_FMT_BGRA8888);
  test_blend_image(0, BITMAP_FMT_BGRA8888, BITMAP_FMT_RGBA8888);
  test_blend_image(0, BITMAP_FMT_BGRA8888, BITMAP_FMT_BGR565);
}

TEST(BlendImage, BITMAP_FMT_BGR888) {
  test_blend_image(0, BITMAP_FMT_BGR888, BITMAP_FMT_BGRA8888);
  test_blend_image(0, BITMAP_FMT_BGR888, BITMAP_FMT_RGBA8888);
  test_blend_image(0, BITMAP_FMT_BGR888, BITMAP_FMT_BGR565);
}

TEST(BlendImage, BITMAP_FMT_BGR565) {
  test_blend_image(0, BITMAP_FMT_BGR565, BITMAP_FMT_BGRA8888);
  test_blend_image(0, BITMAP_FMT_BGR565, BITMAP_FMT_RGBA8888);
  test_blend_image(0, BITMAP_FMT_BGR888, BITMAP_FMT_BGR565);
}

TEST(BlendImage, BITMAP_FMT_BGRA8888_stride) {
  test_blend_image(1, BITMAP_FMT_BGRA8888, BITMAP_FMT_BGRA8888);
  test_blend_image(2, BITMAP_FMT_BGRA8888, BITMAP_FMT_RGBA8888);
  test_blend_image(3, BITMAP_FMT_BGRA8888, BITMAP_FMT_BGR565);
}

TEST(BlendImage, BITMAP_FMT_BGR888_stride) {
  test_blend_image(1, BITMAP_FMT_BGR888, BITMAP_FMT_BGRA8888);
  test_blend_image(2, BITMAP_FMT_BGR888, BITMAP_FMT_RGBA8888);
  test_blend_image(3, BITMAP_FMT_BGR888, BITMAP_FMT_BGR565);
}

TEST(BlendImage, BITMAP_FMT_BGR565_stride) {
  test_blend_image(1, BITMAP_FMT_BGR565, BITMAP_FMT_BGRA8888);
  test_blend_image(2, BITMAP_FMT_BGR565, BITMAP_FMT_RGBA8888);
  test_blend_image(3, BITMAP_FMT_BGR888, BITMAP_FMT_BGR565);
}
