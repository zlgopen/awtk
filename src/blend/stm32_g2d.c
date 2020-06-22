/**
 * File:   stm32_g2d.h
 * Author: AWTK Develop Team
 * Brief:  stm32 dma2d implemented hardware 2d
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-07 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "base/g2d.h"

#ifdef WITH_STM32_G2D

#if defined(STM32H743xx)
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_dma2d.h"
/* Legacy defines */
#define DMA2D_IFSR_CTCIF DMA2D_IFCR_CTCIF /*!< Clears Transfer Complete Interrupt Flag      */

#elif defined(ARM_MATH_CM7)
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_dma2d.h"
#else
#include "stm32f4xx.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_dma2d.h"
#endif /*ARM_MATH_CM7*/

#define DMA2D_WORKING ((DMA2D->CR & DMA2D_CR_START))
#define DMA2D_WAIT                  \
  do {                              \
    while (DMA2D_WORKING)           \
      ;                             \
    DMA2D->IFCR = DMA2D_IFSR_CTCIF; \
  } while (0);

#define PIXEL_FORMAT_BGRA8888 0X00
#define PIXEL_FORMAT_BGR565 0X02

ret_t g2d_fill_rect(bitmap_t* fb, rect_t* dst, color_t c) {
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t w = 0;
  uint16_t h = 0;
  uint32_t color = 0;
  uint32_t o_addr = 0;
  uint16_t o_format = 0;
  uint16_t o_offline = 0;
  uint16_t o_pixsize = 0;
  uint8_t* fb_data = NULL;
  if (c.rgba.a < 0xf0) {
    return RET_NOT_IMPL;
  }

  return_value_if_fail(fb != NULL && fb->buffer != NULL && dst != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == BITMAP_FMT_BGR565 || fb->format == BITMAP_FMT_BGRA8888,
                       RET_BAD_PARAMS);

  x = dst->x;
  y = dst->y;
  w = dst->w;
  h = dst->h;

  if (fb->format == BITMAP_FMT_BGR565) {
    o_pixsize = 2;
    o_format = PIXEL_FORMAT_BGR565;
    color = ((((c.rgba.r) >> 3) << 11) | (((c.rgba.g) >> 2) << 5) | ((c.rgba.b) >> 3));
  } else {
    o_pixsize = 4;
    o_format = PIXEL_FORMAT_BGRA8888;
    color = c.color;
  }

  fb_data = bitmap_lock_buffer_for_write(fb);
  o_offline = fb->w - w;
  o_addr = ((uint32_t)fb_data + o_pixsize * (fb->w * y + x));

  __HAL_RCC_DMA2D_CLK_ENABLE();

  DMA2D->CR &= ~(DMA2D_CR_START);
  DMA2D->CR = DMA2D_R2M;

  DMA2D->OPFCCR = o_format;
  DMA2D->OOR = o_offline;
  DMA2D->OMAR = o_addr;
  DMA2D->OCOLR = color;
  DMA2D->NLR = h | (w << 16);

  DMA2D->CR |= DMA2D_CR_START;

  DMA2D_WAIT
  bitmap_unlock_buffer(fb);

  return RET_OK;
}

