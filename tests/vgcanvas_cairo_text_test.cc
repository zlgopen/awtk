#include <vector>

#include "base/dirty_rects.h"
#include "base/vgcanvas.h"
#include "gtest/gtest.h"

#if defined(WITH_VGCANVAS) && defined(WITH_VGCANVAS_CAIRO)

static bool_t has_non_zero_pixel(const std::vector<uint8_t>& buffer) {
  size_t i = 0;
  for (i = 0; i < buffer.size(); i++) {
    if (buffer[i] != 0) {
      return TRUE;
    }
  }

  return FALSE;
}

typedef struct _pixel_bbox_t {
  bool_t has_pixel;
  int32_t min_x;
  int32_t min_y;
  int32_t max_x;
  int32_t max_y;
} pixel_bbox_t;

static pixel_bbox_t get_non_zero_bbox(const std::vector<uint8_t>& buffer, int32_t w, int32_t h) {
  int32_t x = 0;
  int32_t y = 0;
  pixel_bbox_t bbox = {FALSE, w, h, -1, -1};

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      size_t i = ((size_t)y * (size_t)w + (size_t)x) * 4;
      if (buffer[i] != 0 || buffer[i + 1] != 0 || buffer[i + 2] != 0 || buffer[i + 3] != 0) {
        bbox.has_pixel = TRUE;
        if (x < bbox.min_x) {
          bbox.min_x = x;
        }
        if (y < bbox.min_y) {
          bbox.min_y = y;
        }
        if (x > bbox.max_x) {
          bbox.max_x = x;
        }
        if (y > bbox.max_y) {
          bbox.max_y = y;
        }
      }
    }
  }

  return bbox;
}

static pixel_bbox_t draw_text_and_get_bbox(const char* text_align, const char* text_baseline, int32_t x,
                                           int32_t y) {
  const int32_t w = 320;
  const int32_t h = 120;
  std::vector<uint8_t> buffer((size_t)w * (size_t)h * 4, 0);
  dirty_rects_t dirty_rects = {0};
  vgcanvas_t* vg = NULL;
  pixel_bbox_t bbox;

  dirty_rects.max = rect_init(0, 0, w, h);
  vg = vgcanvas_create(w, h, w * 4, BITMAP_FMT_BGRA8888, buffer.data());
  EXPECT_NE(vg, nullptr);
  if (vg == nullptr) {
    bbox.has_pixel = FALSE;
    bbox.min_x = w;
    bbox.min_y = h;
    bbox.max_x = -1;
    bbox.max_y = -1;
    return bbox;
  }

  EXPECT_EQ(vgcanvas_begin_frame(vg, &dirty_rects), RET_OK);
  EXPECT_EQ(vgcanvas_set_font_size(vg, 28), RET_OK);
  EXPECT_EQ(vgcanvas_set_font(vg, "sans"), RET_OK);
  EXPECT_EQ(vgcanvas_set_fill_color(vg, color_init(0xff, 0xff, 0xff, 0xff)), RET_OK);
  EXPECT_EQ(vgcanvas_set_text_align(vg, text_align), RET_OK);
  EXPECT_EQ(vgcanvas_set_text_baseline(vg, text_baseline), RET_OK);
  EXPECT_EQ(vgcanvas_fill_text(vg, "Hello Cairo", x, y, 200), RET_OK);
  EXPECT_EQ(vgcanvas_end_frame(vg), RET_OK);

  bbox = get_non_zero_bbox(buffer, w, h);
  vgcanvas_destroy(vg);

  return bbox;
}

static pixel_bbox_t draw_text_with_rotation_and_get_bbox(const char* text_align, const char* text_baseline,
                                                          int32_t anchor_x, int32_t anchor_y,
                                                          float_t angle_rad) {
  const int32_t w = 320;
  const int32_t h = 120;
  std::vector<uint8_t> buffer((size_t)w * (size_t)h * 4, 0);
  dirty_rects_t dirty_rects = {0};
  vgcanvas_t* vg = NULL;
  pixel_bbox_t bbox;

  dirty_rects.max = rect_init(0, 0, w, h);
  vg = vgcanvas_create(w, h, w * 4, BITMAP_FMT_BGRA8888, buffer.data());
  EXPECT_NE(vg, nullptr);
  if (vg == nullptr) {
    bbox.has_pixel = FALSE;
    bbox.min_x = w;
    bbox.min_y = h;
    bbox.max_x = -1;
    bbox.max_y = -1;
    return bbox;
  }

  EXPECT_EQ(vgcanvas_begin_frame(vg, &dirty_rects), RET_OK);
  EXPECT_EQ(vgcanvas_set_font_size(vg, 28), RET_OK);
  EXPECT_EQ(vgcanvas_set_font(vg, "sans"), RET_OK);
  EXPECT_EQ(vgcanvas_set_fill_color(vg, color_init(0xff, 0xff, 0xff, 0xff)), RET_OK);
  EXPECT_EQ(vgcanvas_set_text_align(vg, text_align), RET_OK);
  EXPECT_EQ(vgcanvas_set_text_baseline(vg, text_baseline), RET_OK);
  EXPECT_EQ(vgcanvas_save(vg), RET_OK);
  EXPECT_EQ(vgcanvas_translate(vg, (float_t)anchor_x, (float_t)anchor_y), RET_OK);
  EXPECT_EQ(vgcanvas_rotate(vg, angle_rad), RET_OK);
  EXPECT_EQ(vgcanvas_translate(vg, (float_t)-anchor_x, (float_t)-anchor_y), RET_OK);
  EXPECT_EQ(vgcanvas_fill_text(vg, "Hello Cairo", anchor_x, anchor_y, 200), RET_OK);
  EXPECT_EQ(vgcanvas_restore(vg), RET_OK);
  EXPECT_EQ(vgcanvas_end_frame(vg), RET_OK);

  bbox = get_non_zero_bbox(buffer, w, h);
  vgcanvas_destroy(vg);

  return bbox;
}

