#include "nanovg_plus.h"
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "base/events.h"
#include "base/vgcanvas.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "base/font_manager.h"
#include "base/assets_manager.h"
#include "base/vgcanvas_asset_manager.h"

static int vgcanvas_nanovg_plus_ensure_image(vgcanvas_nanovg_plus_t* canvas, bitmap_t* img);

static ret_t vgcanvas_nanovg_plus_destroy(vgcanvas_t* vgcanvas);
static ret_t vgcanvas_nanovg_plus_end_frame(vgcanvas_t* vgcanvas);
static ret_t vgcanvas_nanovg_plus_set_antialias(vgcanvas_t* vgcanvas, bool_t value);
static ret_t vgcanvas_nanovg_plus_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo);
static ret_t vgcanvas_nanovg_plus_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data);
static ret_t vgcanvas_nanovg_plus_begin_frame(vgcanvas_t* vgcanvas, const dirty_rects_t* dirty_rects);
static ret_t vgcanvas_nanovg_plus_create_fbo(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h,
                                        bool_t custom_draw_model, framebuffer_object_t* fbo);
static ret_t vgcanvas_nanovg_plus_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo);
static ret_t vgcanvas_nanovg_plus_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo);
static ret_t vgcanvas_nanovg_plus_fbo_to_bitmap(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                           bitmap_t* img, const rect_t* r);

