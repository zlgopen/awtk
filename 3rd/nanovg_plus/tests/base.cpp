
#include <gtest/gtest.h>
#include "nanovg_plus.h"
#include "gtest_nvpg.h"

static int vg_width = 800;
static int vg_height = 800;

static int32_t gtest_pointer_to_int(const void* p) {
  return (char*)p - (char*)(NULL);
}

static void* gtest_pointer_from_int(int32_t v) {
  return ((char*)NULL) + v;
}

TEST(base, create_vt) {
  gtest_nvgp_ctx_t vt_ctx;
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  nvgp_destroy(vg_ctx);
}

TEST(base, create_begin_and_end_frame) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);

  ASSERT_EQ(vt_ctx.width, vg_width);
  ASSERT_EQ(vt_ctx.height, vg_height);
  ASSERT_EQ(vt_ctx.pixel_ratio, 1.0f);

  nvgp_end_frame(vg_ctx);
  ASSERT_EQ(vt_ctx.test_info, END_FRAME_INFO);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, fill_rect) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t fill_color = nvgp_color_init(0xff, 0x0, 0x0, 0xff);
  nvgp_set_fill_color(vg_ctx, fill_color);
  nvgp_rect(vg_ctx, 0.0f, 0.0f, 100.0f, 100.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, fill_color.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(path->fill[0].x, 0.0f);
  ASSERT_EQ(path->fill[0].y, 0.0f);
  ASSERT_EQ(path->fill[2].x, 100.0f);
  ASSERT_EQ(path->fill[2].y, 100.0f);
  
  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, stroke) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t stroke_color = nvgp_color_init(0xff, 0x0, 0x0, 0xff);
  nvgp_set_stroke_color(vg_ctx, stroke_color);
  nvgp_rect(vg_ctx, 0.0f, 0.0f, 100.0f, 100.0f);
  nvgp_stroke(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, stroke_color.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nstroke, 10);
  ASSERT_EQ(path->stroke[0].x, 0.5f);
  ASSERT_EQ(path->stroke[0].y, 0.5f);
  ASSERT_EQ(path->stroke[1].x, -0.5f);
  ASSERT_EQ(path->stroke[1].y, -0.5f);
  ASSERT_EQ(path->stroke[2].x, 0.5f);
  ASSERT_EQ(path->stroke[2].y, 99.5f);
  ASSERT_EQ(path->stroke[3].x, -0.5f);
  ASSERT_EQ(path->stroke[3].y, 100.5f);
  ASSERT_EQ(path->stroke[4].x, 99.5f);
  ASSERT_EQ(path->stroke[4].y, 99.5f);
  ASSERT_EQ(path->stroke[5].x, 100.5f);
  ASSERT_EQ(path->stroke[5].y, 100.5f);
  ASSERT_EQ(path->stroke[6].x, 99.5f);
  ASSERT_EQ(path->stroke[6].y, 0.5f);
  ASSERT_EQ(path->stroke[7].x, 100.5f);
  ASSERT_EQ(path->stroke[7].y, -0.5f);
  ASSERT_EQ(path->stroke[8].x, 0.5f);
  ASSERT_EQ(path->stroke[8].y, 0.5f);
  ASSERT_EQ(path->stroke[9].x, -0.5f);
  ASSERT_EQ(path->stroke[9].y, -0.5f);
  
  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, draw_image) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  vt_ctx.image = 0xcc;
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  int32_t iw = 200, ih = 200;
  int32_t image_id = nvgp_create_image_rgba(vg_ctx, iw, ih, 0, (const unsigned char*)gtest_pointer_from_int(0xff));

  nvgp_paint_t img_paint = nvgp_image_pattern(vg_ctx, 0, 0, iw, ih, 0, image_id, 0xff);
  nvgp_set_shape_anti_alias(vg_ctx, FALSE);
  nvgp_scale(vg_ctx, 1.0f, 1.0f);
  nvgp_rect(vg_ctx, 0, 0, iw, ih);
  nvgp_set_fill_paint(vg_ctx, img_paint);
  nvgp_fill(vg_ctx);
  nvgp_set_shape_anti_alias(vg_ctx, TRUE);

  ASSERT_EQ(vt_ctx.paint.image, image_id);
  ASSERT_EQ(vt_ctx.paint.inner_color.rgba.a, 0xff);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(path->fill[0].x, 0.0f);
  ASSERT_EQ(path->fill[0].y, 0.0f);
  ASSERT_EQ(path->fill[2].x, (float)iw);
  ASSERT_EQ(path->fill[2].y, (float)ih);
  
  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, translate_rect) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t fill_color = nvgp_color_init(0xff, 0x0, 0x0, 0xff);
  nvgp_translate(vg_ctx, 100.0f, 100.0f);
  nvgp_set_fill_color(vg_ctx, fill_color);
  nvgp_rect(vg_ctx, 0.0f, 0.0f, 100.0f, 100.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, fill_color.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(path->fill[0].x, 100.0f);
  ASSERT_EQ(path->fill[0].y, 100.0f);
  ASSERT_EQ(path->fill[2].x, 200.0f);
  ASSERT_EQ(path->fill[2].y, 200.0f);
  
  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, scale_rect) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t fill_color = nvgp_color_init(0xff, 0x0, 0x0, 0xff);
  nvgp_scale(vg_ctx, 2.0f, 2.0f);
  nvgp_set_fill_color(vg_ctx, fill_color);
  nvgp_rect(vg_ctx, 0.0f, 0.0f, 100.0f, 100.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, fill_color.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(path->fill[0].x, 0.0f);
  ASSERT_EQ(path->fill[0].y, 0.0f);
  ASSERT_EQ(path->fill[2].x, 200.0f);
  ASSERT_EQ(path->fill[2].y, 200.0f);
  
  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, rotate_rect) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t fill_color = nvgp_color_init(0xff, 0x0, 0x0, 0xff);
  nvgp_translate(vg_ctx, 150.0f, 150.0f);
  nvgp_rotate(vg_ctx, NVGP_PI / 2);
  nvgp_translate(vg_ctx, -150.0f, -150.0f);
  nvgp_set_fill_color(vg_ctx, fill_color);
  nvgp_rect(vg_ctx, 100.0f, 100.0f, 100.0f, 100.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, fill_color.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[0].x - 200.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[0].y - 100.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[1].x - 100.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[1].y - 100.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[2].x - 100.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[2].y - 200.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[3].x - 200.0f), true);
  ASSERT_EQ(0.001f > nvgp_abs(path->fill[3].y - 200.0f), true);
  
  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, gradient) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t color1 = nvgp_color_init(0x0, 0x0, 0xff, 0xff);
  nvgp_color_t color2 = nvgp_color_init(0xff, 0x0, 0x0, 0xff);

  nvgp_paint_t paint = nvgp_linear_gradient(vg_ctx, 0, 0, vg_width, vg_height, color1, color2);
  nvgp_set_fill_paint(vg_ctx, paint);
  nvgp_rect(vg_ctx, 0.0f, 0.0f, 100.0f, 100.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, color1.color);
  ASSERT_EQ(vt_ctx.paint.outer_color.color, color2.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

TEST(base, save_and_restore) {
  gtest_nvgp_ctx_t vt_ctx = {0};
  nvgp_context_t* vg_ctx = nvgp_create_by_vt(vg_width, vg_height, nvgp_gtest_vtable(), &vt_ctx);
  ASSERT_EQ(vg_ctx != NULL, true);
  
  nvgp_begin_frame(vg_ctx, vg_width, vg_height, 1.0f);
  nvgp_scissor(vg_ctx, 0, 0, vg_width, vg_height);

  nvgp_color_t fill_color = nvgp_color_init(0xff, 0x0, 0x0, 0xff);
  nvgp_color_t fill_color1 = nvgp_color_init(0xff, 0xff, 0x0, 0xff);
  nvgp_set_fill_color(vg_ctx, fill_color);

  nvgp_save(vg_ctx);
  nvgp_scissor(vg_ctx, 0, 0, 200.0f, 200.0f);
  nvgp_translate(vg_ctx, 100.0f, 100.0f);
  nvgp_set_fill_color(vg_ctx, fill_color1);
  nvgp_rect(vg_ctx, 0.0f, 0.0f, 100.0f, 100.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, fill_color1.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], 100.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], 100.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  nvgp_path_t* path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(path->fill[0].x, 100.0f);
  ASSERT_EQ(path->fill[0].y, 100.0f);
  ASSERT_EQ(path->fill[2].x, 200.0f);
  ASSERT_EQ(path->fill[2].y, 200.0f);
  nvgp_restore(vg_ctx);

  nvgp_begin_path(vg_ctx);
  nvgp_rect(vg_ctx, 0.0f, 100.0f, 200.0f, 200.0f);
  nvgp_fill(vg_ctx);

  ASSERT_EQ(vt_ctx.paint.inner_color.color, fill_color.color);
  ASSERT_EQ(vt_ctx.scissor.extent[0], vg_width / 2.0f);
  ASSERT_EQ(vt_ctx.scissor.extent[1], vg_height / 2.0f);

  ASSERT_EQ(vt_ctx.paths.size, 1);
  
  path = nvgp_darray_get_ptr(&vt_ctx.paths, 0, nvgp_path_t);

  ASSERT_EQ(path->nfill, 4);
  ASSERT_EQ(path->fill[0].x, 0.0f);
  ASSERT_EQ(path->fill[0].y, 100.0f);
  ASSERT_EQ(path->fill[2].x, 200.0f);
  ASSERT_EQ(path->fill[2].y, 300.0f);

  nvgp_end_frame(vg_ctx);
  
  nvgp_destroy(vg_ctx);
}