TEST(VgcanvasCairoText, measure_and_fill_text) {
  std::vector<uint8_t> buffer(320 * 120 * 4, 0);
  dirty_rects_t dirty_rects = {0};
  float_t ascent = 0;
  float_t descent = 0;
  float_t line_hight = 0;
  dirty_rects.max = rect_init(0, 0, 320, 120);
  vgcanvas_t* vg = vgcanvas_create(320, 120, 320 * 4, BITMAP_FMT_BGRA8888, buffer.data());
  ASSERT_NE(vg, nullptr);

  ASSERT_EQ(vgcanvas_begin_frame(vg, &dirty_rects), RET_OK);
  ASSERT_EQ(vgcanvas_set_font_size(vg, 28), RET_OK);
  ASSERT_EQ(vgcanvas_set_font(vg, "sans"), RET_OK);
  ASSERT_EQ(vgcanvas_set_fill_color(vg, color_init(0xff, 0x20, 0x20, 0xff)), RET_OK);
  ASSERT_EQ(vgcanvas_set_text_align(vg, "left"), RET_OK);
  ASSERT_EQ(vgcanvas_set_text_baseline(vg, "top"), RET_OK);

  ASSERT_GT(vgcanvas_measure_text(vg, "Hello Cairo"), 0);
  ASSERT_EQ(vgcanvas_get_text_metrics(vg, &ascent, &descent, &line_hight), RET_OK);
  ASSERT_GT(ascent, 0);
  ASSERT_GT(line_hight, 0);
  ASSERT_GT(ascent - descent, 0);
  ASSERT_EQ(vgcanvas_fill_text(vg, "Hello Cairo", 20, 20, 200), RET_OK);
  ASSERT_EQ(vgcanvas_end_frame(vg), RET_OK);

  ASSERT_TRUE(has_non_zero_pixel(buffer));

  vgcanvas_destroy(vg);
}

TEST(VgcanvasCairoText, text_align_and_baseline_positioning) {
  const int32_t anchor_x = 160;
  const int32_t anchor_y = 60;
  const int32_t tol = 2;
  pixel_bbox_t left_top = draw_text_and_get_bbox("left", "top", anchor_x, anchor_y);
  pixel_bbox_t center_middle = draw_text_and_get_bbox("center", "middle", anchor_x, anchor_y);
  pixel_bbox_t right_bottom = draw_text_and_get_bbox("right", "bottom", anchor_x, anchor_y);

  ASSERT_TRUE(left_top.has_pixel);
  ASSERT_TRUE(center_middle.has_pixel);
  ASSERT_TRUE(right_bottom.has_pixel);

  ASSERT_GE(left_top.min_x, anchor_x - tol);
  ASSERT_GE(left_top.min_y, anchor_y - tol);

  ASSERT_LE(right_bottom.max_x, anchor_x + tol);
  ASSERT_LE(right_bottom.max_y, anchor_y + tol);

  ASSERT_LT(center_middle.min_x, anchor_x);
  ASSERT_GT(center_middle.max_x, anchor_x);
  ASSERT_LT(center_middle.min_y, anchor_y);
  ASSERT_GT(center_middle.max_y, anchor_y);
}

TEST(VgcanvasCairoText, center_middle_anchor_with_rotation) {
  const int32_t anchor_x = 160;
  const int32_t anchor_y = 60;
  pixel_bbox_t no_rotation = draw_text_and_get_bbox("center", "middle", anchor_x, anchor_y);
  pixel_bbox_t rotated = draw_text_with_rotation_and_get_bbox("center", "middle", anchor_x, anchor_y,
                                                               0.5235987756f /* 30deg */);

  ASSERT_TRUE(no_rotation.has_pixel);
  ASSERT_TRUE(rotated.has_pixel);

  ASSERT_LT(no_rotation.min_x, anchor_x);
  ASSERT_GT(no_rotation.max_x, anchor_x);
  ASSERT_LT(no_rotation.min_y, anchor_y);
  ASSERT_GT(no_rotation.max_y, anchor_y);

  ASSERT_LT(rotated.min_x, anchor_x);
  ASSERT_GT(rotated.max_x, anchor_x);
  ASSERT_LT(rotated.min_y, anchor_y);
  ASSERT_GT(rotated.max_y, anchor_y);

  ASSERT_TRUE(rotated.min_x != no_rotation.min_x || rotated.max_x != no_rotation.max_x ||
              rotated.min_y != no_rotation.min_y || rotated.max_y != no_rotation.max_y);
}

#endif /*defined(WITH_VGCANVAS) && defined(WITH_VGCANVAS_CAIRO)*/
