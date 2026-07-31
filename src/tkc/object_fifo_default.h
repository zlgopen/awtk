/**
 * File:   object_fifo_default.h
 * Author: AWTK Develop Team
 * Brief:  object_fifo 的默认实现
 *
 * Copyright (c) 2024 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-22 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_FIFO_DEFAULT_H
#define TK_OBJECT_FIFO_DEFAULT_H

#include "object_fifo.h"

BEGIN_C_DECLS

struct _object_fifo_default_t;
typedef struct _object_fifo_default_t object_fifo_default_t;

/**
 * @class object_fifo_default_t
 * @parent object_fifo_t
 *
 * object_fifo的缺省实现。
 *
 */
struct _object_fifo_default_t {
  object_fifo_t base;

  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * FIFO的容量大小。
   */
  uint32_t capacity;
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * FIFO中元素的个数。
   */
  uint32_t size;
  /**
   * @property {uint32_t} cursor
   * @annotation ["readable"]
   * FIFO中最后一个元素的索引。
   */
  uint32_t cursor;
  /**
   * @property {uint32_t} unit_size
   * @annotation ["readable"]
   * FIFO中单个元素的大小。
   */
  uint32_t unit_size;
  /**
   * @property {uint8_t*} buffer
   * @annotation ["readable"]
   * FIFO中的数据缓存。
   */
  uint8_t* buffer;
};

/**
 * @method object_fifo_default_create
 * 创建object_fifo_default对象。
 *
 * @annotation ["constructor"]
 * @param {uint32_t} capacity FIFO初始容量。
 * @param {uint32_t} unit_size FIFO单个元素的大小。
 *
 * @return {object_t*} 返回object_fifo对象。
 */
object_t* object_fifo_default_create(uint32_t capacity, uint32_t unit_size);

#define OBJECT_FIFO_DEFAULT(obj) ((object_fifo_default_t*)(obj))

END_C_DECLS

#endif /*TK_OBJECT_FIFO_DEFAULT_H*/
