/**
 * File:   lcd_sdl2_mono.c
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
#include "lcd/lcd_mono.h"
#include "lcd/lcd_sdl2_mono.h"

typedef struct _mono_info_t {
  SDL_Renderer* render;
  SDL_Texture* texture;
} mono_info_t;

static mono_info_t* mono_info_create(SDL_Renderer* render) {
  mono_info_t* info = TKMEM_ZALLOC(mono_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->render = render;

  return info;
}

static ret_t mono_info_create_texture(mono_info_t* info, wh_t w, wh_t h) {
  int flags = SDL_TEXTUREACCESS_STREAMING;

  /*SDL ABGR is rgba from low address to high address*/
  info->texture = SDL_CreateTexture(info->render, SDL_PIXELFORMAT_ARGB8888, flags, w, h);
  log_debug("WITH_FB_BGRA8888\n");

  return info->texture != NULL ? RET_OK : RET_FAIL;
}

static ret_t mono_info_destroy(mono_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  SDL_DestroyTexture(info->texture);
  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t lcd_sdl2_mono_flush(lcd_t* lcd) {
  bitmap_t src;
  bitmap_t dst;
  int pitch = 0;
  void* addr = NULL;
  lcd_mono_t* mono = (lcd_mono_t*)(lcd);
  rect_t* dr = &(lcd->dirty_rect);
  rect_t* fps_r = &(lcd->fps_rect);
  mono_info_t* info = (mono_info_t*)(mono->ctx);

  memset(&src, 0x00, sizeof(src));
  memset(&dst, 0x00, sizeof(dst));

  if ((dr->w > 0 && dr->h > 0) || (fps_r->w > 0 && fps_r->h > 0)) {
    uint32_t j = 0;
    uint32_t i = 0;
    uint8_t* p = NULL;
    SDL_Rect sr = {0, 0, lcd->w, lcd->h};

    SDL_LockTexture(info->texture, NULL, (void**)&(addr), &pitch);
    bitmap_init(&dst, lcd->w, lcd->h, BITMAP_FMT_BGRA8888, addr);

    for (j = 0; j < lcd->h; j++) {
      for (i = 0; i < lcd->w; i++) {
        bool_t pixel = bitmap_mono_get_pixel(mono->data, lcd->w, lcd->h, i, j);
        p = ((uint8_t*)addr) + j * pitch + i * 4;
        if (pixel) {
          p[0] = 0xff;
          p[1] = 0xff;
          p[2] = 0xff;
          p[3] = 0xff;
        } else {
          p[0] = 0;
          p[1] = 0;
          p[2] = 0;
          p[3] = 0xff;
        }
      }
    }

    SDL_UnlockTexture(info->texture);

    SDL_RenderCopy(info->render, info->texture, &sr, &sr);
  }

  if (lcd->draw_mode != LCD_DRAW_OFFLINE) {
    SDL_RenderPresent(info->render);
  }

  return RET_OK;
}

static ret_t lcd_sdl2_mono_destroy(lcd_t* lcd) {
  lcd_mono_t* mono = (lcd_mono_t*)lcd;

  mono_info_destroy((mono_info_t*)(mono->ctx));
  mono->ctx = NULL;

  return RET_OK;
}

lcd_t* lcd_sdl2_mono_init(SDL_Renderer* render) {
  int w = 0;
  int h = 0;
  mono_info_t* info = NULL;
  return_value_if_fail(render != NULL, NULL);

  SDL_GetRendererOutputSize(render, &w, &h);
  info = mono_info_create(render);
  return_value_if_fail(info != NULL, NULL);

  ENSURE(mono_info_create_texture(info, w, h) == RET_OK);

  return lcd_mono_create(w, h, lcd_sdl2_mono_flush, lcd_sdl2_mono_destroy, info);
}
