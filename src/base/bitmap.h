/**
 * File:   bitmap.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap interface
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_BITMAP_H
#define LFTK_BITMAP_H

#include "base/color.h"

BEGIN_C_DECLS

struct _bitmap_t;
typedef struct _bitmap_t bitmap_t;

typedef ret_t (*bitmap_destroy_t)(bitmap_t* bitmap);

enum { IMAGE_FMT_NONE = 0, IMAGE_FMT_RGBA, IMAGE_FTM_1555, IMAGE_FTM_565 };

struct _bitmap_t {
  wh_t w;
  wh_t h;
  uint8_t flag;
  uint8_t format;
  uint16_t tex_id; /*texture id为opengl保留*/
  const char* name;
  const uint8_t* data;
  bitmap_destroy_t destroy;
};

struct _image_loader_t;
typedef struct _image_loader_t image_loader_t;

typedef bool_t (*image_loader_match_t)(image_loader_t* loader, uint16_t type);
typedef ret_t (*image_loader_load_t)(image_loader_t* loader, const uint8_t* buff, uint32_t size,
                                     bitmap_t* bitmap);

struct _image_loader_t {
  image_loader_match_t match;
  image_loader_load_t load;
};

bool_t image_loader_match(image_loader_t* loader, uint16_t type);
ret_t image_loader_load(image_loader_t* loader, const uint8_t* buff, uint32_t size,
                        bitmap_t* bitmap);

ret_t bitmap_destroy(bitmap_t* bitmap);

/**
 * @enum image_draw_type_t
 * @prefix IMAGE_DRAW_
 * 图片绘制方法常量定义。
 */
typedef enum _image_draw_type_t {
  /**
   * @const IMAGE_DRAW_CENTER
   * 居中显示。
   * 将图片按原大小显示在目标矩形的中央。
   */
  IMAGE_DRAW_CENTER = 0,

  /**
   * @const IMAGE_DRAW_SCALE
   * 缩放显示。
   * 将图片缩放至目标矩形的大小(不保证宽高成比例)。
   */
  IMAGE_DRAW_SCALE,

  /**
   * @const IMAGE_DRAW_SCALE_AUTO
   * 自动缩放显示。
   * 将图片缩放至目标矩形的宽度或高度(选取最小的比例)，并居中显示。
   */
  IMAGE_DRAW_SCALE_AUTO,

  /**
   * @const IMAGE_DRAW_SCALE_W
   * 宽度缩放显示。
   * 将图片缩放至目标矩形的宽度，高度按此比例进行缩放，超出不部分不显示。
   */
  IMAGE_DRAW_SCALE_W,

  /**
   * @const IMAGE_DRAW_SCALE_H
   * 高度缩放显示。
   * 将图片缩放至目标矩形的高度，宽度按此比例进行缩放，超出不部分不显示。
   */
  IMAGE_DRAW_SCALE_H,

  /**
   * @const IMAGE_DRAW_REPEAT
   * 平铺显示。
   */
  IMAGE_DRAW_REPEAT,

  /**
   * @const IMAGE_DRAW_REPEAT_X
   * 水平方向平铺显示，垂直方向缩放。
   */
  IMAGE_DRAW_REPEAT_X,
  /**
   * @const IMAGE_DRAW_REPEAT_Y
   * 垂直方向平铺显示，水平方向缩放。
   */
  IMAGE_DRAW_REPEAT_Y,

  /**
   * @const IMAGE_DRAW_9PATCH
   * 9宫格显示。
   * 将图片分成等大小的9块，4个角按原大小显示在目标矩形的4个角，左右上下和中间5块分别缩放显示在对应的目标区域。
   */
  IMAGE_DRAW_9PATCH,

  /**
   * @const IMAGE_DRAW_3PATCH_X
   * 水平方向3宫格显示，垂直方向居中显示。
   * 将图片在水平方向上分成等大小的3块，左右两块按原大小显示在目标矩形的左右，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_3PATCH_X,

  /**
   * @const IMAGE_DRAW_3PATCH_Y
   * 垂直方向3宫格显示，水平方向居中显示。
   * 将图片在垂直方向上分成等大小的3块，上下两块按原大小显示在目标矩形的上下，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_3PATCH_Y,

  /**
   * @const IMAGE_DRAW_3PATCH_X_SCALE_Y
   * 水平方向3宫格显示，垂直方向缩放显示。
   * 将图片在水平方向上分成等大小的3块，左右两块按原大小显示在目标矩形的左右，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_3PATCH_X_SCALE_Y,

  /**
   * @const IMAGE_DRAW_3PATCH_Y_SCALE_X
   * 垂直方向3宫格显示，水平方向缩放显示。
   * 将图片在垂直方向上分成等大小的3块，上下两块按原大小显示在目标矩形的上下，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_3PATCH_Y_SCALE_X
} image_draw_type_t;

END_C_DECLS

#endif /*LFTK_BITMAP_H*/
