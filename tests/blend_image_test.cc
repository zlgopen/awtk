#include "base/pixel.h"
#include "tkc/color.h"
#include "base/bitmap.h"
#include "blend/image_g2d.h"
/* 该头文件由 gen.sh 生成，没有 BEGIN_C_DECLS，需要用 C 语言链接方式引用 */
extern "C" {
#include "blend/blend_image_bgra8888_bgra8888.h"
}
#include "gtest/gtest.h"
#include "common.h"

static void test_blend_image(uint32_t stride, bitmap_format_t bgfmt, bitmap_format_t fgfmt) {
  uint32_t w = 8;
  uint32_t h = 8;
  rect_t r = rect_init(2, 2, 3, 3);
  rect_t r_all = rect_init(0, 0, w, h);
  rectf_t rf = rect_to_rectf((const rect_t*)(&r));
  rectf_t rf_all = rect_to_rectf((const rect_t*)(&r_all));
  uint32_t bpp = bitmap_get_bpp_of_format(bgfmt);
  bitmap_t* bg = bitmap_create_ex(w, h, bpp * (w + stride), bgfmt);
  bitmap_t* fg = bitmap_create_ex(w, h, bpp * (w + stride), fgfmt);

  color_t c = color_init(0x40, 0x60, 0x80, 0xff);
  color_t c_clear = color_init(0x0, 0x0, 0x0, 0xff);

  ASSERT_EQ(image_clear(fg, &r, c), RET_OK);
  ASSERT_EQ(image_blend(bg, fg, &rf, &rf, 0xff), RET_OK);

  bitmap_dump(bg);
  bitmap_check(bg, &r, c.rgba);

  ASSERT_EQ(image_clear(bg, &r_all, c_clear), RET_OK);
  bitmap_dump(bg);

  ASSERT_EQ(image_blend(bg, fg, &rf_all, &rf, 0xff), RET_OK);
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

/* 直接按 BGRA8888 的存储顺序写像素，用于构造半透明图片 */
static void bitmap_fill_bgra8888(bitmap_t* b, uint8_t r, uint8_t g, uint8_t blue, uint8_t a) {
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t bpp = bitmap_get_bpp(b);
  uint32_t line_length = bitmap_get_physical_line_length(b);
  uint8_t* data = bitmap_lock_buffer_for_write(b);

  ASSERT_EQ(bpp, (uint32_t)pixel_bgra8888_BPP);

  for (y = 0; y < b->h; y++) {
    uint8_t* p = data + y * line_length;
    for (x = 0; x < b->w; x++, p += bpp) {
      p[0] = blue;
      p[1] = g;
      p[2] = r;
      p[3] = a;
    }
  }

  bitmap_unlock_buffer(b);
}

static void bitmap_check_bgra8888(bitmap_t* b, rgba_t e) {
  uint32_t x = 0;
  uint32_t y = 0;

  for (y = 0; y < b->h; y++) {
    for (x = 0; x < b->w; x++) {
      rgba_t rgba;
      ASSERT_EQ(bitmap_get_pixel(b, x, y, &rgba), RET_OK);
      ASSERT_EQ(rgba.r, e.r);
      ASSERT_EQ(rgba.g, e.g);
      ASSERT_EQ(rgba.b, e.b);
      ASSERT_EQ(rgba.a, e.a);
    }
  }
}

static bitmap_t* bitmap_create_test(uint32_t w, uint32_t h, bitmap_format_t fmt) {
  return bitmap_create_ex(w, h, bitmap_get_bpp_of_format(fmt) * w, fmt);
}

TEST(BlendImage, alpha_sum_saturate) {
  uint32_t w = 4;
  uint32_t h = 4;
  rect_t r_all = rect_init(0, 0, w, h);
  rectf_t rf_all = rect_to_rectf(&r_all);
  bitmap_t* dst = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  bitmap_t* src = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  /* src 的 alpha 加上 dst 的 alpha 为 0x100，out_a 必须饱和到 0xff(截断会变成 0) */
  rgba_t expected = {12, 22, 33, 0xff};

  bitmap_fill_bgra8888(dst, 100, 150, 200, 0xf4);
  bitmap_fill_bgra8888(src, 10, 20, 30, 0xf8);

  ASSERT_EQ(blend_image_bgra8888_bgra8888(dst, src, &rf_all, &rf_all, 0xff), RET_OK);
  bitmap_dump(dst);
  bitmap_check_bgra8888(dst, expected);

  bitmap_destroy(dst);
  bitmap_destroy(src);
}

TEST(BlendImage, premulti_alpha_sum_saturate) {
  uint32_t w = 4;
  uint32_t h = 4;
  rect_t r_all = rect_init(0, 0, w, h);
  rectf_t rf_all = rect_to_rectf(&r_all);
  bitmap_t* dst = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  bitmap_t* src = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  rgba_t expected = {2, 3, 4, 0xff};

  bitmap_fill_bgra8888(dst, 100, 150, 200, 0xf4);
  bitmap_fill_bgra8888(src, 10, 20, 30, 0xf8);
  src->flags |= BITMAP_FLAG_PREMULTI_ALPHA;

  ASSERT_EQ(blend_image_bgra8888_bgra8888(dst, src, &rf_all, &rf_all, 0xff), RET_OK);
  bitmap_dump(dst);
  bitmap_check_bgra8888(dst, expected);

  bitmap_destroy(dst);
  bitmap_destroy(src);
}

TEST(BlendImage, global_alpha_transparent) {
  uint32_t w = 4;
  uint32_t h = 4;
  rect_t r_all = rect_init(0, 0, w, h);
  rectf_t rf_all = rect_to_rectf(&r_all);
  bitmap_t* dst = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  bitmap_t* src = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  /* 全局 alpha 小于等于 PIXEL_ALPHA_TRANSPARENT 时不做任何处理 */
  rgba_t expected = {100, 150, 200, 0xf4};

  bitmap_fill_bgra8888(dst, 100, 150, 200, 0xf4);
  bitmap_fill_bgra8888(src, 10, 20, 30, 0x80);

  ASSERT_EQ(blend_image_bgra8888_bgra8888(dst, src, &rf_all, &rf_all, PIXEL_ALPHA_TRANSPARENT),
            RET_OK);
  bitmap_check_bgra8888(dst, expected);

  ASSERT_EQ(blend_image_bgra8888_bgra8888(dst, src, &rf_all, &rf_all, PIXEL_ALPHA_TRANSPARENT - 1),
            RET_OK);
  bitmap_check_bgra8888(dst, expected);

  bitmap_destroy(dst);
  bitmap_destroy(src);
}

static void test_blend_image_rotate(lcd_orientation_t o, ret_t expected_ret) {
  uint32_t w = 4;
  uint32_t h = 4;
  rect_t r_all = rect_init(0, 0, w, h);
  rectf_t rf_all = rect_to_rectf(&r_all);
  bitmap_t* dst = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  bitmap_t* src = bitmap_create_test(w, h, BITMAP_FMT_BGRA8888);
  /* src 半透明，dst 不透明：(0x40 * 0x80) >> 8 = 0x20 */
  rgba_t blended = {0x20, 0x40, 0x60, 0xff};
  /* 旋转失败时目标数据保持不变 */
  rgba_t unchanged = {0, 0, 0, 0xff};

  bitmap_fill_bgra8888(dst, 0, 0, 0, 0xff);
  bitmap_fill_bgra8888(src, 0x40, 0x80, 0xc0, 0x80);

  ASSERT_EQ(blend_image_rotate_bgra8888_bgra8888(dst, src, &rf_all, &rf_all, 0xff, o),
            expected_ret);
  bitmap_dump(dst);
  bitmap_check_bgra8888(dst, expected_ret == RET_OK ? blended : unchanged);

  bitmap_destroy(dst);
  bitmap_destroy(src);
}

TEST(BlendImage, rotate) {
  test_blend_image_rotate(LCD_ORIENTATION_0, RET_OK);
  test_blend_image_rotate(LCD_ORIENTATION_90, RET_OK);
  test_blend_image_rotate(LCD_ORIENTATION_180, RET_OK);
  test_blend_image_rotate(LCD_ORIENTATION_270, RET_OK);
  test_blend_image_rotate((lcd_orientation_t)45, RET_NOT_IMPL);
}