static ret_t vgcanvas_nanovg_plus_on_asset_events(void* ctx, event_t* e) {
  assets_event_t* evt = (assets_event_t*)e;

  if (evt->type == ASSET_TYPE_FONT) {
    asset_info_t* info = evt->asset_info;
    if (e->type == EVT_ASSET_MANAGER_CLEAR_CACHE) {
      vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager(), ctx, NULL);
    } else if (e->type == EVT_ASSET_MANAGER_UNLOAD_ASSET) {
      vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager(), ctx, info->name);
    }
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_init(vgcanvas_t* vgcanvas) {
  (void)vgcanvas;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_assets_manager(vgcanvas_t* vg, assets_manager_t* assets_manager) {
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);

  if (vg->assets_manager != NULL && vg->assets_manager != assets_manager) {
    emitter_off_by_ctx(EMITTER(vg->assets_manager), vg);
  }
  if (assets_manager != NULL && vg->assets_manager != assets_manager) {
    emitter_t* emitter = EMITTER(assets_manager);
    emitter_on(emitter, EVT_ASSET_MANAGER_CLEAR_CACHE, vgcanvas_nanovg_plus_on_asset_events, vg);
    emitter_on(emitter, EVT_ASSET_MANAGER_UNLOAD_ASSET, vgcanvas_nanovg_plus_on_asset_events, vg);
  }
  vg->assets_manager = assets_manager;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_deinit(vgcanvas_t* vgcanvas) {
  emitter_t* emitter = EMITTER(assets_manager());
  emitter_off_by_ctx(emitter, vgcanvas);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_reset(vgcanvas_t* vgcanvas) {
  /*TODO: unload fonts*/
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_delete_font_by_name(vg, NULL);
  vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager(), vgcanvas, NULL);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_reset_curr_state(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_reset_curr_state(vg);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_flush(vgcanvas_t* vgcanvas) {
#ifdef WITH_GPU
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_end_frame(vg);
#endif
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_clear_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                        float_t h, color_t c) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  color_t fill_color = vgcanvas->fill_color;
  vgcanvas_set_fill_color(vgcanvas, c);
  nvgp_begin_path(vg);
  nvgp_rect(vg, x, y, w, h);
  nvgp_close_path(vg);
  nvgp_fill(vg);
  vgcanvas_set_fill_color(vgcanvas, fill_color);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_begin_path(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_begin_path(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_move_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_move_to(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_line_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_line_to(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_quad_to(vgcanvas_t* vgcanvas, float_t cpx, float_t cpy, float_t x,
                                     float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_quad_to(vg, cpx, cpy, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_bezier_to(vgcanvas_t* vgcanvas, float_t cp1x, float_t cp1y,
                                       float_t cp2x, float_t cp2y, float_t x, float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_bezier_to(vg, cp1x, cp1y, cp2x, cp2y, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_arc_to(vgcanvas_t* vgcanvas, float_t x1, float_t y1, float_t x2,
                                    float_t y2, float_t r) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_arc_to(vg, x1, y1, x2, y2, r);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_arc(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t r,
                                 float_t start, float_t end, bool_t ccw) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_arc(vg, x, y, r, start, end, ccw);

  return RET_OK;
}

static bool_t vgcanvas_nanovg_plus_is_point_in_path(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  (void)vg;
  (void)x;
  (void)y;

  /*TODO*/

  return FALSE;
}

static ret_t vgcanvas_nanovg_plus_rotate(vgcanvas_t* vgcanvas, float_t rad) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_rotate(vg, rad);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_scale(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_scale(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_translate(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_translate(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                       float_t d, float_t e, float_t f) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_transform(vg, a, b, c, d, e, f);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                           float_t d, float_t e, float_t f) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_reset_transform(vg);
  nvgp_transform(vg, a, b, c, d, e, f);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_rounded_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                          float_t h, float_t r) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_rounded_rect(vg, x, y, w, h, r);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_ellipse(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t rx,
                                     float_t ry) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_ellipse(vg, x, y, rx, ry);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_close_path(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_close_path(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_path_winding(vgcanvas_t* vgcanvas, bool_t dir) {
  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_intersect_clip_rect(vgcanvas_t* vgcanvas, float_t* x, float_t* y,
                                                 float_t* w, float_t* h) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_intersect_scissor(vg, x, y, w, h);
  nvgp_get_curr_clip_rect(vg, &vgcanvas->clip_rect.x, &vgcanvas->clip_rect.y, &vgcanvas->clip_rect.w, &vgcanvas->clip_rect.h);

  return RET_OK;
}

static bool_t vgcanvas_nanovg_plus_is_rectf_in_clip_rect(vgcanvas_t* vgcanvas, float_t left, float_t top, float_t right, float_t bottom) {
  float_t clip_left = vgcanvas->clip_rect.x;
  float_t clip_right = vgcanvas->clip_rect.x + vgcanvas->clip_rect.w;
  float_t clip_top = vgcanvas->clip_rect.y;
  float_t clip_bottom = vgcanvas->clip_rect.y + vgcanvas->clip_rect.h;
  if (left > clip_right || right < clip_left || top > clip_bottom || bottom < clip_top) {
    return FALSE;
  }
  return TRUE;
}

static ret_t vgcanvas_nanovg_plus_clip_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                       float_t h) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_scissor(vg, x, y, w, h);
  nvgp_get_curr_clip_rect(vg, &vgcanvas->clip_rect.x, &vgcanvas->clip_rect.y, &vgcanvas->clip_rect.w, &vgcanvas->clip_rect.h);
  return RET_OK;
}

const rectf_t* vgcanvas_nanovg_plus_get_clip_rect(vgcanvas_t* vgcanvas) {
  return &(vgcanvas->clip_rect);
}

static ret_t vgcanvas_nanovg_plus_fill(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_fill(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_stroke(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_stroke(vg);

  return RET_OK;
}

static int vgcanvas_nanovg_plus_bitmap_flag_to_NVGimageFlags(bitmap_flag_t flags) {
  if(flags & BITMAP_FLAG_PREMULTI_ALPHA) {
    return NVGP_GL_IMAGE_PREMULTIPLIED;
  }
  return 0;
}

static int vgcanvas_nanovg_plus_ensure_image(vgcanvas_nanovg_plus_t* canvas, bitmap_t* img) {
  int flag = 0;
  int32_t i = 0;
  uint8_t* data = NULL;
  framebuffer_object_t* fbo = NULL;
  uint32_t bpp = bitmap_get_bpp(img);
  vgcanvas_nanovg_plus_gl_texture_t* texture = NULL;
  uint8_t* img_data = bitmap_lock_buffer_for_read(img);
  return_value_if_fail(img != NULL && graphic_buffer_is_valid_for(img->buffer, img), -1);

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    ret_t reuslt = RET_FAIL;
    const void* specific = NULL;
    if (img->flags & BITMAP_FLAG_GPU_FBO_TEXTURE) {
      img->flags &= ~BITMAP_FLAG_GPU_FBO_TEXTURE;
      i = tk_pointer_to_int(img->specific);
      fbo = (framebuffer_object_t*)img->specific_ctx;
      goto create_texture_info;
    } else {
      specific = vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, &reuslt);
    }
    if (reuslt == RET_OK) {
      i = ((vgcanvas_nanovg_plus_gl_texture_t*)specific)->image_id;

      if (img->flags & BITMAP_FLAG_CHANGED) {
        img->flags &= (~(BITMAP_FLAG_CHANGED));
        nvgp_update_image_rgba(canvas->vg, i, img_data);
      }
      bitmap_unlock_buffer(img);

      return i;
    }
  }

  flag = (int)vgcanvas_nanovg_plus_bitmap_flag_to_NVGimageFlags((bitmap_flag_t)(img->flags));

  if (bpp * img->w == img->line_length) {
    data = (uint8_t*)(img_data);
  } else {
    uint32_t size = 0;
    size = bpp * img->w * img->h;
    size = TK_ROUND_TO(size, BITMAP_ALIGN_SIZE) + BITMAP_ALIGN_SIZE;
    data = (uint8_t*)TKMEM_ALLOC(size);
    memset(data, 0x00, size);
    int j;
    for (j = 0; j < img->h; j++) {
      memcpy(data + j * img->w * bpp, img_data + j * img->line_length, img->w * bpp);
    }
  }

  i = nvgp_create_image_rgba(canvas->vg, img->w, img->h, flag, data);

  if (data != img_data) {
    TKMEM_FREE(data);
  }

create_texture_info:
  if (i >= 0) {
    texture = TKMEM_ZALLOC(vgcanvas_nanovg_plus_gl_texture_t);
    assert(texture != NULL);
    texture->fbo = fbo;
    texture->image_id = i;
    img->flags |= BITMAP_FLAG_TEXTURE;
    vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, texture);
  }
  bitmap_unlock_buffer(img);

  return i;
}

static ret_t vgcanvas_nanovg_plus_paint(vgcanvas_t* vgcanvas, bool_t stroke, bitmap_t* img) {
  int iw = img->w;
  int ih = img->h;
  nvgp_paint_t img_paint;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  int id = vgcanvas_nanovg_plus_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  img_paint = nvgp_image_pattern(vg, 0, 0, iw, ih, 0, id, 0xFF);

  if (stroke) {
    nvgp_set_stroke_paint(vg, img_paint);
    nvgp_stroke(vg);
  } else {
    nvgp_set_fill_paint(vg, img_paint);
    nvgp_close_path(vg);
    nvgp_fill(vg);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_set_font_size(vg, size);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_font(vgcanvas_t* vgcanvas, const char* name) {
  int font_id = 0;
  ret_t reuslt = RET_FAIL;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  return_value_if_fail(name && *name, RET_BAD_PARAMS);

  font_id = tk_pointer_to_int(vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vgcanvas, name, &reuslt));

  if (reuslt != RET_OK) {
    assets_manager_t* am = vgcanvas->assets_manager ? vgcanvas->assets_manager : assets_manager();
    const asset_info_t* r = assets_manager_ref(am, ASSET_TYPE_FONT, name);

    if (r == NULL || r->subtype != ASSET_TYPE_FONT_TTF) {
      font_id = nvgp_find_font(vg, name);
      if (font_id >= 0) {
        canvas->font_id = font_id;
        return RET_OK;
      }

      if (r == NULL) {
        font_t* font = font_manager_get_font(font_manager(), NULL, TK_DEFAULT_FONT_SIZE);
        if (font != NULL) {
          r = assets_manager_ref(assets_manager(), ASSET_TYPE_FONT, font->name);
        }
      }
    }

    if (r != NULL && r->subtype == ASSET_TYPE_FONT_TTF) {
      font_id = nvgp_create_font_mem(vg, name, (unsigned char*)r->data, r->size, 0);
      if (font_id < 0) {
        log_warn("load font %s failed\n", name);
      }
    }

    if (r != NULL) {
      assets_manager_unref(am, r);
    }
    vgcanvas_asset_manager_add_font(vgcanvas_asset_manager(), vgcanvas, name, tk_pointer_from_int(font_id));
  }

  canvas->font_id = font_id;
  nvgp_font_face_id(vg, font_id);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;

  if (text_align[0] == 'r') {
    canvas->text_align_h = NVGP_ALIGN_RIGHT;
  } else if (text_align[0] == 'c') {
    canvas->text_align_h = NVGP_ALIGN_CENTER;
  } else {
    canvas->text_align_h = NVGP_ALIGN_LEFT;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_text_baseline(vgcanvas_t* vgcanvas, const char* text_baseline) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;

  if (text_baseline[0] == 'b') {
    canvas->text_align_v = NVGP_ALIGN_BOTTOM;
  } else if (text_baseline[0] == 'm') {
    canvas->text_align_v = NVGP_ALIGN_MIDDLE;
  } else {
    canvas->text_align_v = NVGP_ALIGN_TOP;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x, float_t y,
                                       float_t max_width) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_text_align(vg, canvas->text_align_v | canvas->text_align_h);
  nvgp_text(vg, x, y, text, text + strlen(text));

  return RET_OK;
}

static float_t vgcanvas_nanovg_plus_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  float bounds[4];
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_text_align(vg, canvas->text_align_v | canvas->text_align_h);

  return nvgp_text_bounds(vg, 0, 0, text, text + strlen(text), bounds);
}

#ifdef WITH_GPU
static ret_t vgcanvas_nanovg_plus_get_text_metrics(vgcanvas_t* vgcanvas, float_t* ascent,
                                              float_t* descent, float_t* line_hight) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  float a = 0;
  float d = 0;
  float h = 0;

  nvgp_text_metrics(vg, &a, &d, &h);
  if (ascent != NULL) {
    *ascent = a;
  }

  if (descent != NULL) {
    *descent = d;
  }

  if (line_hight != NULL) {
    *line_hight = h;
  }

  return RET_OK;
}
#else
#define vgcanvas_nanovg_plus_get_text_metrics NULL
#endif /*WITH_GPU*/

static ret_t vgcanvas_nanovg_plus_draw_image(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx, float_t sy,
                                        float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                        float_t dh) {
  int iw = img->w;
  int ih = img->h;
  nvgp_paint_t img_paint;
  float scaleX = (float)dw / sw;
  float scaleY = (float)dh / sh;
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  int id = vgcanvas_nanovg_plus_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  img_paint = nvgp_image_pattern(vg, 0, 0, iw, ih, 0, id, 0xFF);

  nvgp_save(vg);
  nvgp_begin_path(vg);
  nvgp_translate(vg, dx - (sx * scaleX), dy - (sy * scaleY));
  nvgp_scale(vg, scaleX, scaleY);
  nvgp_rect(vg, sx, sy, sw, sh);
  nvgp_set_fill_paint(vg, img_paint);
  nvgp_close_path(vg);
  nvgp_fill(vg);
  nvgp_begin_path(vg);
  nvgp_restore(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_draw_image_repeat(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx, float_t sy,
                                                    float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                                    float_t dh, float_t dst_w, float_t dst_h) {
  nvgp_paint_t img_paint;
  float scaleX = (float)dw / sw;
  float scaleY = (float)dh / sh;
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  int id = vgcanvas_nanovg_plus_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);
  (void)dst_w;
  (void)dst_h;
  img_paint = nvgp_image_pattern_repeat(vg, sx, sy, sw, sh, dw, dh, img->w, img->h, 0, id, 0xFF);

  nvgp_save(vg);
  nvgp_begin_path(vg);
  nvgp_translate(vg, dx - (sx * scaleX), dy - (sy * scaleY));
  nvgp_scale(vg, scaleX, scaleY);
  nvgp_rect(vg, sx, sy, sw, sh);
  nvgp_set_fill_paint(vg, img_paint);
  nvgp_close_path(vg);
  nvgp_fill(vg);
  nvgp_begin_path(vg);
  nvgp_restore(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_antialias(vgcanvas_t* vgcanvas, bool_t value) {
  vgcanvas_nanovg_plus_t* canvas = (vgcanvas_nanovg_plus_t*)vgcanvas;
  nvgp_context_t* vg = canvas->vg;
  nvgp_set_shape_anti_alias(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_line_width(vgcanvas_t* vgcanvas, float_t value) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_set_stroke_width(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_global_alpha(vgcanvas_t* vgcanvas, float_t value) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  vgcanvas->global_alpha = value;
  nvgp_set_global_alpha(vg, value * 255.0f);

  return RET_OK;
}

static nvgp_color_t to_nvgp_color(color_t c) {
  return nvgp_color_init(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);
}

static ret_t vgcanvas_nanovg_plus_set_fill_color(vgcanvas_t* vgcanvas, color_t c) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_set_fill_color(vg, to_nvgp_color(c));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_fill_linear_gradient(vgcanvas_t* vgcanvas, float_t sx, float_t sy,
                                                      float_t ex, float_t ey, color_t icolor,
                                                      color_t ocolor) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_paint_t paint = nvgp_linear_gradient(vg, sx, sy, ex, ey, to_nvgp_color(icolor), to_nvgp_color(ocolor));
  nvgp_set_fill_paint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_fill_radial_gradient(vgcanvas_t* vgcanvas, float_t cx, float_t cy,
                                                      float_t inr, float_t outr, color_t icolor,
                                                      color_t ocolor) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_paint_t paint = nvgp_radial_gradient(vg, cx, cy, inr, outr, to_nvgp_color(icolor), to_nvgp_color(ocolor));
  nvgp_set_fill_paint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_stroke_color(vgcanvas_t* vgcanvas, color_t c) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_set_stroke_color(vg, to_nvgp_color(c));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_stroke_linear_gradient(vgcanvas_t* vgcanvas, float_t sx,
                                                        float_t sy, float_t ex, float_t ey,
                                                        color_t icolor, color_t ocolor) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_paint_t paint = nvgp_linear_gradient(vg, sx, sy, ex, ey, to_nvgp_color(icolor), to_nvgp_color(ocolor));
  nvgp_set_stroke_paint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_stroke_radial_gradient(vgcanvas_t* vgcanvas, float_t cx,
                                                        float_t cy, float_t inr, float_t outr,
                                                        color_t icolor, color_t ocolor) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;
  nvgp_paint_t paint = nvgp_radial_gradient(vg, cx, cy, inr, outr, to_nvgp_color(icolor), to_nvgp_color(ocolor));
  nvgp_set_stroke_paint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_line_cap(vgcanvas_t* vgcanvas, const char* value) {
  int line_cap = NVGP_BUTT;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_cap = NVGP_ROUND;
  } else if (*value == 's') {
    line_cap = NVGP_SQUARE;
  } else if (*value == 'b') {
    line_cap = NVGP_BUTT;
  }

  nvgp_set_line_cap(vg, line_cap);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_line_join(vgcanvas_t* vgcanvas, const char* value) {
  int line_join = NVGP_MITER;
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_join = NVGP_ROUND;
  } else if (*value == 'b') {
    line_join = NVGP_BEVEL;
  }

  nvgp_set_line_join(vg, line_join);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_set_miter_limit(vgcanvas_t* vgcanvas, float_t value) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_set_miter_limit(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_save(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_save(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_restore(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_restore(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_plus_clear_cache(vgcanvas_t* vgcanvas) {
  nvgp_context_t* vg = ((vgcanvas_nanovg_plus_t*)vgcanvas)->vg;

  nvgp_clear_cache(vg);
  nvgp_delete_font_by_name(vg, NULL);
  vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager(), vgcanvas, NULL);
  vgcanvas_asset_manager_remove_image(vgcanvas_asset_manager(), vgcanvas, NULL);
  
  return RET_OK;
}

static const vgcanvas_vtable_t vt = {
    .reinit = vgcanvas_nanovg_plus_reinit,
    .begin_frame = vgcanvas_nanovg_plus_begin_frame,
    .set_assets_manager = vgcanvas_nanovg_plus_set_assets_manager,
    .reset = vgcanvas_nanovg_plus_reset,
    .reset_curr_state = vgcanvas_nanovg_plus_reset_curr_state,
    .flush = vgcanvas_nanovg_plus_flush,
    .clear_rect = vgcanvas_nanovg_plus_clear_rect,
    .begin_path = vgcanvas_nanovg_plus_begin_path,
    .move_to = vgcanvas_nanovg_plus_move_to,
    .line_to = vgcanvas_nanovg_plus_line_to,
    .arc = vgcanvas_nanovg_plus_arc,
    .arc_to = vgcanvas_nanovg_plus_arc_to,
    .bezier_to = vgcanvas_nanovg_plus_bezier_to,
    .quad_to = vgcanvas_nanovg_plus_quad_to,
    .is_point_in_path = vgcanvas_nanovg_plus_is_point_in_path,
    .ellipse = vgcanvas_nanovg_plus_ellipse,
    .rounded_rect = vgcanvas_nanovg_plus_rounded_rect,
    .close_path = vgcanvas_nanovg_plus_close_path,
    .scale = vgcanvas_nanovg_plus_scale,
    .rotate = vgcanvas_nanovg_plus_rotate,
    .translate = vgcanvas_nanovg_plus_translate,
    .transform = vgcanvas_nanovg_plus_transform,
    .set_transform = vgcanvas_nanovg_plus_set_transform,
    .clip_rect = vgcanvas_nanovg_plus_clip_rect,
    .get_clip_rect = vgcanvas_nanovg_plus_get_clip_rect,
    .is_rectf_in_clip_rect = vgcanvas_nanovg_plus_is_rectf_in_clip_rect,
    .intersect_clip_rect = vgcanvas_nanovg_plus_intersect_clip_rect,
    .path_winding = vgcanvas_nanovg_plus_path_winding,
    .fill = vgcanvas_nanovg_plus_fill,
    .stroke = vgcanvas_nanovg_plus_stroke,
    .paint = vgcanvas_nanovg_plus_paint,
    .set_font = vgcanvas_nanovg_plus_set_font,
    .set_font_size = vgcanvas_nanovg_plus_set_font_size,
    .set_text_align = vgcanvas_nanovg_plus_set_text_align,
    .set_text_baseline = vgcanvas_nanovg_plus_set_text_baseline,
    .fill_text = vgcanvas_nanovg_plus_fill_text,
    .measure_text = vgcanvas_nanovg_plus_measure_text,
    .draw_image = vgcanvas_nanovg_plus_draw_image,
    .draw_image_repeat = vgcanvas_nanovg_plus_draw_image_repeat,
    .set_antialias = vgcanvas_nanovg_plus_set_antialias,
    .set_global_alpha = vgcanvas_nanovg_plus_set_global_alpha,
    .set_line_width = vgcanvas_nanovg_plus_set_line_width,
    .set_fill_color = vgcanvas_nanovg_plus_set_fill_color,
    .set_fill_linear_gradient = vgcanvas_nanovg_plus_set_fill_linear_gradient,
    .set_fill_radial_gradient = vgcanvas_nanovg_plus_set_fill_radial_gradient,
    .set_stroke_color = vgcanvas_nanovg_plus_set_stroke_color,
    .set_stroke_linear_gradient = vgcanvas_nanovg_plus_set_stroke_linear_gradient,
    .set_stroke_radial_gradient = vgcanvas_nanovg_plus_set_stroke_radial_gradient,
    .set_line_join = vgcanvas_nanovg_plus_set_line_join,
    .set_line_cap = vgcanvas_nanovg_plus_set_line_cap,
    .set_miter_limit = vgcanvas_nanovg_plus_set_miter_limit,
    .get_text_metrics = vgcanvas_nanovg_plus_get_text_metrics,
    .save = vgcanvas_nanovg_plus_save,
    .restore = vgcanvas_nanovg_plus_restore,
    .end_frame = vgcanvas_nanovg_plus_end_frame,
    .create_fbo = vgcanvas_nanovg_plus_create_fbo,
    .bind_fbo = vgcanvas_nanovg_plus_bind_fbo,
    .destroy_fbo = vgcanvas_nanovg_plus_destroy_fbo,
    .unbind_fbo = vgcanvas_nanovg_plus_unbind_fbo,
    .fbo_to_bitmap = vgcanvas_nanovg_plus_fbo_to_bitmap,
    .clear_cache = vgcanvas_nanovg_plus_clear_cache,
    .destroy = vgcanvas_nanovg_plus_destroy};
