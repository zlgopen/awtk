/**
 * File:   waitable_ring_buffer.h
 * Author: AWTK Develop Team
 * Brief:  waitable_ring_buffer
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-04-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WAITABLE_RING_BUFFER_H
#define TK_WAITABLE_RING_BUFFER_H

#include "tkc/mutex.h"
#include "tkc/semaphore.h"
#include "tkc/ring_buffer.h"

BEGIN_C_DECLS

/**
 * @class waitable_ring_buffer_t
 * waitable ring buffer
 */
typedef struct _waitable_ring_buffer_t {
  /*private*/
  uint32_t block_size;
  ring_buffer_t* ring_buffer;

  tk_mutex_t* mutex;
  tk_semaphore_t* sema_read;
  tk_semaphore_t* sema_write;
} waitable_ring_buffer_t;

/**
 * @method waitable_ring_buffer_create
 * @annotation ["constructor"]
 * 创建waitable_ring_buffer对象。
 *
 * @param {uint32_t} capacity 容量。
 * @param {uint32_t} block_size 块的大小。 
 *
 * @return {waitable_ring_buffer_t*} waitable_ring_buffer对象。
 */
waitable_ring_buffer_t* waitable_ring_buffer_create(uint32_t capacity, uint32_t block_size);

/**
 * @method waitable_ring_buffer_read
 * 读取数据。
 * 
 * @param {waitable_ring_buffer_t*} rb waitable_ring_buffer对象。
 * @param {void*} buff 接收数据的buff。 
 * @param {uint32_t} size 读取数据的长度(必须等于 block_size)。
 * @param {uint32_t} timeout_ms 超时时间(ms)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_ring_buffer_read(waitable_ring_buffer_t* rb, void* buff, uint32_t size,
                                uint32_t timeout_ms);

/**
 * @method waitable_ring_buffer_write
 * 写入数据。
 *
 * @param {waitable_ring_buffer_t*} rb waitable_ring_buffer对象。
 * @param {const void*} buff 要写入的数据。 
 * @param {uint32_t} size 数据的长度(必须等于 block_size)。
 * @param {uint32_t} timeout_ms 超时时间(ms)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_ring_buffer_write(waitable_ring_buffer_t* rb, const void* buff, uint32_t size,
                                 uint32_t timeout_ms);

/**
 * @method waitable_ring_buffer_destroy
 * 销毁。
 *
 * @param {waitable_ring_buffer_t*} rb waitable_ring_buffer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_ring_buffer_destroy(waitable_ring_buffer_t* rb);

END_C_DECLS

#endif /*TK_WAITABLE_RING_BUFFER_H*/
