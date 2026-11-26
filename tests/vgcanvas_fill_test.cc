#ifdef WITH_GPU

#include "awtk.h"
#include "native_window/native_window_sdl.h"
#include "gtest/gtest.h"

/* 验证 GPU 后端 ALL_FILL 模式对子路径和绕向的处理。 */

static bool_t vgcanvas_fill_has_pixel(bitmap_t* img, framebuffer_object_t* fbo, float_t x,
                                      float_t y) {
  // 把逻辑坐标换算到 FBO 的物理像素，再检查 alpha 通道，避免受 ratio 影响。
  uint32_t px = tk_roundi(x * fbo->ratio);
  uint32_t py = tk_roundi(y * fbo->ratio);
  uint32_t line_length = bitmap_get_physical_line_length(img);
  const uint8_t* data = bitmap_lock_buffer_for_read(img);
  bool_t has_pixel = FALSE;

  if (data != NULL && px < bitmap_get_physical_width(img) &&
      py < bitmap_get_physical_height(img)) {
    has_pixel = data[py * line_length + px * 4 + 3] > 0x80;
  }

  if (data != NULL) {
    bitmap_unlock_buffer(img);
  }

  return has_pixel;
}

static bitmap_t* vgcanvas_fill_read_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo) {
  // 把 FBO 内容读回位图，方便用像素结果验证 fill 规则。
  bitmap_t* img = bitmap_create_ex(fbo->w * fbo->ratio, fbo->h * fbo->ratio, 0,
                                   BITMAP_FMT_RGBA8888);
  if (img != NULL) {
    vgcanvas_fbo_to_bitmap(vg, fbo, img, NULL);
  }
  return img;
}

TEST(VgcanvasGpuFillTest, AllFillDrawsEverySubpath) {
  bitmap_t* img = NULL;
  framebuffer_object_t fbo;

  ASSERT_EQ(native_window_sdl_init(TRUE, 320, 240), RET_OK);
  widget_t* win = window_create(NULL, 0, 0, 320, 240);
  native_window_t* native_win = native_window_create(win);
  canvas_t* c = native_window_get_canvas(native_win);
  ASSERT_NE(c, nullptr);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  ASSERT_NE(vg, nullptr);

  memset(&fbo, 0, sizeof(fbo));
  ASSERT_EQ(vgcanvas_begin_frame(vg, NULL), RET_OK);
  ASSERT_EQ(vgcanvas_create_fbo_ex(vg, 100, 50, FALSE, TRUE, &fbo), RET_OK);
  ASSERT_EQ(vgcanvas_bind_fbo(vg, &fbo), RET_OK);
  ASSERT_EQ(vgcanvas_set_fill_mode(vg, VGCANVAS_FILL_MODE_ALL_FILL), RET_OK);
  ASSERT_EQ(vgcanvas_set_fill_color(vg, color_init(0xFF, 0, 0, 0xFF)), RET_OK);
  ASSERT_EQ(vgcanvas_begin_path(vg), RET_OK);

  // ALL_FILL 需要把同一个 path 里的每个子路径都当成独立填充区域。
  // 这里用三个不重叠的三角形验证它们不会因为共享 path 而丢失。
  vgcanvas_move_to(vg, 10, 10);
  vgcanvas_line_to(vg, 30, 10);
  vgcanvas_line_to(vg, 20, 30);
  vgcanvas_close_path(vg);

  vgcanvas_move_to(vg, 40, 10);
  vgcanvas_line_to(vg, 60, 10);
  vgcanvas_line_to(vg, 50, 30);
  vgcanvas_close_path(vg);

  vgcanvas_move_to(vg, 70, 10);
  vgcanvas_line_to(vg, 90, 10);
  vgcanvas_line_to(vg, 80, 30);
  vgcanvas_close_path(vg);

  ASSERT_EQ(vgcanvas_fill(vg), RET_OK);
  ASSERT_EQ(vgcanvas_end_frame(vg), RET_OK);

  img = vgcanvas_fill_read_fbo(vg, &fbo);
  ASSERT_NE(img, nullptr);
  // 三个采样点分别落在三个三角形内部。
  ASSERT_TRUE(vgcanvas_fill_has_pixel(img, &fbo, 20, 18));
  ASSERT_TRUE(vgcanvas_fill_has_pixel(img, &fbo, 50, 18));
  ASSERT_TRUE(vgcanvas_fill_has_pixel(img, &fbo, 80, 18));
  // 这个点在所有三角形之外，用来确认没有越界填充。
  ASSERT_FALSE(vgcanvas_fill_has_pixel(img, &fbo, 5, 40));

  bitmap_destroy(img);
  fbo.custom_draw_model = TRUE;
  ASSERT_EQ(vgcanvas_unbind_fbo(vg, &fbo), RET_OK);
  vgcanvas_destroy_fbo(vg, &fbo);
  widget_destroy(win);
  native_window_sdl_deinit();
}

