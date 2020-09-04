/**
 * File:   canvas_offline.c
 * Author: AWTK Develop Team
 * Brief:  offline canvas.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-07-10 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "tkc/wstr.h"
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "base/canvas.h"
#include "base/system_info.h"

#include "widget.h"
#include "bitmap.h"
#include "native_window.h"
#include "widget_consts.h"
#include "window_manager.h"
#include "canvas_offline.h"

#ifdef WITH_NANOVG_GPU

#ifdef WITHOUT_GLAD
#include <SDL.h>

#ifdef IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/

#else
#include "glad/glad.h"
#endif /*WITHOUT_GLAD*/

typedef struct _canvas_offline_gpu_t {
  canvas_offline_t base;
  uint32_t online_lcd_w;
  uint32_t online_lcd_h;
  framebuffer_object_t fbo;
} canvas_offline_gpu_t;

#else

#include "../blend/image_g2d.h"
#include "../lcd/lcd_mem_rgb565.h"
#include "../lcd/lcd_mem_bgr565.h"
#include "../lcd/lcd_mem_rgba8888.h"
#include "../lcd/lcd_mem_bgra8888.h"

#endif

canvas_t* canvas_offline_create(uint32_t w, uint32_t h, bitmap_format_t format) {
  canvas_t* c = NULL;
  native_window_t* native_window = NULL;

#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = NULL;
  framebuffer_object_t* fbo = NULL;
  canvas_offline_gpu_t* canvas = NULL;
#else
  lcd_t* lcd = NULL;
  uint8_t* buff = NULL;
  bitmap_t* bitmap = NULL;
  canvas_offline_t* canvas = NULL;
#endif

#ifdef FRAGMENT_FRAME_BUFFER_SIZE
  log_warn(" fragment frame buffer not supported yet\n");
  return NULL;
#endif

  native_window =
      (native_window_t*)widget_get_prop_pointer(window_manager(), WIDGET_PROP_NATIVE_WINDOW);
  return_value_if_fail(native_window != NULL, NULL);

  c = native_window_get_canvas(native_window);
#ifdef WITH_NANOVG_GPU

  if (format != BITMAP_FMT_RGBA8888) {
    assert(!" opengl gpu only supported RGBA foramt ");
    log_warn(" opengl gpu only supported RGBA foramt \n");
    return NULL;
  }

  canvas = TKMEM_ZALLOC(canvas_offline_gpu_t);
  return_value_if_fail(canvas != NULL, NULL);

  fbo = &(canvas->fbo);
  canvas->base.bitmap = bitmap_create_ex(w, h, 0, BITMAP_FMT_RGBA8888);

  canvas->base.begin_draw = 0;
  canvas->online_lcd_w = c->lcd->w;
  canvas->online_lcd_h = c->lcd->h;

  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(c != NULL && vg != NULL, NULL);

  canvas_init((canvas_t*)canvas, c->lcd, widget_get_font_manager(window_manager()));
  canvas_set_assets_manager((canvas_t*)canvas, widget_get_assets_manager(window_manager()));
  canvas_set_global_alpha((canvas_t*)canvas, 0xff);
  vgcanvas_create_fbo(vg, w, h, fbo);

  return (canvas_t*)canvas;
#else

  canvas = TKMEM_ZALLOC(canvas_offline_t);
  return_value_if_fail(canvas != NULL, NULL);

  canvas->begin_draw = 0;
  canvas->bitmap = bitmap_create_ex(w, h, 0, format);

  bitmap = canvas->bitmap;

  buff = bitmap_lock_buffer_for_write(bitmap);
  return_value_if_fail(buff != NULL, NULL);
  if (bitmap->format == BITMAP_FMT_RGBA8888) {
    lcd = lcd_mem_rgba8888_create_single_fb(bitmap->w, bitmap->h, buff);
  } else if (bitmap->format == BITMAP_FMT_BGRA8888) {
    lcd = lcd_mem_bgra8888_create_single_fb(bitmap->w, bitmap->h, buff);
  } else if (bitmap->format == BITMAP_FMT_BGR565) {
    lcd = lcd_mem_bgr565_create_single_fb(bitmap->w, bitmap->h, buff);
  } else if (bitmap->format == BITMAP_FMT_RGB565) {
    lcd = lcd_mem_rgb565_create_single_fb(bitmap->w, bitmap->h, buff);
  } else {
    assert(!" bitmap format not supported yet \n");
    log_warn(" bitmap format not supported yet \n");
  }
  bitmap_unlock_buffer(bitmap);

  if (lcd != NULL) {
    canvas_t* c_tmp = NULL;
    ((lcd_mem_t*)lcd)->vgcanvas = canvas_get_vgcanvas(c);
    c_tmp = canvas_init(&(canvas->base), lcd, widget_get_font_manager(window_manager()));
    canvas_set_assets_manager(c_tmp, widget_get_assets_manager(window_manager()));
    canvas_set_global_alpha(&(canvas->base), 0xff);
    canvas_begin_frame(c_tmp, NULL, LCD_DRAW_OFFLINE);
    return c_tmp;
  } else {
    return NULL;
  }

#endif
}

