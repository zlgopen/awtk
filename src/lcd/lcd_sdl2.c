/**
 * File:   lcd_sdl2.c
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented lcd interface/
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "lcd/lcd_sdl2.h"
#include "blend/image_g2d.h"
#include "lcd/lcd_mem_special.h"

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
  rect_t* dr = &(lcd->dirty_rect);
  rect_t* fps_r = &(lcd->fps_rect);
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  special_info_t* info = (special_info_t*)(special->ctx);

  memset(&src, 0x00, sizeof(src));
  memset(&dst, 0x00, sizeof(dst));

  if ((dr->w > 0 && dr->h > 0) || (fps_r->w > 0 && fps_r->h > 0)) {
    SDL_Rect sr = {0, 0, lcd->w, lcd->h};

    SDL_LockTexture(info->texture, NULL, (void**)&(addr), &pitch);
    bitmap_init(&dst, lcd->w, lcd->h, special->format, addr);
    bitmap_set_line_length(&dst, pitch);
    bitmap_init(&src, lcd->w, lcd->h, special->format, special->lcd_mem->offline_fb);
    image_copy(&dst, &src, dr, dr->x, dr->y);
    SDL_UnlockTexture(info->texture);
    log_debug("dirty_rect: %d %d %d %d\n", dr->x, dr->y, dr->w, dr->h);

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