TEST(VgcanvasGpuFillTest, AllFillKeepsReverseWindingSubpath) {
  bitmap_t* img = NULL;
  framebuffer_object_t fbo;

  ASSERT_EQ(native_window_sdl_init(TRUE, 320, 240), RET_OK);
  widget_t* win = window_create(NULL, 0, 0, 320, 240);
  native_window_t* native_win = native_window_create(win);
  canvas_t* c = native_window_get_canvas(native_win);
  ASSERT_NE(c, nullptr);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  ASSERT_NE(vg, nullptr);

  memset(&fbo, 0, sizeof(fbo));
  ASSERT_EQ(vgcanvas_begin_frame(vg, NULL), RET_OK);
  ASSERT_EQ(vgcanvas_create_fbo_ex(vg, 100, 80, FALSE, TRUE, &fbo), RET_OK);
  ASSERT_EQ(vgcanvas_bind_fbo(vg, &fbo), RET_OK);
  ASSERT_EQ(vgcanvas_set_fill_mode(vg, VGCANVAS_FILL_MODE_ALL_FILL), RET_OK);
  ASSERT_EQ(vgcanvas_set_fill_color(vg, color_init(0xFF, 0, 0, 0xFF)), RET_OK);
  ASSERT_EQ(vgcanvas_begin_path(vg), RET_OK);

  // ALL_FILL 不依赖绕向，反向子路径也不能被剔除。
  // 外三角和内三角都应保留，画布外的点应保持空白。
  vgcanvas_move_to(vg, 10, 10);
  vgcanvas_line_to(vg, 70, 10);
  vgcanvas_line_to(vg, 40, 70);
  vgcanvas_close_path(vg);

  vgcanvas_move_to(vg, 30, 30);
  vgcanvas_line_to(vg, 40, 50);
  vgcanvas_line_to(vg, 50, 30);
  vgcanvas_close_path(vg);

  ASSERT_EQ(vgcanvas_fill(vg), RET_OK);
  ASSERT_EQ(vgcanvas_end_frame(vg), RET_OK);

  img = vgcanvas_fill_read_fbo(vg, &fbo);
  ASSERT_NE(img, nullptr);
  // 外三角和反向内三角都应保留。
  ASSERT_TRUE(vgcanvas_fill_has_pixel(img, &fbo, 20, 20));
  ASSERT_TRUE(vgcanvas_fill_has_pixel(img, &fbo, 40, 35));
  // 画布外不应有任何填充。
  ASSERT_FALSE(vgcanvas_fill_has_pixel(img, &fbo, 90, 70));

  bitmap_destroy(img);
  fbo.custom_draw_model = TRUE;
  ASSERT_EQ(vgcanvas_unbind_fbo(vg, &fbo), RET_OK);
  vgcanvas_destroy_fbo(vg, &fbo);
  widget_destroy(win);
  native_window_sdl_deinit();
}

#endif /* WITH_GPU */
