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

#ifndef TK_BITMAP_H
#define TK_BITMAP_H

#include "base/color.h"

BEGIN_C_DECLS

struct _bitmap_t;
typedef struct _bitmap_t bitmap_t;

typedef ret_t (*bitmap_destroy_t)(bitmap_t* bitmap);

/**
 * @enum bitmap_format_t
 * @prefix BITMAP_FMT_
 * 位图格式常量定义。
 */
typedef enum _bitmap_format_t {
  /**
   * @const BITMAP_FMT_NONE
   * 无效格式。
   */
  BITMAP_FMT_NONE = 0,
  /**
   * @const BITMAP_FMT_RGBA
   * 一个像素占用4个字节，RGBA四个通道占一个字节。
   */
  BITMAP_FMT_RGBA,
  /**
   * @const BITMAP_FMT_RGB565
   * 一个像素占用2个字节，RGB分别占用5,6,5位。
   */
  BITMAP_FMT_RGB565
} bitmap_format_t;

/**
 * @enum _bitmap_format_t
 * @prefix BITMAP_FLAG_
 * 位图标志常量定义。
 */
typedef enum _bitmap_flag_t {
  /**
   * @const BITMAP_FLAG_NONE
   * 无特殊标志。
   */
  BITMAP_FLAG_NONE = 0,
  /**
   * @const BITMAP_FLAG_OPAQUE
   * 不透明图片。
   */
  BITMAP_FLAG_OPAQUE = 1,
  /**
   * @const BITMAP_FLAG_IMMUTABLE
   * 图片内容不会变化。
   */
  BITMAP_FLAG_IMMUTABLE = 2,

  /**
   * @const BITMAP_FLAG_TEXTURE
   * OpenGL Texture, bitmap的id是有效的texture id。
   */
  BITMAP_FLAG_TEXTURE = 4
} bitmap_flag_t;

/**
 * @class bitmap_t
 * 位图。
 */
struct _bitmap_t {
  /**
   * @property {wh_t} w
   * @readonly
   * 宽度。
   */
  wh_t w;
  /**
   * @property {wh_t} h
   * @readonly
   * 高度。
   */
  wh_t h;
  /**
   * @property {uint16_t} flags
   * @readonly
   * 标志。请参考{bitmap_flag_t}。
   */
  uint16_t flags;
  /**
   * @property {uint16_t} format
   * @readonly
   * 格式。请参考{bitmap_format_t}。
   */
  uint16_t format;
  /**
   * @property {char*} name
   * @readonly
   * 名称。
   */
  const char* name;
  /**
   * @property {uint8_t*} data
   * @readonly
   * 图片数据。
   */
  const uint8_t* data;

  /**
   * @property {void*} specfic
   * @readonly
   * 显示特定的数据，如OpenGL texture ID，picasso/agg中图片等。
   */
  void* specific;
  void* specific_ctx;
  bitmap_destroy_t specific_destroy;

  bitmap_destroy_t destroy;
};

/**
 * @method bitmap_destroy
 * 销毁图片。
 * @param {bitmap_t*} bitmap bitmap对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
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

#endif /*TK_BITMAP_H*/
