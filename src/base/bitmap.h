/**
 * File:   bitmap.h
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/color.h"
#include "base/types_def.h"

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
   * @const BITMAP_FMT_RGBA8888
   * 一个像素占用4个字节，RGBA占一个字节，按内存地址递增。
   */
  BITMAP_FMT_RGBA8888,
  /**
   * @const BITMAP_FMT_ABGR8888
   * 一个像素占用4个字节，ABGR占一个字节，按内存地址递增。
   */
  BITMAP_FMT_ABGR8888,
  /**
   * @const BITMAP_FMT_BGRA8888
   * 一个像素占用4个字节，BGRA占一个字节，按内存地址递增。
   */
  BITMAP_FMT_BGRA8888,
  /**
   * @const BITMAP_FMT_ARGB8888
   * 一个像素占用4个字节，ARGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_ARGB8888,
  /**
   * @const BITMAP_FMT_RGB565
   * 一个像素占用2个字节，RGB分别占用5,6,5位, 按内存地址递增。
   */
  BITMAP_FMT_RGB565,
  /**
   * @const BITMAP_FMT_BGR565
   * 一个像素占用2个字节，BGR分别占用5,6,5位, 按内存地址递增。
   */
  BITMAP_FMT_BGR565,
  /**
   * @const BITMAP_FMT_RGB888
   * 一个像素占用3个字节，RGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_RGB888,
  /**
   * @const BITMAP_FMT_BGR888
   * 一个像素占用3个字节，RGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_BGR888
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
   * @property {uint32_t} line_length
   * @annotation ["readable", "scriptable"]
   * 每一行实际占用的内存(也称为stride或pitch)，一般情况下为w*bpp。
   */
  uint32_t line_length;
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

  bool_t is_gif;
  /*for gif begin*/
  int* gif_delays;
  uint32_t gif_frame_h;
  uint32_t gif_frames_nr;
  /*for gif end*/

  /*private members*/
  /*data是按cache line对齐，而data_free_ptr是用于释放的指针*/
  uint8_t* data_free_ptr;

  /*destroy时是否需要释放data指向的内存*/
  bool_t should_free_data;

  /*destroy时是否需要释放bitmap本身的内存*/
  bool_t should_free_handle;

  /* 显示特定的数据，如OpenGL texture ID，agg/agge中图片等。*/
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
 * @annotation ["constructor", "scriptable", "gc"]
 * @return {bitmap_t*} 返回bitmap对象。
 */
bitmap_t* bitmap_create(void);

/**
 * @method bitmap_create_ex
 * 创建图片对象。
 * @annotation ["constructor", "scriptable", "gc"]
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 * @param {uint32_t} line_length line_length。
 * @param {bitmap_format_t} format 格式。
 *
 * @return {bitmap_t*} 返回bitmap对象。
 */
bitmap_t* bitmap_create_ex(uint32_t w, uint32_t h, uint32_t line_length, bitmap_format_t format);

/**
 * @method bitmap_get_bpp
 * 获取图片一个像素占用的字节数。
 * @annotation ["scriptable"]
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {uint32_t} 返回一个像素占用的字节数。
 */
uint32_t bitmap_get_bpp(bitmap_t* bitmap);

/**
 * @method bitmap_get_pixel
 * 获取图片指定像素的rgba颜色值(主要用于测试程序)。
 * @param {bitmap_t*} bitmap bitmap对象。
 * @param {uint32_t} x x坐标。
 * @param {uint32_t} y y坐标。
 * @param {rgba_t*} rgba 返回颜色值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_get_pixel(bitmap_t* bitmap, uint32_t x, uint32_t y, rgba_t* rgba);

/**
 * @method bitmap_set_line_length
 * 设置line_length。
 * @param {bitmap_t*} bitmap bitmap对象。
 * @param {uint32_t} line_length line_length。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_set_line_length(bitmap_t* bitmap, uint32_t line_length);

/**
 * @method bitmap_get_line_length
 * 获取每一行占用内存的字节数。
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {ret_t} 返回每一行占用内存的字节数。
 */
uint32_t bitmap_get_line_length(bitmap_t* bitmap);

/**
 * @method bitmap_init_from_rgba
 * 初始化图片。
 * @param {bitmap_t*} bitmap bitmap对象。
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 * @param {bitmap_format_t} format 格式。
 * @param {const uint8_t*} data
 * 数据。3通道时为RGB888格式，4通道时为RGBA888格式(内部拷贝该数据，不会引用，调用者自行释放)。
 * @param {uint32_t} comp 颜色通道数(目前支持3(rgb)和4(rgba))。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_init_from_rgba(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format,
                            const uint8_t* data, uint32_t comp);

/**
 * @method bitmap_init
 * 初始化图片。
 * @param {bitmap_t*} bitmap bitmap对象。
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 * @param {bitmap_format_t} format 格式。
 * @param {const uint8_t*} data 数据，直接引用，但不负责释放。如果为空，由内部自动分配和释放。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_init(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format, uint8_t* data);

/**
 * @method bitmap_destroy
 * 销毁图片。
 * @annotation ["deconstructor", "scriptable", "gc"]
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
   * @const IMAGE_DRAW_SCALE_DOWN
   * 如果图片比目标矩形大，自动缩小显示，否则居中显示。
   */
  IMAGE_DRAW_SCALE_DOWN,

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

#ifndef BITMAP_ALIGN_SIZE
#define BITMAP_ALIGN_SIZE 32
#endif /*BITMAP_ALIGN_SIZE*/

/*private*/
ret_t bitmap_alloc_data(bitmap_t* bitmap);
uint32_t bitmap_get_bpp_of_format(bitmap_format_t format);
bool_t rgba_data_is_opaque(const uint8_t* data, uint32_t w, uint32_t h, uint8_t comp);

#if defined(WITH_SDL) || defined(LINUX)
/*for helping debug drawing bugs*/
bool_t bitmap_save_png(bitmap_t* bitmap, const char* filename);
#endif /*defined(WITH_SDL) || defined(LINUX)*/

END_C_DECLS

#endif /*TK_BITMAP_H*/
