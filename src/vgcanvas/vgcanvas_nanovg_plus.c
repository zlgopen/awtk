#include "tkc/utils.h"
#include "tkc/utf8.h"
#include "tkc/mem.h"
#include "base/vgcanvas.h"
#include "base/image_manager.h"
#include "base/native_window.h"

#include "nanovg_plus.h"
#include "nanovg_plus_gl.h"
#include "nanovg_plus_gl_utils.h"

typedef struct _vgcanvas_nanovg_plus_t {
  vgcanvas_t base;

  int font_id;
  nvgp_context_t* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;

  native_window_t* window;
} vgcanvas_nanovg_plus_t;

typedef struct _vgcanvas_nanovg_plus_gl_texture_t {
  int32_t image_id;
  framebuffer_object_t* fbo;
} vgcanvas_nanovg_plus_gl_texture_t;

#include "vgcanvas_nanovg_plus.inc"

static ret_t vgcanvas_nanovg_plus_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                         bitmap_format_t format, void* data) {
  (void)vg;
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_begin_frame(vgcanvas_t* vgcanvas,
                                              const dirty_rects_t* dirty_rects) {
  float_t angle = 0.0f;
  float_t anchor_x = 0.0f;
  float_t anchor_y = 0.0f;

  system_info_t* info = system_info();
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  const rect_t* dirty_rect = dirty_rects != NULL ? &(dirty_rects->max) : NULL;

  native_window_gl_make_current(canvas->window);

  if (dirty_rect != NULL) {
    canvas->base.dirty_rect = rect_init(dirty_rect->x, dirty_rect->y, dirty_rect->w, dirty_rect->h);
  } else {
    if (info->lcd_orientation == LCD_ORIENTATION_90 ||
        info->lcd_orientation == LCD_ORIENTATION_270) {
      canvas->base.dirty_rect = rect_init(0, 0, info->lcd_h, info->lcd_w);
    } else {
      canvas->base.dirty_rect = rect_init(0, 0, info->lcd_w, info->lcd_h);
    }
  }

  nvgp_begin_frame(canvas->vg, info->lcd_w, info->lcd_h, info->device_pixel_ratio);

  switch (info->lcd_orientation) {
    case LCD_ORIENTATION_0:
      angle = 0.0f;
      break;
    case LCD_ORIENTATION_90:
      angle = TK_D2R(270);
      break;
    case LCD_ORIENTATION_180:
      angle = TK_D2R(180);
      break;
    case LCD_ORIENTATION_270:
      angle = TK_D2R(90);
      break;
  }

  anchor_x = info->lcd_w / 2.0f;
  anchor_y = info->lcd_h / 2.0f;

  nvgp_save(canvas->vg);

  if (info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
    nvgp_translate(canvas->vg, anchor_x, anchor_y);
    nvgp_rotate(canvas->vg, angle);
    nvgp_translate(canvas->vg, -anchor_y, -anchor_x);
  } else if (info->lcd_orientation == LCD_ORIENTATION_180) {
    nvgp_translate(canvas->vg, anchor_x, anchor_y);
    nvgp_rotate(canvas->vg, angle);
    nvgp_translate(canvas->vg, -anchor_x, -anchor_y);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = canvas->vg;

  nvgp_restore(vg);
  nvgp_end_frame(vg);

  native_window_swap_buffer(canvas->window);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_create_fbo(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h,
                                             bool_t custom_draw_model, framebuffer_object_t* fbo) {
  nvgp_gl_util_framebuffer* handle = NULL;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  handle =
      nvgp_gl_create_framebuffer(vg, (int)(w * vgcanvas->ratio), (int)(h * vgcanvas->ratio), 0);
  return_value_if_fail(handle != NULL, RET_FAIL);

  fbo->w = w;
  fbo->h = h;
  fbo->init = FALSE;
  fbo->handle = handle;
  fbo->id = handle->image;
  fbo->ratio = vgcanvas->ratio;
  fbo->offline_fbo = handle->fbo;
  fbo->custom_draw_model = custom_draw_model;
  fbo->online_fbo = nvgp_gl_get_curr_framebuffer();

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  nvgp_gl_util_framebuffer* handle = (nvgp_gl_util_framebuffer*)fbo->handle;
  handle->fbo = fbo->offline_fbo;
  nvgp_gl_delete_framebuffer(handle);
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  nvgp_context_t* vg = NULL;
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_gl_util_framebuffer* handle = (nvgp_gl_util_framebuffer*)fbo->handle;

  vg = canvas->vg;

  fbo->online_fbo = nvgp_gl_get_curr_framebuffer();
  fbo->online_dirty_rect = rect_init(vgcanvas->dirty_rect.x, vgcanvas->dirty_rect.y,
                                     vgcanvas->dirty_rect.w, vgcanvas->dirty_rect.h);

  handle->fbo = fbo->offline_fbo;
  vgcanvas->dirty_rect = rect_init(0, 0, fbo->w, fbo->h);
  nvgp_gl_bind_framebuffer(handle);
  if (!fbo->custom_draw_model) {
    glViewport(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);
    if (!fbo->init) {
      glScissor(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);
      glEnable(GL_SCISSOR_TEST);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      glDisable(GL_SCISSOR_TEST);
      fbo->init = TRUE;
    }
    nvgp_begin_frame_ex(vg, fbo->w, fbo->h, fbo->ratio, FALSE);
    nvgp_save(vg);
    nvgp_reset_curr_state(vg);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  nvgp_context_t* vg = NULL;
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_gl_util_framebuffer* handle = (nvgp_gl_util_framebuffer*)fbo->handle;

  vg = canvas->vg;

  handle->fbo = fbo->online_fbo;
  if (!fbo->custom_draw_model) {
    nvgp_restore(vg);
    nvgp_end_frame(vg);
  }

  nvgp_gl_bind_framebuffer(handle);

  vgcanvas->dirty_rect = rect_init(fbo->online_dirty_rect.x, fbo->online_dirty_rect.y,
                                   fbo->online_dirty_rect.w, fbo->online_dirty_rect.h);
  glViewport(0, 0, vgcanvas->w * fbo->ratio, vgcanvas->h * fbo->ratio);
  glScissor(0, 0, vgcanvas->w * fbo->ratio, vgcanvas->h * fbo->ratio);
  if (!fbo->custom_draw_model) {
    nvgp_begin_frame_ex(vg, vgcanvas->w, vgcanvas->h, fbo->ratio, FALSE);
  }
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_fbo_to_bitmap(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                                bitmap_t* img, const rect_t* r) {
  uint32_t i = 0;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t height = 0;
  uint8_t* p = NULL;
  uint8_t* data = NULL;
  uint8_t* img_data = NULL;

  int online_fbo = nvgp_gl_get_curr_framebuffer();
  nvgp_gl_util_framebuffer* handle = (nvgp_gl_util_framebuffer*)fbo->handle;

  handle->fbo = fbo->offline_fbo;
  nvgp_gl_bind_framebuffer(handle);
  data = TKMEM_ZALLOCN(uint8_t, img->h * img->line_length);
  img_data = (uint8_t*)bitmap_lock_buffer_for_write(img);
  height = fbo->h * fbo->ratio;
  if (r != NULL) {
    x = r->x;
    y = r->y;
  }

  /* 因为 opengles 的原点坐标为左下角，所以需要把 AWTK 的坐标（AWTK 是右上角为原点的坐标系）转换为左下角为原点的坐标系*/
  nvgp_gl_read_current_framebuffer_data(x, height - img->h - y, img->w, img->h, fbo->w * fbo->ratio,
                                        height, data);

  p = data + ((img->h - 1) * img->line_length);

  /* 
   * 图像数据垂直翻转
   * opengles 出来的数据是预乘的数据，需要反算预乘。 
   */
  while (TRUE) {
    uint8_t* src_data = p;
    uint8_t* dst_data = img_data;
    for (i = 0; i < img->w; i++) {
      if (0x0 < src_data[3] && src_data[3] < 0xff) {
        uint8_t a = src_data[3] >> 2;
        dst_data[0] = src_data[0] >> 2 == a ? 0xff : (src_data[0] << 8) / src_data[3];
        dst_data[1] = src_data[1] >> 2 == a ? 0xff : (src_data[1] << 8) / src_data[3];
        dst_data[2] = src_data[2] >> 2 == a ? 0xff : (src_data[2] << 8) / src_data[3];
      } else {
        dst_data[0] = src_data[0];
        dst_data[1] = src_data[1];
        dst_data[2] = src_data[2];
      }
      dst_data[3] = src_data[3];

      src_data += 4;
      dst_data += 4;
    }
    if (p == data) {
      break;
    }
    p -= img->line_length;
    img_data += img->line_length;
  }

  bitmap_unlock_buffer(img);
  handle->fbo = online_fbo;
  nvgp_gl_bind_framebuffer(handle);
  TKMEM_FREE(data);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = canvas->vg;

  vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager(), vgcanvas);

  vgcanvas_nanovg_plus_deinit(vgcanvas);

  nvgp_destroy(vg);

  TKMEM_FREE(vgcanvas);

  return RET_OK;
}

static ret_t vgcanvas_asset_manager_nanovg_plus_font_destroy(void* vg, const char* font_name,
                                                             void* specific) {
  int32_t id = tk_pointer_to_int(specific);
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vg;
  if (canvas != NULL && canvas->vg != NULL && id >= 0) {
    nvgp_delete_font_by_name(canvas->vg, font_name);
  }
  return RET_OK;
}

static ret_t vgcanvas_asset_manager_nanovg_plus_bitmap_destroy(void* vg, void* specific) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vg;
  vgcanvas_nanovg_plus_gl_texture_t* texture = (vgcanvas_nanovg_plus_gl_texture_t*)specific;
  if (canvas != NULL && canvas->vg != NULL && texture != NULL) {
    if (texture->fbo != NULL) {
      vgcanvas_destroy_fbo((vgcanvas_t*)canvas, texture->fbo);
      TKMEM_FREE(texture->fbo);
    } else {
      nvgp_delete_image(canvas->vg, texture->image_id);
    }
  }
  if (texture) {
    TKMEM_FREE(texture);
  }
  return RET_OK;
}

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* win) {
  native_window_info_t info;
  native_window_t* window = NATIVE_WINDOW(win);
  return_value_if_fail(native_window_get_info(win, &info) == RET_OK, NULL);
  vgcanvas_nanovg_plus_t* nanovg = TKMEM_ZALLOC(vgcanvas_nanovg_plus_t);
  return_value_if_fail(nanovg != NULL, NULL);

  (void)format;

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->window = window;
  nanovg->base.ratio = info.ratio;

  vgcanvas_nanovg_plus_init((vgcanvas_t*)nanovg);

  nanovg->vg = nvgp_create(NVGP_MODE_GPU, w, h);

  if (nanovg->vg == NULL) {
    assert(!"OpenGL is not supported!");
  }
  log_info("nanovg_plus init \r\n");
  vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), &(nanovg->base),
                                vgcanvas_asset_manager_nanovg_plus_bitmap_destroy,
                                vgcanvas_asset_manager_nanovg_plus_font_destroy);
  return &(nanovg->base);
}
