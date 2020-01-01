/**
 * File:   ostream_retry.h
 * Author: AWTK Develop Team
 * Brief:  retry ostream: if write fail retry write later.
 *
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
 * 2019-10-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OSTREAM_RETRY_H
#define TK_OSTREAM_RETRY_H

#include "tkc/buffer.h"
#include "tkc/ostream.h"
#include "tkc/ring_buffer.h"

BEGIN_C_DECLS

struct _tk_ostream_retry_t;
typedef struct _tk_ostream_retry_t tk_ostream_retry_t;

/**
 * @enum data_discard_policy_t
 * @prefix DATA_DISCARD_
 *
 * 缓存不够时丢弃数据的策略。
 */
typedef enum _data_discard_policy_t {
  /**
   * @const DATA_DISCARD_OLD
   * 丢弃旧的数据。
   */
  DATA_DISCARD_OLD = 0,
  /**
   * @const DATA_DISCARD_NEW
   * 丢弃新的数据。
   */
  DATA_DISCARD_NEW
} data_discard_policy_t;

/**
 * @class tk_ostream_retry_t
 * @parent tk_ostream_t
 *
 * 如果写数据失败，将数据放入ring_buffer，后面再重试(flush)。
 *
 * 如果重试次数超出最大值，则丢掉数据。
 *
 */
struct _tk_ostream_retry_t {
  tk_ostream_t ostream;

  /**
   * @property {uint32_t} pending_bytes
   * @annotation ["readable"]
   * 待重写的数据大小。
   */
  uint32_t pending_bytes;
  /**
   * @property {uint32_t} pending_packets
   * @annotation ["readable"]
   * 待重写的数据包的个数。
   */
  uint32_t pending_packets;
  /**
   * @property {uint32_t} discard_bytes
   * @annotation ["readable"]
   * 总共丢弃数据的字节数。
   */
  uint32_t discard_bytes;
  /**
   * @property {uint32_t} discard_packets
   * @annotation ["readable"]
   * 总共丢弃数据包的个数。
   */
  uint32_t discard_packets;
  /**
   * @property {uint32_t} max_retry_times
   * @annotation ["readable"]
   * 尝试次数超过指定的值时丢弃该数据包。
   */
  uint32_t max_retry_times;
  /**
   * @property {uint32_t} timeout
   * @annotation ["readable"]
   * 写超时时间(ms)。
   */
  uint32_t timeout;
  /**
   * @property {data_discard_policy_t} discard_policy
   * @annotation ["readable"]
   * 缓存不够时，丢弃数据包的策略。
   */
  data_discard_policy_t discard_policy;

  /*private*/
  wbuffer_t wb;
  ring_buffer_t* rb;
  uint32_t retried_times;
  tk_ostream_t* real_ostream;
};

/**
 * @method tk_ostream_retry_create
 *
 * 创建ostream对象。
 *
 * 本函数自动增加real_ostream的引用计数。
 *
 * @param {tk_ostream_t*} real_ostream 实际的ostream对象。
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_retry_create(tk_ostream_t* real_ostream);

/**
 * @method tk_ostream_retry_set_max_retry_times
 *
 * 设置最大尝试次数。
 *
 * @param {tk_ostream_t*} ostream ostream对象。
 * @param {uint32_t} max_retry_times 最大尝试次数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_retry_set_max_retry_times(tk_ostream_t* ostream, uint32_t max_retry_times);

/**
 * @method tk_ostream_retry_set_max_buffer_size
 *
 * 设置最大缓存大小。
 *
 * @param {tk_ostream_t*} ostream ostream对象。
 * @param {uint32_t} max_buffer_size 缓存大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_retry_set_max_buffer_size(tk_ostream_t* ostream, uint32_t max_buffer_size);

/**
 * @method tk_ostream_retry_clear_buffer
 *
 * 清除缓存的数据。
 *
 * @param {tk_ostream_t*} ostream ostream对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_retry_clear_buffer(tk_ostream_t* ostream);

/**
 * @method tk_ostream_retry_set_discard_policy
 *
 * 设置数据丢弃策略。
 *
 * @param {tk_ostream_t*} ostream ostream对象。
 * @param {data_discard_policy_t} discard_policy 丢弃策略。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_retry_set_discard_policy(tk_ostream_t* ostream,
                                          data_discard_policy_t discard_policy);

#define TK_OSTREAM_RETRY(obj) ((tk_ostream_retry_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_RETRY_H*/
