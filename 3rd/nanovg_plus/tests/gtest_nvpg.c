
#include "gtest_nvpg.h"

static nvgp_bool_t gtest_clear_cache(void* uptr) {
  return TRUE;
}

static int gtest_find_texture(void* uptr, const void* data) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  if (ctx->data == data) {
    return ctx->image;
  } else {
    return 0;
  }
}

static int gtest_create_texture(void* uptr, int type, int w, int h, int stride, int image_flags, const unsigned char* data) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->type = type;
  ctx->w = w;
  ctx->h = h;
  ctx->stride = stride;
  ctx->image_flags = image_flags;
  ctx->data = (void*)data;
  return ctx->image;
}

static nvgp_bool_t gtest_delete_texture(void* uptr, int image) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->image = 0;
  return TRUE;
}

static nvgp_bool_t gtest_update_texture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->x = x;
  ctx->y = y;
  ctx->w = w;
  ctx->h = h;
  ctx->image = image;
  ctx->data = (void*)data;
  return TRUE;
}

static nvgp_bool_t gtest_get_texture_size(void* uptr, int image, int* w, int* h) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  if (ctx->image != 0) {
    *w = ctx->w;
    *h = ctx->h;
    return 1;
  }
  return 0;
}

static int gtest_get_edge_anti_alias(void* uptr) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  return ctx->edge_anti_alias;
}

static void gtest_end_frame(void* uptr) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->test_info = END_FRAME_INFO;
}

static void gtest_begin_frame(void* uptr, float width, float height, float pixel_ratio) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->width = width;
  ctx->height = height;
  ctx->pixel_ratio = pixel_ratio;
}

static void gtest_set_line_cap(void* uptr, int line_cap) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->line_cap = line_cap;
}

static void gtest_set_line_join(void* uptr, int line_join) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->line_join = line_join;
}

static void gtest_render_cancel(void* uptr) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->test_info = RENDER_CANCEL_INFO;
}

static void gtest_render_fill(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->fringe = fringe;
  memcpy(&ctx->paint, paint, sizeof(nvgp_paint_t));
  memcpy(&ctx->paths, paths, sizeof(nvgp_darray_t));
  memcpy(ctx->bounds, bounds, sizeof(ctx->bounds));
  memcpy(&ctx->scissor, scissor, sizeof(nvgp_scissor_t));
}

static void gtest_render_stroke(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, float stroke_width, const nvgp_darray_t* paths) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->fringe = fringe;
  ctx->stroke_width = stroke_width;
  memcpy(&ctx->paint, paint, sizeof(nvgp_paint_t));
  memcpy(&ctx->paths, paths, sizeof(nvgp_darray_t));
  memcpy(&ctx->scissor, scissor, sizeof(nvgp_scissor_t));
}

static void gtest_render_draw_text(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, nvgp_vertex_t* verts, uint32_t nverts) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->verts = verts;
  ctx->nverts = nverts;
  memcpy(&ctx->paint, paint, sizeof(nvgp_paint_t));
  memcpy(&ctx->scissor, scissor, sizeof(nvgp_scissor_t));
}

static void gtest_destroy(void* uptr) {
  gtest_nvgp_ctx_t* ctx = (gtest_nvgp_ctx_t*)uptr;
  ctx->test_info = DESTROY_INFO;
}

static const nvgp_vtable_t vt = {
  .clear_cache = gtest_clear_cache,
  .find_texture = gtest_find_texture,
  .create_texture = gtest_create_texture,
  .delete_texture = gtest_delete_texture,
  .update_texture = gtest_update_texture,
  .get_texture_size = gtest_get_texture_size,
  .get_edge_anti_alias = gtest_get_edge_anti_alias,

  .end_frame = gtest_end_frame,
  .begin_frame = gtest_begin_frame,
  .set_line_cap = gtest_set_line_cap,
  .set_line_join = gtest_set_line_join,
  .render_cancel = gtest_render_cancel,
  .render_fill = gtest_render_fill,
  .render_stroke = gtest_render_stroke,
  .render_draw_text = gtest_render_draw_text,
  .destroy = gtest_destroy,
  
};

const nvgp_vtable_t* nvgp_gtest_vtable() {
  return &vt;
} 
