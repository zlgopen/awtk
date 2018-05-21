/**
 * File:   stm32_g2d.h
 * Author: AWTK Develop Team
 * Brief:  stm32 dma2d implemented hardware 2d
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef WITH_G2D

#include "base/g2d.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_dma2d.h"

#define DMA2D_WORKING ((DMA2D->CR & DMA2D_CR_START))
#define DMA2D_WAIT                  \
  do {                              \
    while (DMA2D_WORKING)           \
      ;                             \
    DMA2D->IFCR = DMA2D_IFSR_CTCIF; \
  } while (0);

#define PIXEL_FORMAT_ARGB8888 0X00
#define PIXEL_FORMAT_RGB565 0X02

ret_t g2d_fill_rect(bitmap_t* fb, xy_t x, xy_t y, wh_t w, wh_t h, color_t c) {
  uint32_t color = 0;
  uint32_t o_addr = 0;
  uint16_t o_format = 0;
  uint16_t o_offline = 0;
  uint16_t o_pixsize = 0;

  return_value_if_fail(fb != NULL && fb->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == BITMAP_FMT_RGB565 || fb->format == BITMAP_FMT_BGRA,
                       RET_BAD_PARAMS);

  if (fb->format == BITMAP_FMT_RGB565) {
    o_pixsize = 2;
    o_format = PIXEL_FORMAT_RGB565;
    color = ((((c.rgba.r) >> 3) << 11) | (((c.rgba.g) >> 2) << 5) | ((c.rgba.b) >> 3));
  } else {
    o_pixsize = 4;
    o_format = PIXEL_FORMAT_ARGB8888;
    color = c.color;
  }

  o_offline = fb->w - w;
  o_addr = ((uint32_t)fb->data + o_pixsize * (fb->w * y + x));

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

  return_value_if_fail(fb != NULL && fb->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(img != NULL && img->data != NULL && src != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == BITMAP_FMT_RGB565 || fb->format == BITMAP_FMT_BGRA,
                       RET_BAD_PARAMS);
  return_value_if_fail(img->format == BITMAP_FMT_RGB565 || img->format == BITMAP_FMT_BGRA,
                       RET_BAD_PARAMS);

  sx = src->x;
  sy = src->y;
  w = src->w;
  h = src->h;
  iw = img->w;

  if (fb->format == BITMAP_FMT_RGB565) {
    o_pixsize = 2;
    o_format = PIXEL_FORMAT_RGB565;
  } else {
    o_pixsize = 4;
    o_format = PIXEL_FORMAT_ARGB8888;
  }

  if (img->format == BITMAP_FMT_RGB565) {
    fg_pixsize = 2;
    fg_format = PIXEL_FORMAT_RGB565;
  } else {
    fg_pixsize = 4;
    fg_format = PIXEL_FORMAT_ARGB8888;
  }

  o_offline = fb->w - w;
  o_addr = ((uint32_t)fb->data + o_pixsize * (fb->w * y + x));
  fg_offline = iw - w;
  fg_addr = ((uint32_t)img->data + fg_pixsize * (img->w * sy + sx));

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

  return RET_OK;
}

ret_t g2d_blend_image(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t x, xy_t y) {
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

  return_value_if_fail(fb != NULL && fb->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(img != NULL && img->data != NULL && src != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == BITMAP_FMT_RGB565 || fb->format == BITMAP_FMT_BGRA,
                       RET_BAD_PARAMS);
  return_value_if_fail(img->format == BITMAP_FMT_RGB565 || img->format == BITMAP_FMT_BGRA,
                       RET_BAD_PARAMS);

  sx = src->x;
  sy = src->y;
  w = src->w;
  h = src->h;
  iw = img->w;

  if (fb->format == BITMAP_FMT_RGB565) {
    o_pixsize = 2;
    o_format = PIXEL_FORMAT_RGB565;
  } else {
    o_pixsize = 4;
    o_format = PIXEL_FORMAT_ARGB8888;
  }

  if (img->format == BITMAP_FMT_RGB565) {
    fg_pixsize = 2;
    fg_format = PIXEL_FORMAT_RGB565;
  } else {
    fg_pixsize = 4;
    fg_format = PIXEL_FORMAT_ARGB8888;
  }

  o_offline = fb->w - w;
  o_addr = ((uint32_t)fb->data + o_pixsize * (fb->w * y + x));
  fg_offline = iw - w;
  fg_addr = ((uint32_t)img->data + fg_pixsize * (img->w * sy + sx));

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

  return RET_OK;
}
#endif /*WITH_G2D*/
