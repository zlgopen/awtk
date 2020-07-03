/**
 * File:   graphic_buffer.h
 * Author: AWTK Develop Team
 * Brief:  graphic_buffer
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-10-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GRAPHIC_BUFFER_H
#define TK_GRAPHIC_BUFFER_H

#include "base/types_def.h"

BEGIN_C_DECLS

struct _graphic_buffer_t;
typedef struct _graphic_buffer_t graphic_buffer_t;

typedef uint8_t* (*graphic_buffer_lock_for_read_t)(graphic_buffer_t* buffer);
typedef uint8_t* (*graphic_buffer_lock_for_write_t)(graphic_buffer_t* buffer);
typedef ret_t (*graphic_buffer_unlock_t)(graphic_buffer_t* buffer);
typedef ret_t (*graphic_buffer_attach_t)(graphic_buffer_t* buffer, void* data, uint32_t w,
                                         uint32_t h);
typedef bool_t (*graphic_buffer_is_valid_for_t)(graphic_buffer_t* buffer, bitmap_t* bitmap);
typedef ret_t (*graphic_buffer_destroy_t)(graphic_buffer_t* buffer);

typedef enum _graphic_buffer_options_t {
  GRAPHIC_BUFFER_SW_READ_NEVER = 1,
  GRAPHIC_BUFFER_SW_READ_RARELY = (1 << 1),
  GRAPHIC_BUFFER_SW_READ_OFTEN = (1 << 2),
  GRAPHIC_BUFFER_SW_READ_MASK = (1 << 3),
  GRAPHIC_BUFFER_SW_WRITE_NEVER = (1 << 4),
  GRAPHIC_BUFFER_SW_WRITE_RARELY = (1 << 5),
  GRAPHIC_BUFFER_SW_WRITE_OFTEN = (1 << 6),
  GRAPHIC_BUFFER_SW_WRITE_MASK = (1 << 7),
  GRAPHIC_BUFFER_SOFTWARE_MASK = (1 << 8),
  GRAPHIC_BUFFER_PROTECTED = (1 << 9),
  GRAPHIC_BUFFER_HW_TEXTURE = (1 << 10),
  GRAPHIC_BUFFER_HW_RENDER = (1 << 11),
  GRAPHIC_BUFFER_HW_2D = (1 << 12),
  GRAPHIC_BUFFER_HW_COMPOSER = (1 << 13),
  GRAPHIC_BUFFER_HW_VIDEO_ENCODER = (1 << 14),
  GRAPHIC_BUFFER_HW_MASK = (1 << 15)
} graphic_buffer_options_t;

typedef struct _graphic_buffer_vtable_t {
  graphic_buffer_lock_for_read_t lock_for_read;
  graphic_buffer_lock_for_write_t lock_for_write;
  graphic_buffer_unlock_t unlock;
  graphic_buffer_attach_t attach;
  graphic_buffer_is_valid_for_t is_valid_for;
  graphic_buffer_destroy_t destroy;
} graphic_buffer_vtable_t;

/**
 * @class graphic_buffer_t
 * graphic_buffer。
 */
struct _graphic_buffer_t {
  const graphic_buffer_vtable_t* vt;
};

/**
 * @method graphic_buffer_create_for_bitmap
 * 为位图创建缓冲区。
 * @param {bitmap_t*} bitmap 位图对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t graphic_buffer_create_for_bitmap(bitmap_t* bitmap);

/**
 * @method graphic_buffer_create_with_data
 * 创建缓冲区。
 *
 * > 为了兼容raw图像。
 * @param {const uint8_t*} data 数据。
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 宽度。
 * @param {bitmap_format_t} format 格式。
 *
 * @return {graphic_buffer_t*} 返回缓存区。
 */
graphic_buffer_t* graphic_buffer_create_with_data(const uint8_t* data, uint32_t w, uint32_t h,
                                                  bitmap_format_t format);

/**
 * @method graphic_buffer_lock_for_read
 * 为读取数据而锁定缓冲区。
 * @param {graphic_buffer_t*} buffer 图像缓冲区对象。
 *
 * @return {uint8_t*} 返回缓存区的首地址。
 */
uint8_t* graphic_buffer_lock_for_read(graphic_buffer_t* buffer);

/**
 * @method graphic_buffer_lock_for_write
 * 为修改数据而锁定缓冲区。
 * @param {graphic_buffer_t*} buffer 图像缓冲区对象。
 *
 * @return {uint8_t*} 返回缓存区的首地址。
 */
uint8_t* graphic_buffer_lock_for_write(graphic_buffer_t* buffer);

/**
 * @method graphic_buffer_unlock
 * 解锁缓冲区。
 * @param {graphic_buffer_t*} buffer 图像缓冲区对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t graphic_buffer_unlock(graphic_buffer_t* buffer);

/**
 * @method graphic_buffer_attach
 * 附件到指定的内存。
 * @param {graphic_buffer_t*} buffer 图像缓冲区对象。
 * @param {void*} data 内存数据。
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t graphic_buffer_attach(graphic_buffer_t* buffer, void* data, uint32_t w, uint32_t h);

/**
 * @method graphic_buffer_is_valid_for
 * 用于检查graphic buffer的有效性。
 * 
 * @param {graphic_buffer_t*} buffer 图像缓冲区对象。
 * @param {bitmap_t*} bitmap 位图对象。
 *
 * @return {bool_t} 返回TRUE表示有效，否则表示无效。
 */
bool_t graphic_buffer_is_valid_for(graphic_buffer_t* buffer, bitmap_t* bitmap);

/**
 * @method graphic_buffer_destroy
 * 销毁缓冲区。
 * @param {graphic_buffer_t*} buffer 图像缓冲区对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t graphic_buffer_destroy(graphic_buffer_t* buffer);

#define GRAPHIC_BUFFER(buffer) ((graphic_buffer_t*)(buffer))

#define GRAPHIC_BUFFER_CREATE_FOR_BITMAP(bitmap) graphic_buffer_create_for_bitmap(bitmap)
#define GRAPHIC_BUFFER_CREATE_WITH_DATA(data, w, h, format) \
  graphic_buffer_create_with_data(data, w, h, format)

END_C_DECLS

#endif /*TK_GRAPHIC_BUFFER_H*/
