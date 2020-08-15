/**
 * File:   bitmap.h
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
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

#ifndef TK_BITMAP_H
#define TK_BITMAP_H

#include "tkc/color.h"
#include "base/types_def.h"
#include "base/graphic_buffer.h"

BEGIN_C_DECLS

typedef ret_t (*bitmap_destroy_t)(bitmap_t* bitmap);

/**
 * @class bitmap_t
 * @order -9
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
   * @property {const char*} name
   * @annotation ["readable", "scriptable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {graphic_buffer_t*} buffer
   * @annotation ["readable"]
   * 图片数据。
   */
  graphic_buffer_t* buffer;

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

  image_manager_t* image_manager;
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
 * @method bitmap_lock_buffer_for_read
 * 为读取数据而锁定bitmap的图片缓冲区。
 *
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {uint8_t*} 返回缓存区的首地址。
 */
uint8_t* bitmap_lock_buffer_for_read(bitmap_t* bitmap);

/**
 * @method bitmap_lock_buffer_for_write
 * 为修改数据而锁定bitmap的图片缓冲区。
 *
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {uint8_t*} 返回缓存区的首地址。
 */
uint8_t* bitmap_lock_buffer_for_write(bitmap_t* bitmap);

/**
 * @method bitmap_unlock_buffer
 * 解锁图像缓冲区。
 *
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_unlock_buffer(bitmap_t* bitmap);

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

#if defined(WITH_STB_IMAGE) || defined(WITH_FS_RES)
/*for helping debug drawing bugs*/

/**
 * @method bitmap_save_png
 * 把bitmap保存为png。
 *
 * @param {bitmap_t*} bitmap bitmap对象。
 * @param {const char*} filename 文件名。
 *
 * @return {bool_t} 返回TRUE表示成功，FALSE表示失败。
 */
bool_t bitmap_save_png(bitmap_t* bitmap, const char* filename);

#endif /*defined(WITH_STB_IMAGE) || defined(WITH_FS_RES)*/

/**
 * @method bitmap_mono_dump
 * dump mono bitmap。
 * @annotation ["static"]
 * @param {const uint8_t*} buff 数据。
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_mono_dump(const uint8_t* buff, uint32_t w, uint32_t h);

/**
 * @method bitmap_destroy
 * 销毁图片。
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {bitmap_t*} bitmap bitmap对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bitmap_destroy(bitmap_t* bitmap);

/**
 * @method bitmap_get_bpp_of_format
 * 获取位图格式对应的颜色位数。
 * @annotation ["scriptable", "static"]
 * 
 * @param {bitmap_format_t} format 位图格式。
 * 
 * @return {uint32_t} 成功返回颜色位数，失败返回0。
 */
uint32_t bitmap_get_bpp_of_format(bitmap_format_t format);

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
   * @const IMAGE_DRAW_REPEAT_Y_INVERSE
   * 垂直方向平铺显示，水平方向缩放(从底部到顶部)。
   */
  IMAGE_DRAW_REPEAT_Y_INVERSE,

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
  IMAGE_DRAW_PATCH3_Y_SCALE_X,

  /**
   * @const IMAGE_DRAW_REPEAT9
   * 平铺9宫格显示。
   * 将图片分成4个角和5块平铺块，4个角按原大小显示在目标矩形的4个角，其余5块会平铺对应的目标区域。
   * 切割方法为（如下图）：
   * 如果图片宽度为奇数，则中间一块为一列数据，如果图片宽度为偶数，则中间一块为二列数据，其他数据分为左右块
   * 如果图片高度为奇数，则中间一块为一行数据，如果图片高度为偶数，则中间一块为二行数据，其他数据分为上下块
   * 中间一块数据根据上面两条规则组成4中情况，分别是一列一行数据，一列两行数据，两列一行数据和两行两列数据
   */
  IMAGE_DRAW_REPEAT9,

  /**
   * @const IMAGE_DRAW_REPEAT3_X
   * 水平方向3宫格显示，垂直方向居中显示。
   * 将图片在水平方向上分成左右相等两块和中间一块，如果图片宽度为奇数，则中间一块为一列数据，如果图片宽度为偶数，则中间一块为二列数据，其他数据分为左右块。
   * 左右两块按原大小显示在目标矩形的左右，中间一列像素点平铺显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_REPEAT3_X,

  /**
   * @const IMAGE_DRAW_REPEAT3_Y
   * 垂直方向3宫格显示，水平方向居中显示。
   * 将图片在垂直方向上分成上下相等两块和中间一块，如果图片高度为奇数，则中间一块为一行数据，如果图片高度为偶数，则中间一块为二行数据，其他数据分为上下块
   * 上下两块按原大小显示在目标矩形的上下，中间一块平铺显示在目标区域中间剩余部分。
   */
  IMAGE_DRAW_REPEAT3_Y
} image_draw_type_t;

/*private*/
ret_t bitmap_alloc_data(bitmap_t* bitmap);
bool_t rgba_data_is_opaque(const uint8_t* data, uint32_t w, uint32_t h, uint8_t comp);

bitmap_t* bitmap_clone(bitmap_t* bitmap);
ret_t bitmap_premulti_alpha(bitmap_t* bitmap);

#define TK_BITMAP_MONO_LINE_LENGTH(w) (((w + 15) >> 4) << 1)

uint8_t* bitmap_mono_create_data(uint32_t w, uint32_t h);
bool_t bitmap_mono_get_pixel(const uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y);
ret_t bitmap_mono_set_pixel(uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y,
                            bool_t pixel);
END_C_DECLS

#endif /*TK_BITMAP_H*/
