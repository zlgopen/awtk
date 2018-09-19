/**
 * File:   bitmap.h
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
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
 * @annotation ["scriptable"]
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
   * 一个像素占用4个字节，RGBA占一个字节，按内存地址递增。
   */
  BITMAP_FMT_RGBA,
  /**
   * @const BITMAP_FMT_BGRA
   * 一个像素占用4个字节，BGRA占一个字节，按内存地址递增。
   */
  BITMAP_FMT_BGRA,
  /**
   * @const BITMAP_FMT_RGB565
   * 一个像素占用2个字节，RGB分别占用5,6,5位。
   */
  BITMAP_FMT_RGB565,
  /**
   * @const BITMAP_FMT_RGB
   * 一个像素占用3个字节，RGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_RGB,
} bitmap_format_t;

/**
 * @enum bitmap_flag_t
 * @annotation ["scriptable"]
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
  BITMAP_FLAG_TEXTURE = 4,
  /**
   * @const BITMAP_FLAG_CHANGED
   * 如果是MUTABLE的图片，更新时需要设置此标志，底层可能会做特殊处理，比如更新图片到GPU。
   */
  BITMAP_FLAG_CHANGED = 8
} bitmap_flag_t;

/**
 * @class bitmap_t
 * @annotation ["scriptable"]
 * 位图。
 */
struct _bitmap_t {
  /**
   * @property {wh_t} w
   * @annotation ["readable", "scriptable"]
   * 宽度。
   */
  wh_t w;
  /**
   * @property {wh_t} h
   * @annotation ["readable", "scriptable"]
   * 高度。
   */
  wh_t h;
  /**
   * @property {uint16_t} flags
   * @annotation ["readable", "scriptable"]
   * 标志。请参考{bitmap_flag_t}。
   */
  uint16_t flags;
  /**
   * @property {uint16_t} format
   * @annotation ["readable", "scriptable"]
   * 格式。请参考{bitmap_format_t}。
   */
  uint16_t format;
  /**
   * @property {char*} name
   * @annotation ["readable", "scriptable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * 图片数据。
   */
  const uint8_t* data;

  /*private members*/

  bool_t should_free_handle;
  /* 显示特定的数据，如OpenGL texture ID，picasso/agg中图片等。*/
  void* specific;
  /*specific_destroy的上下文*/
  void* specific_ctx;
  /*用于销毁specific*/
  bitmap_destroy_t specific_destroy;

  /*virtual functions*/
  bitmap_destroy_t destroy;
};

/**
 * @method bitmap_create
 * 创建图片对象(一般供脚本语言中使用)。
 * @annotation ["constructor", "scriptable"]
 * @return {bitmap_t*} 返回bitmap对象。
 */
bitmap_t* bitmap_create(void);

/**
 * @method bitmap_destroy
 * 销毁图片。
 * @annotation ["deconstructor", "scriptable"]
 * @param {bitmap_t*} bitmap bitmap对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_destroy(bitmap_t* bitmap);

/**
 * @enum image_draw_type_t
 * @prefix IMAGE_DRAW_
 * @annotation ["scriptable"]
 * 图片绘制方法常量定义。
 */
typedef enum _image_draw_type_t {
  /**
   * @const IMAGE_DRAW_DEFAULT
   * 缺省显示。将图片按原大小显示在目标矩形的左上角。
   */
  IMAGE_DRAW_DEFAULT = 0,

  /**
   * @const IMAGE_DRAW_CENTER
   * 居中显示。将图片按原大小显示在目标矩形的中央。
   */
  IMAGE_DRAW_CENTER,

  /**
   * @const IMAGE_DRAW_ICON
   * 图标显示。同居中显示，但会根据屏幕密度调整大小。
   */
  IMAGE_DRAW_ICON,

  /**
   * @const IMAGE_DRAW_SCALE
   * 缩放显示。将图片缩放至目标矩形的大小(不保证宽高成比例)。
   */
  IMAGE_DRAW_SCALE,

  /**
   * @const IMAGE_DRAW_SCALE_AUTO
   * 自动缩放显示。将图片缩放至目标矩形的宽度或高度(选取最小的比例)，并居中显示。
   */
  IMAGE_DRAW_SCALE_AUTO,

  /**
   * @const IMAGE_DRAW_SCALE_W
   * 宽度缩放显示。 将图片缩放至目标矩形的宽度，高度按此比例进行缩放，超出不部分不显示。
   */
  IMAGE_DRAW_SCALE_W,

  /**
   * @const IMAGE_DRAW_SCALE_H
   * 高度缩放显示。将图片缩放至目标矩形的高度，宽度按此比例进行缩放，超出不部分不显示。
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
   * @const IMAGE_DRAW_PATCH9
   * 9宫格显示。
   * 将图片分成等大小的9块，4个角按原大小显示在目标矩形的4个角，左右上下和中间5块分别缩放显示在对应的目标区域。
   */
  IMAGE_DRAW_PATCH9,

  /**
   * @const IMAGE_DRAW_PATCH3_X
   * 水平方向3宫格显示，垂直方向居中显示。
   * 将图片在水平方向上分成等大小的3块，左右两块按原大小显示在目标矩形的左右，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_PATCH3_X,

  /**
   * @const IMAGE_DRAW_PATCH3_Y
   * 垂直方向3宫格显示，水平方向居中显示。
   * 将图片在垂直方向上分成等大小的3块，上下两块按原大小显示在目标矩形的上下，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_PATCH3_Y,

  /**
   * @const IMAGE_DRAW_PATCH3_X_SCALE_Y
   * 水平方向3宫格显示，垂直方向缩放显示。
   * 将图片在水平方向上分成等大小的3块，左右两块按原大小显示在目标矩形的左右，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_PATCH3_X_SCALE_Y,

  /**
   * @const IMAGE_DRAW_PATCH3_Y_SCALE_X
   * 垂直方向3宫格显示，水平方向缩放显示。
   * 将图片在垂直方向上分成等大小的3块，上下两块按原大小显示在目标矩形的上下，中间一块缩放显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_PATCH3_Y_SCALE_X
} image_draw_type_t;

ret_t bitmap_rgba_to_rgb565(bitmap_t* image, uint16_t* output);

ret_t bitmap_rgba_to_bgra(bitmap_t* image);

END_C_DECLS

#endif /*TK_BITMAP_H*/
