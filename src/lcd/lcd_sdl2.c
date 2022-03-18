/**
 * File:   lcd_sdl2.c
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented lcd interface/
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "lcd/lcd_sdl2.h"
#include "blend/image_g2d.h"
#include "base/system_info.h"
#include "lcd/lcd_mem_special.h"
#include "base/lcd_orientation_helper.h"

typedef struct _special_info_t {
  SDL_Renderer* render;
  SDL_Texture* texture;
  bitmap_format_t format;
} special_info_t;

static special_info_t* special_info_create(SDL_Renderer* render) {
  special_info_t* info = TKMEM_ZALLOC(special_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->render = render;

  return info;
}

static ret_t special_info_create_texture(special_info_t* info, wh_t w, wh_t h) {
  int flags = SDL_TEXTUREACCESS_STREAMING;

#ifdef WITH_FB_BGRA8888
  /*SDL ABGR is rgba from low address to high address*/
  info->format = BITMAP_FMT_BGRA8888;
  info->texture = SDL_CreateTexture(info->render, SDL_PIXELFORMAT_ARGB8888, flags, w, h);
  log_debug("WITH_FB_BGRA8888\n");
#else
  /*SDL ABGR is rgba from low address to high address*/
  info->format = BITMAP_FMT_BGR565;
  info->texture = SDL_CreateTexture(info->render, SDL_PIXELFORMAT_RGB565, flags, w, h);
  log_debug("WITH_FB_BGR565\n");
#endif
  return info->texture != NULL ? RET_OK : RET_FAIL;
}

static ret_t special_info_destroy(special_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  SDL_DestroyTexture(info->texture);
  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t lcd_sdl2_flush(lcd_t* lcd) {
  bitmap_t src;
  bitmap_t dst;
  int pitch = 0;
  void* addr = NULL;
  const dirty_rects_t* dirty_rects;
  lcd_mem_t* lcd_mem = (lcd_mem_t*)lcd;
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  special_info_t* info = (special_info_t*)(special->ctx);
  uint8_t* offline_fb = lcd_mem_get_offline_fb(lcd_mem);
  lcd_orientation_t o = system_info()->lcd_orientation;

  memset(&src, 0x00, sizeof(src));
  memset(&dst, 0x00, sizeof(dst));

  dirty_rects =
      lcd_fb_dirty_rects_get_dirty_rects_by_fb(&(lcd_mem->fb_dirty_rects_list), offline_fb);
  if (dirty_rects != NULL && dirty_rects->nr > 0) {
#ifdef WITH_FAST_LCD_PORTRAIT
    uint32_t lcd_w = lcd_get_physical_width(lcd);
    uint32_t lcd_h = lcd_get_physical_height(lcd);
    SDL_Rect sr = {0, 0, lcd_w, lcd_h};
#else
    uint32_t lcd_w = lcd->w;
    uint32_t lcd_h = lcd->h;
    SDL_Rect sr = {0, 0, lcd_w, lcd_h};
    if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
      sr.w = lcd->h;
      sr.h = lcd->w;
    }
#endif

    SDL_LockTexture(info->texture, NULL, (void**)&(addr), &pitch);
    bitmap_init(&dst, sr.w, sr.h, special->format, addr);
    bitmap_set_line_length(&dst, pitch);
    bitmap_init(&src, lcd_w, lcd_h, special->format, offline_fb);
    if (dirty_rects->disable_multiple) {
      const rect_t* dr = (const rect_t*)&(dirty_rects->max);
#ifdef WITH_FAST_LCD_PORTRAIT
      rect_t rr;
      if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
        rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, src.h, src.w);
      } else {
        rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, src.w, src.h);
      }
      image_copy(&dst, &src, &rr, rr.x, rr.y);
#else
      if (o == LCD_ORIENTATION_0) {
        image_copy(&dst, &src, dr, dr->x, dr->y);
      } else {
        image_rotate(&dst, &src, dr, o);
      }
#endif
    } else {
      uint32_t i = 0;
      for (i = 0; i < dirty_rects->nr; i++) {
#ifdef WITH_FAST_LCD_PORTRAIT
        rect_t rr;
        const rect_t* dr = (const rect_t*)dirty_rects->rects + i;
        if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
          rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, src.h, src.w);
        } else {
          rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, src.w, src.h);
        }
        image_copy(&dst, &src, &rr, rr.x, rr.y);
#else
        const rect_t* dr = (const rect_t*)dirty_rects->rects + i;
        if (o == LCD_ORIENTATION_0) {
          image_copy(&dst, &src, dr, dr->x, dr->y);
        } else {
          image_rotate(&dst, &src, dr, o);
        }
#endif
      }
    }

    SDL_UnlockTexture(info->texture);

    SDL_RenderCopy(info->render, info->texture, &sr, &sr);

    if (src.buffer != NULL) {
      graphic_buffer_destroy(src.buffer);
    }
    if (dst.buffer != NULL) {
      graphic_buffer_destroy(dst.buffer);
    }
  }

  if (lcd->draw_mode != LCD_DRAW_OFFLINE) {
    SDL_RenderPresent(info->render);
  }

  return RET_OK;
}

static ret_t lcd_sdl2_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  special_info_t* info = (special_info_t*)(special->ctx);

  SDL_DestroyTexture(info->texture);

  info->texture = NULL;
  special_info_create_texture(info, w, h);

  return RET_OK;
}

static ret_t lcd_sdl2_destroy(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  special_info_destroy((special_info_t*)(special->ctx));
  special->ctx = NULL;

  return RET_OK;
}

lcd_t* lcd_sdl2_init(SDL_Renderer* render) {
  int w = 0;
  int h = 0;
  special_info_t* info = NULL;
  return_value_if_fail(render != NULL, NULL);

  SDL_GetRendererOutputSize(render, &w, &h);
  info = special_info_create(render);
  return_value_if_fail(info != NULL, NULL);

  ENSURE(special_info_create_texture(info, w, h) == RET_OK);

  return lcd_mem_special_create(w, h, info->format, lcd_sdl2_flush, lcd_sdl2_resize,
                                lcd_sdl2_destroy, info);
}
