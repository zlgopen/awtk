/**
 * File:   vgcanvas.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-04-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#if defined(WITH_NANOVG_GL3)
#define NANOVG_GL3_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES2)
#define NANOVG_GLES2_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES3)
#define NANOVG_GLES3_IMPLEMENTATION
#endif

#include "base/utf8.h"
#include "base/mem.h"
#include "base/vgcanvas.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"

#include "nanovg.h"

#ifdef WITH_NANOVG_AGGE
#include "agge/nanovg_agge.h"
#else /*OpenGL*/
#include "glad/glad.h"
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#endif /*WITH_NANOVG_AGGE*/

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
  NVGcontext* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;
#ifdef WITH_NANOVG_AGGE
#else
  SDL_Window* sdl_window;
#endif /*WITH_NANOVG_AGGE*/
} vgcanvas_nanovg_t;

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img);

static ret_t vgcanvas_nanovg_reset(vgcanvas_t* vgcanvas) {
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_flush(vgcanvas_t* vgcanvas) {
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_clear_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                        float_t h, color_t c) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  color_t fill_color = vgcanvas->fill_color;
  vgcanvas_set_fill_color(vgcanvas, c);
  nvgBeginPath(vg);
  nvgRect(vg, x, y, w, h);
  nvgClosePath(vg);
  nvgFill(vg);
  vgcanvas_set_fill_color(vgcanvas, fill_color);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_path(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgBeginPath(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_move_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgMoveTo(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_line_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgLineTo(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_quad_to(vgcanvas_t* vgcanvas, float_t cpx, float_t cpy, float_t x,
                                     float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgQuadTo(vg, cpx, cpy, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_bezier_to(vgcanvas_t* vgcanvas, float_t cp1x, float_t cp1y,
                                       float_t cp2x, float_t cp2y, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgBezierTo(vg, cp1x, cp1y, cp2x, cp2y, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_arc_to(vgcanvas_t* vgcanvas, float_t x1, float_t y1, float_t x2,
                                    float_t y2, float_t r) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgArcTo(vg, x1, y1, x2, y2, r);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_arc(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t r,
                                 float_t start, float_t end, bool_t ccw) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgArc(vg, x, y, r, start, end, ccw ? NVG_CCW : NVG_CW);

  return RET_OK;
}

static bool_t vgcanvas_nanovg_is_point_in_path(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  (void)vg;
  (void)x;
  (void)y;

  /*TODO*/

  return FALSE;
}

static ret_t vgcanvas_nanovg_rotate(vgcanvas_t* vgcanvas, float_t rad) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgRotate(vg, rad);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_scale(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgScale(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_translate(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTranslate(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                       float_t d, float_t e, float_t f) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTransform(vg, a, b, c, d, e, f);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                           float_t d, float_t e, float_t f) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgResetTransform(vg);
  nvgTransform(vg, a, b, c, d, e, f);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_rounded_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                          float_t h, float_t r) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgRoundedRect(vg, x, y, w, h, r);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_ellipse(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t rx,
                                     float_t ry) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgEllipse(vg, x, y, rx, ry);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_close_path(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgClosePath(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_clip_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                       float_t h) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgScissor(vg, x, y, w, h);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_fill(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgFill(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_stroke(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgStroke(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_paint(vgcanvas_t* vgcanvas, bool_t stroke, bitmap_t* img) {
  int iw = img->w;
  int ih = img->h;
  NVGpaint imgPaint;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  int id = vgcanvas_nanovg_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, id, 1);

  if (stroke) {
    nvgStrokePaint(vg, imgPaint);
    nvgStroke(vg);
  } else {
    nvgFillPaint(vg, imgPaint);
    nvgClosePath(vg);
    nvgFill(vg);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  nvgFontSize(vg, size);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_font(vgcanvas_t* vgcanvas, const char* name) {
  int font_id = 0;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (name == NULL) {
    name = TK_DEFAULT_FONT;
  }

  font_id = nvgFindFont(vg, name);
  if (font_id < 0) {
    const asset_info_t* r = assets_manager_ref(assets_manager(), ASSET_TYPE_FONT, name);
    if (r == NULL || r->subtype != ASSET_TYPE_FONT_TTF) {
      name = TK_DEFAULT_FONT;
      font_id = nvgFindFont(vg, name);
      if (font_id >= 0) {
        canvas->font_id = font_id;
        return RET_OK;
      }

      r = assets_manager_ref(assets_manager(), ASSET_TYPE_FONT, name);
    }

    if (r != NULL && r->subtype == ASSET_TYPE_FONT_TTF) {
      font_id = nvgCreateFontMem(vg, name, (unsigned char*)r->data, r->size, 0);
    }
  }

  return_value_if_fail(font_id >= 0, RET_FAIL);

  vgcanvas->font = name;
  canvas->font_id = font_id;
  nvgFontFaceId(vg, font_id);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (text_align[0] == 'r') {
    canvas->text_align_h = NVG_ALIGN_RIGHT;
  } else if (text_align[0] == 'c') {
    canvas->text_align_h = NVG_ALIGN_CENTER;
  } else {
    canvas->text_align_h = NVG_ALIGN_LEFT;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_text_baseline(vgcanvas_t* vgcanvas, const char* text_baseline) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (text_baseline[0] == 'b') {
    canvas->text_align_v = NVG_ALIGN_BOTTOM;
  } else if (text_baseline[0] == 'm') {
    canvas->text_align_v = NVG_ALIGN_MIDDLE;
  } else {
    canvas->text_align_v = NVG_ALIGN_TOP;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x, float_t y,
                                       float_t max_width) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTextAlign(vg, canvas->text_align_v | canvas->text_align_h);
  nvgText(vg, x, y, text, text + strlen(text));

  return RET_OK;
}

static float_t vgcanvas_nanovg_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  float bounds[4];
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTextAlign(vg, canvas->text_align_v | canvas->text_align_h);

  return nvgTextBounds(vg, 0, 0, text, text + strlen(text), bounds);
}

#ifndef WITH_NANOVG_AGGE
static ret_t nanovg_on_bitmap_destroy(bitmap_t* img) {
  int32_t id = (char*)(img->specific) - (char*)NULL;
  NVGcontext* vg = (NVGcontext*)(img->specific_ctx);

  if (vg != NULL && id >= 0) {
    nvgDeleteImage(vg, id);
  }
  img->specific = NULL;
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;

  return RET_OK;
}
#endif /*WITH_NANOVG_AGGE*/

static ret_t vgcanvas_nanovg_draw_image(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx, float_t sy,
                                        float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                        float_t dh) {
  int iw = img->w;
  int ih = img->h;
  NVGpaint imgPaint;
  float scaleX = (float)dw / sw;
  float scaleY = (float)dh / sh;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  int id = vgcanvas_nanovg_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, id, 1);

  nvgSave(vg);
  nvgBeginPath(vg);
  nvgTranslate(vg, dx - (sx * scaleX), dy - (sy * scaleY));
  nvgScale(vg, scaleX, scaleY);
  nvgRect(vg, sx, sy, sw, sh);
  nvgFillPaint(vg, imgPaint);
  nvgClosePath(vg);
  nvgFill(vg);
  nvgBeginPath(vg);
  nvgRestore(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_antialias(vgcanvas_t* vgcanvas, bool_t value) {
  (void)vgcanvas;
  (void)value;
  /*always*/

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_line_width(vgcanvas_t* vgcanvas, float_t value) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgStrokeWidth(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_global_alpha(vgcanvas_t* vgcanvas, float_t value) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgGlobalAlpha(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_fill_color(vgcanvas_t* vgcanvas, color_t c) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgFillColor(vg, nvgRGBA(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_stroke_color(vgcanvas_t* vgcanvas, color_t c) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgStrokeColor(vg, nvgRGBA(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_line_cap(vgcanvas_t* vgcanvas, const char* value) {
  int line_cap = NVG_BUTT;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_cap = NVG_ROUND;
  } else if (*value == 's') {
    line_cap = NVG_SQUARE;
  }

  nvgLineCap(vg, line_cap);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_line_join(vgcanvas_t* vgcanvas, const char* value) {
  int line_join = NVG_MITER;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_join = NVG_ROUND;
  } else if (*value == 'b') {
    line_join = NVG_BEVEL;
  }

  nvgLineJoin(vg, line_join);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_miter_limit(vgcanvas_t* vgcanvas, float_t value) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgMiterLimit(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_save(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgSave(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_restore(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgRestore(vg);

  return RET_OK;
}

#if defined(WITH_NANOVG_AGGE)
static agge_bitmap_format_t bitmap_format_to_agge(bitmap_format_t format) {
  agge_bitmap_format_t f = AGGE_RGBA8888;

  switch (format) {
    case BITMAP_FMT_RGBA: {
      f = AGGE_RGBA8888;
      break;
    }
    case BITMAP_FMT_BGRA: {
      f = AGGE_BGRA8888;
      break;
    }
    case BITMAP_FMT_RGB565: {
      f = AGGE_RGB565;
      break;
    }
    default: {
      assert(!"not supported format");
      break;
    }
  }

  return f;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h,
                                    bitmap_format_t format, void* data) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  vgcanvas->w = w;
  vgcanvas->h = h;
  vgcanvas->buff = (uint32_t*)data;
  nvgReinitAgge(vg, w, h, bitmap_format_to_agge(format), data);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgBeginFrame(vg, vgcanvas->w, vgcanvas->h, vgcanvas->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);

  return RET_OK;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int32_t i = 0;
  int32_t f = 0;
  if (img->flags & BITMAP_FLAG_TEXTURE) {
    i = (char*)(img->specific) - (char*)NULL;

    return i;
  }

  switch (img->format) {
    case BITMAP_FMT_RGBA: {
      f = NVG_TEXTURE_RGBA;
      break;
    }
    case BITMAP_FMT_BGRA: {
      f = NVG_TEXTURE_BGRA;
      break;
    }
    case BITMAP_FMT_RGB565: {
      f = NVG_TEXTURE_RGB565;
      break;
    }
    case BITMAP_FMT_RGB: {
      f = NVG_TEXTURE_RGB;
      break;
    }
    default: { assert(!"not supported format"); }
  }

  i = nvgCreateImageRaw(canvas->vg, img->w, img->h, f, NVG_IMAGE_NEAREST, img->data);

  if (i >= 0) {
    img->flags |= BITMAP_FLAG_TEXTURE;
    img->specific = (char*)NULL + i;
    image_manager_update_specific(image_manager(), img);
  }

  return i;
}
#else
static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, bitmap_format_t format,
                                    void* data) {
  (void)vg;
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  int ww = 0;
  int wh = 0;
  int fw = 0;
  int fh = 0;

  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  SDL_Window* sdl_window = canvas->sdl_window;
  NVGcontext* vg = canvas->vg;

  SDL_GetWindowSize(sdl_window, &ww, &wh);
  SDL_GL_GetDrawableSize(sdl_window, &fw, &fh);

  glViewport(0, 0, fw, fh);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  nvgBeginFrame(vg, ww, wh, vgcanvas->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  SDL_Window* sdl_window = canvas->sdl_window;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);
  SDL_GL_SwapWindow(sdl_window);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGLUframebuffer* handle = NULL;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  handle = nvgluCreateFramebuffer(vg, (int)(vgcanvas->w * vgcanvas->ratio),
                                  (int)(vgcanvas->h * vgcanvas->ratio), 0);
  return_value_if_fail(handle != NULL, RET_FAIL);

  fbo->w = vgcanvas->w;
  fbo->h = vgcanvas->h;
  fbo->handle = handle;
  fbo->id = handle->image;
  fbo->ratio = vgcanvas->ratio;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;
  nvgluDeleteFramebuffer(handle);
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;

  nvgluBindFramebuffer(handle);
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  nvgBeginFrame(vg, fbo->w, fbo->h, fbo->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgEndFrame(vg);
  nvgluBindFramebuffer(NULL);

  return RET_OK;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int32_t i = 0;
  if (img->flags & BITMAP_FLAG_TEXTURE) {
    i = (char*)(img->specific) - (char*)NULL;

    if (img->flags & BITMAP_FLAG_CHANGED) {
      img->flags &= (~(BITMAP_FLAG_CHANGED));
      nvgUpdateImage(canvas->vg, i, img->data);
      log_debug("nvgUpdateImage %d\n", i);
    }

    return i;
  }

  i = nvgCreateImageRGBA(canvas->vg, img->w, img->h, NVG_IMAGE_NEAREST, img->data);

  if (i >= 0) {
    img->flags |= BITMAP_FLAG_TEXTURE;
    img->specific = (char*)NULL + i;
    img->specific_ctx = canvas->vg;
    img->specific_destroy = nanovg_on_bitmap_destroy;
    image_manager_update_specific(image_manager(), img);
  }

  return i;
}

#endif /*WITH_NANOVG_AGGE*/

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
#if defined(WITH_NANOVG_GL3)
  nvgDeleteGL3(vg);
#elif defined(WITH_NANOVG_GLES2)
  nvgDeleteGLES2(vg);
#elif defined(WITH_NANOVG_GLES3)
  nvgDeleteGLES3(vg);
#elif defined(WITH_NANOVG_AGGE)
  nvgDeleteAGGE(vg);
#endif

  return RET_OK;
}

static const vgcanvas_vtable_t vt = {vgcanvas_nanovg_reinit,
                                     vgcanvas_nanovg_begin_frame,
                                     vgcanvas_nanovg_reset,
                                     vgcanvas_nanovg_flush,
                                     vgcanvas_nanovg_clear_rect,
                                     vgcanvas_nanovg_begin_path,
                                     vgcanvas_nanovg_move_to,
                                     vgcanvas_nanovg_line_to,
                                     vgcanvas_nanovg_arc,
                                     vgcanvas_nanovg_arc_to,
                                     vgcanvas_nanovg_bezier_to,
                                     vgcanvas_nanovg_quad_to,
                                     vgcanvas_nanovg_is_point_in_path,
                                     vgcanvas_nanovg_ellipse,
                                     vgcanvas_nanovg_rounded_rect,
                                     vgcanvas_nanovg_close_path,
                                     vgcanvas_nanovg_scale,
                                     vgcanvas_nanovg_rotate,
                                     vgcanvas_nanovg_translate,
                                     vgcanvas_nanovg_transform,
                                     vgcanvas_nanovg_set_transform,
                                     vgcanvas_nanovg_clip_rect,
                                     vgcanvas_nanovg_fill,
                                     vgcanvas_nanovg_stroke,
                                     vgcanvas_nanovg_paint,
                                     vgcanvas_nanovg_set_font,
                                     vgcanvas_nanovg_set_font_size,
                                     vgcanvas_nanovg_set_text_align,
                                     vgcanvas_nanovg_set_text_baseline,
                                     vgcanvas_nanovg_fill_text,
                                     vgcanvas_nanovg_measure_text,
                                     vgcanvas_nanovg_draw_image,
                                     vgcanvas_nanovg_set_antialias,
                                     vgcanvas_nanovg_set_global_alpha,
                                     vgcanvas_nanovg_set_line_width,
                                     vgcanvas_nanovg_set_fill_color,
                                     vgcanvas_nanovg_set_stroke_color,
                                     vgcanvas_nanovg_set_line_join,
                                     vgcanvas_nanovg_set_line_cap,
                                     vgcanvas_nanovg_set_miter_limit,
                                     vgcanvas_nanovg_save,
                                     vgcanvas_nanovg_restore,
                                     vgcanvas_nanovg_end_frame,
                                     vgcanvas_nanovg_create_fbo,
                                     vgcanvas_nanovg_destroy_fbo,
                                     vgcanvas_nanovg_bind_fbo,
                                     vgcanvas_nanovg_unbind_fbo,
                                     vgcanvas_nanovg_destroy};

#if defined(WITH_NANOVG_AGGE)
vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, bitmap_format_t format, void* data) {
  agge_bitmap_format_t f = bitmap_format_to_agge(format);
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->base.ratio = 1;
  nanovg->base.buff = (uint32_t*)data;

  nanovg->vg = nvgCreateAGGE(w, h, f, (uint8_t*)data);

  return &(nanovg->base);
}
#else /*OpenGL*/
vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, bitmap_format_t format, void* sdl_window) {
  int ww = 0;
  int wh = 0;
  int fw = 0;
  int fh = 0;
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  (void)format;

  SDL_GetWindowSize((SDL_Window*)sdl_window, &ww, &wh);
  SDL_GL_GetDrawableSize((SDL_Window*)sdl_window, &fw, &fh);

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->base.ratio = (float)fw / (float)ww;
  nanovg->sdl_window = (SDL_Window*)sdl_window;
#if defined(WITH_NANOVG_GL3)
  nanovg->vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif defined(WITH_NANOVG_GLES2)
  nanovg->vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif defined(WITH_NANOVG_GLES3)
  nanovg->vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif

  if (nanovg->vg == NULL) {
    assert(!"OpenGL is not supported!");
  }

  return &(nanovg->base);
}
#endif