ret_t canvas_offline_clear_canvas(canvas_t* canvas) {
#ifndef WITH_NANOVG_GPU
  rect_t rect;
  canvas_offline_t* canvas_offline = NULL;
#endif
  return_value_if_fail(canvas != NULL, RET_BAD_PARAMS);
#ifdef WITH_NANOVG_GPU
  glEnable(GL_SCISSOR_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);
#else
  canvas_offline = (canvas_offline_t*)canvas;
  rect = rect_init(0, 0, canvas_offline->bitmap->w, canvas_offline->bitmap->h);
  image_clear(canvas_offline->bitmap, &rect, color_init(0x0, 0x0, 0x0, 0x0));
#endif
  return RET_OK;
}

ret_t canvas_offline_begin_draw(canvas_t* canvas) {
  vgcanvas_t* vg = NULL;
  canvas_offline_t* canvas_offline = NULL;
#ifdef WITH_NANOVG_GPU
  canvas_offline_gpu_t* c = NULL;
#endif

  return_value_if_fail(canvas != NULL, RET_BAD_PARAMS);
  vg = lcd_get_vgcanvas(canvas->lcd);
  canvas_offline = (canvas_offline_t*)canvas;
#ifdef WITH_NANOVG_GPU
  c = (canvas_offline_gpu_t*)canvas;
  if (vg != NULL && canvas_offline->begin_draw == 0) {
    canvas_get_clip_rect(canvas, &canvas_offline->canvas_clip_rect);
    canvas_offline->vg_clip_rect =
        rect_init(vg->clip_rect.x, vg->clip_rect.y, vg->clip_rect.w, vg->clip_rect.h);
    c->online_lcd_w = canvas->lcd->w;
    c->online_lcd_h = canvas->lcd->h;

    vgcanvas_flush(vg);

    vgcanvas_bind_fbo(vg, &c->fbo);
    vgcanvas_save(vg);

    canvas->lcd->w = vg->w = canvas_offline->bitmap->w;
    canvas->lcd->h = vg->h = canvas_offline->bitmap->h;
    canvas_set_clip_rect(canvas, NULL);
  }
#else
  if (canvas_offline->begin_draw == 0) {
    canvas_get_clip_rect(canvas, &canvas_offline->canvas_clip_rect);
    if (vg != NULL) {
      canvas_offline->vg_clip_rect =
          rect_init(vg->clip_rect.x, vg->clip_rect.y, vg->clip_rect.w, vg->clip_rect.h);

      vgcanvas_save(vg);
      vgcanvas_reset_curr_state(vg);
      vgcanvas_clip_rect(vg, 0, 0, vg->w, vg->h);
    }
    canvas_set_clip_rect(canvas, NULL);
  }
#endif
  canvas_offline->begin_draw++;
  return RET_OK;
}

