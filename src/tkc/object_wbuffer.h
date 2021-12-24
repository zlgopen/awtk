/**
 * File:   object_wbuffer.h
 * Author: AWTK Develop Team
 * Brief:  wrap wbuffer to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_WBUFFER_H
#define TK_OBJECT_WBUFFER_H

#include "tkc/object.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

/**
 * @class object_wbuffer_t
 * @parent tk_object_t
 *
 * 将wbuffer包装成object。
 *
 */
typedef struct _object_wbuffer_t {
  tk_object_t object;

  /**
   * @property {wbuffer_t*} wbuffer
   * @annotation ["readable"]
   * wbuffer对象。
   *
   */
  wbuffer_t* wbuffer;

  /*private*/
  wbuffer_t awbuffer;
} object_wbuffer_t;

/**
 * @method object_wbuffer_create
 *
 * 创建wbuffer对象。
 *
 * @annotation ["constructor"]
 * @param {uint8_t*} data 缓冲区。
 * @param {uint16_t} capacity 缓冲区的容量。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_wbuffer_create(uint8_t* data, uint32_t capacity);

/**
 * @method object_wbuffer_create_extendable
 *
 * 创建可扩展的wbuffer对象。
 *
 * @annotation ["constructor"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_wbuffer_create_extendable(void);

/**
 * @method object_wbuffer_cast
 * 转换为object_wbuffer对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_wbuffer对象。
 *
 * @return {object_wbuffer_t*} object_wbuffer对象。
 */
object_wbuffer_t* object_wbuffer_cast(tk_object_t* obj);
#define OBJECT_WBUFFER(obj) object_wbuffer_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_WBUFFER_H*/