ret_t g2d_copy_image(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t x, xy_t y) {
  uint32_t o_addr = 0;
  uint16_t o_offline = 0;
  uint16_t o_format = 0;
  uint16_t o_pixsize = 0;
  uint32_t fg_addr = 0;
  uint16_t fg_offline = 0;
  uint16_t fg_format = 0;
  uint16_t fg_pixsize = 0;
  uint16_t sx = 0;
  uint16_t sy = 0;
  uint16_t w = 0;
  uint16_t h = 0;
  uint16_t iw = 0;
  uint8_t* fb_data = NULL;
  uint8_t* img_data = NULL;

  return_value_if_fail(fb != NULL && fb->buffer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(img != NULL && img->buffer != NULL && src != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == BITMAP_FMT_BGR565 || fb->format == BITMAP_FMT_BGRA8888,
                       RET_BAD_PARAMS);
  return_value_if_fail(img->format == BITMAP_FMT_BGR565 || img->format == BITMAP_FMT_BGRA8888,
                       RET_BAD_PARAMS);

  sx = src->x;
  sy = src->y;
  w = src->w;
  h = src->h;
  iw = img->w;

  if (fb->format == BITMAP_FMT_BGR565) {
    o_pixsize = 2;
    o_format = PIXEL_FORMAT_BGR565;
  } else {
    o_pixsize = 4;
    o_format = PIXEL_FORMAT_BGRA8888;
  }

  if (img->format == BITMAP_FMT_BGR565) {
    fg_pixsize = 2;
    fg_format = PIXEL_FORMAT_BGR565;
  } else {
    fg_pixsize = 4;
    fg_format = PIXEL_FORMAT_BGRA8888;
  }

  fb_data = bitmap_lock_buffer_for_write(fb);
  img_data = bitmap_lock_buffer_for_write(img);

  o_offline = fb->w - w;
  o_addr = ((uint32_t)fb_data + o_pixsize * (fb->w * y + x));
  fg_offline = iw - w;
  fg_addr = ((uint32_t)img_data + fg_pixsize * (img->w * sy + sx));

  __HAL_RCC_DMA2D_CLK_ENABLE();

  DMA2D->CR &= ~(DMA2D_CR_START);
  if (o_format == fg_format) {
    DMA2D->CR = DMA2D_M2M;
  } else {
    DMA2D->CR = DMA2D_M2M_PFC;
  }

  DMA2D->OPFCCR = o_format;
  DMA2D->OOR = o_offline;
  DMA2D->OMAR = o_addr;

  DMA2D->FGPFCCR = fg_format;
  DMA2D->FGOR = fg_offline;
  DMA2D->FGMAR = fg_addr;

  DMA2D->NLR = h | (w << 16);

  DMA2D->CR |= DMA2D_CR_START;
  DMA2D_WAIT
  bitmap_unlock_buffer(fb);
  bitmap_unlock_buffer(img);

  return RET_OK;
}

ret_t g2d_rotate_image(bitmap_t* fb, bitmap_t* img, rect_t* src, lcd_orientation_t o) {
  (void)fb;
  (void)img;
  (void)src;
  (void)o;

  return RET_NOT_IMPL;
}

ret_t g2d_blend_image(bitmap_t* fb, bitmap_t* img, rect_t* dst, rect_t* src, uint8_t global_alpha) {
  uint32_t o_addr = 0;
  uint16_t o_offline = 0;
  uint16_t o_format = 0;
  uint16_t o_pixsize = 0;
  uint32_t fg_addr = 0;
  uint16_t fg_offline = 0;
  uint16_t fg_format = 0;
  uint16_t fg_pixsize = 0;
  uint16_t sx = 0;
  uint16_t sy = 0;
  uint16_t w = 0;
  uint16_t h = 0;
  uint16_t iw = 0;
  uint16_t x = 0;
  uint16_t y = 0;
  uint8_t* fb_data = NULL;
  uint8_t* img_data = NULL;
  //	return RET_NOT_IMPL;

  return_value_if_fail(global_alpha > 0xf0, RET_NOT_IMPL); /*not support global_alpha*/
  return_value_if_fail(fb != NULL && fb->buffer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(img != NULL && img->buffer != NULL && src != NULL && dst != NULL,
                       RET_BAD_PARAMS);
  return_value_if_fail(fb->format == BITMAP_FMT_BGR565 || fb->format == BITMAP_FMT_BGRA8888,
                       RET_BAD_PARAMS);
  return_value_if_fail(img->format == BITMAP_FMT_BGR565 || img->format == BITMAP_FMT_BGRA8888,
                       RET_BAD_PARAMS);
  return_value_if_fail(src->w == dst->w && src->h == dst->h, RET_NOT_IMPL); /*not support scale*/

  x = dst->x;
  y = dst->y;
  sx = src->x;
  sy = src->y;
  w = src->w;
  h = src->h;
  iw = img->w;

  if (fb->format == BITMAP_FMT_BGR565) {
    o_pixsize = 2;
    o_format = PIXEL_FORMAT_BGR565;
  } else {
    o_pixsize = 4;
    o_format = PIXEL_FORMAT_BGRA8888;
  }

  if (img->format == BITMAP_FMT_BGR565) {
    fg_pixsize = 2;
    fg_format = PIXEL_FORMAT_BGR565;
  } else {
    fg_pixsize = 4;
    fg_format = PIXEL_FORMAT_BGRA8888;
  }

  fb_data = bitmap_lock_buffer_for_write(fb);
  img_data = bitmap_lock_buffer_for_write(img);
  o_offline = fb->w - w;
  o_addr = ((uint32_t)fb_data + o_pixsize * (fb->w * y + x));
  fg_offline = iw - w;
  fg_addr = ((uint32_t)img_data + fg_pixsize * (img->w * sy + sx));

  __HAL_RCC_DMA2D_CLK_ENABLE();

  DMA2D->CR &= ~(DMA2D_CR_START);
  if (o_format == fg_format) {
    DMA2D->CR = DMA2D_M2M;
  } else {
    DMA2D->CR = DMA2D_M2M_BLEND;
  }

  DMA2D->BGPFCCR = o_format;
  DMA2D->BGOR = o_offline;
  DMA2D->BGMAR = o_addr;

  DMA2D->OPFCCR = o_format;
  DMA2D->OOR = o_offline;
  DMA2D->OMAR = o_addr;

  DMA2D->FGPFCCR = fg_format;
  DMA2D->FGOR = fg_offline;
  DMA2D->FGMAR = fg_addr;

  DMA2D->NLR = h | (w << 16);

  DMA2D->CR |= DMA2D_CR_START;
  DMA2D_WAIT
  bitmap_unlock_buffer(fb);
  bitmap_unlock_buffer(img);

  return RET_OK;
}
#endif /*WITH_STM32_G2D*/