ret_t canvas_offline_end_draw(canvas_t* canvas) {
  vgcanvas_t* vg = NULL;
  canvas_offline_t* canvas_offline = NULL;
#ifdef WITH_NANOVG_GPU
  canvas_offline_gpu_t* c = NULL;
#endif

  return_value_if_fail(canvas != NULL, RET_BAD_PARAMS);
  vg = lcd_get_vgcanvas(canvas->lcd);
  canvas_offline = (canvas_offline_t*)canvas;

  canvas_offline->begin_draw--;
#ifdef WITH_NANOVG_GPU
  c = (canvas_offline_gpu_t*)canvas;
  if (vg != NULL && canvas_offline->begin_draw == 0) {
    uint16_t flag = BITMAP_FLAG_CHANGED;

    canvas->lcd->w = vg->w = c->online_lcd_w;
    canvas->lcd->h = vg->h = c->online_lcd_h;

    vgcanvas_restore(vg);
    vgcanvas_unbind_fbo(vg, &c->fbo);

    c->base.bitmap->specific = tk_pointer_from_int(c->fbo.id);

    c->base.bitmap->flags &= (~flag);
    c->base.bitmap->flags |= BITMAP_FLAG_TEXTURE;

    canvas_set_clip_rect(canvas, &canvas_offline->canvas_clip_rect);
  }
#else
  if (canvas_offline->begin_draw == 0) {
    canvas_set_clip_rect(canvas, &canvas_offline->canvas_clip_rect);
    if (vg != NULL) {
      vgcanvas_clip_rect(vg, canvas_offline->vg_clip_rect.x, canvas_offline->vg_clip_rect.y,
                         canvas_offline->vg_clip_rect.w, canvas_offline->vg_clip_rect.h);
      vgcanvas_restore(vg);
    }
  }
#endif
  return RET_OK;
}

bitmap_t* canvas_offline_get_bitmap(canvas_t* canvas) {
  canvas_offline_t* c = (canvas_offline_t*)canvas;
  return_value_if_fail(canvas != NULL && c != NULL, NULL);
  return c->bitmap;
}

ret_t canvas_offline_bitmap_move_to_new_bitmap(canvas_t* canvas, bitmap_t* bitmap) {
  bool_t should_free_handle;
  bitmap_t* canvas_bitmap = NULL;
  return_value_if_fail(canvas != NULL && bitmap != NULL, RET_BAD_PARAMS);

  should_free_handle = bitmap->should_free_handle;
  canvas_bitmap = ((canvas_offline_t*)canvas)->bitmap;

  memcpy(bitmap, canvas_bitmap, sizeof(bitmap_t));

  TKMEM_FREE(canvas_bitmap);
  bitmap->should_free_handle = should_free_handle;

  ((canvas_offline_t*)canvas)->bitmap = NULL;

  return RET_OK;
}

ret_t canvas_offline_flush_bitmap(canvas_t* canvas) {
#ifdef WITH_NANOVG_GPU
  rect_t r;
  vgcanvas_t* vg = NULL;
  canvas_offline_gpu_t* c = NULL;
#endif
  return_value_if_fail(canvas != NULL, RET_BAD_PARAMS);
#ifdef WITH_NANOVG_GPU
  vg = lcd_get_vgcanvas(canvas->lcd);
  if (vg != NULL) {
    c = (canvas_offline_gpu_t*)canvas;
    r = rect_init(0, 0, c->base.bitmap->w, c->base.bitmap->h);

    vgcanvas_fbo_to_bitmap(vg, &c->fbo, c->base.bitmap, &r);
    /* 不能把数据回流到 GPU 中，因为 fbo 颜色数据是预乘数据和垂直翻转的数据 */
    // c->base.bitmap->flags |= BITMAP_FLAG_CHANGED;
  }
#endif
  return RET_OK;
}

ret_t canvas_offline_destroy(canvas_t* canvas) {
  bitmap_t* bitmap = NULL;
#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = NULL;
  framebuffer_object_t* fbo = NULL;
#endif

  return_value_if_fail(canvas != NULL, RET_BAD_PARAMS);
  bitmap = ((canvas_offline_t*)canvas)->bitmap;
#ifdef WITH_NANOVG_GPU
  vg = lcd_get_vgcanvas(canvas->lcd);
  fbo = &((canvas_offline_gpu_t*)canvas)->fbo;

  vgcanvas_destroy_fbo(vg, fbo);
#else
  ((lcd_mem_t*)canvas->lcd)->vgcanvas = NULL;
  lcd_destroy(canvas->lcd);
#endif
  if (bitmap != NULL) {
    bitmap_destroy(bitmap);
  }
  TKMEM_FREE(canvas);

  return RET_OK;
}
