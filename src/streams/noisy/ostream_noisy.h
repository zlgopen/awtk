/**
 * File:   ostream_noisy.h
 * Author: AWTK Develop Team
 * Brief:  inject error into stream
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
 * 2019-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OSTREAM_NOISY_H
#define TK_OSTREAM_NOISY_H

#include "tkc/buffer.h"
#include "tkc/ostream.h"

BEGIN_C_DECLS

struct _tk_ostream_noisy_t;
typedef struct _tk_ostream_noisy_t tk_ostream_noisy_t;

/**
 * @class tk_ostream_noisy_t
 * @parent tk_ostream_t
 *
 * 在输出流中注入错误，方便进行可靠性测试。
 *
 */
struct _tk_ostream_noisy_t {
  tk_ostream_t ostream;

  /**
   * @property {uint32_t} error_level
   * error level.
   *
   */
  uint32_t error_level;
  /**
   * @property {uint32_t} error_count
   * 注入的错误数。
   */
  uint32_t error_count;
  /**
   * @property {uint32_t} write_count
   * 写入的次数。
   */
  uint32_t write_count;

  wbuffer_t wb;
  tk_ostream_t* real_ostream;
};

/**
 * @method tk_ostream_noisy_create
 *
 * 创建ostream对象。
 *
 *> 只能由iostream_noisy调用。
 *
 * @param {tk_ostream_t*} real_ostream 实际的ostream对象。
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_noisy_create(tk_ostream_t* real_ostream);

#define TK_OSTREAM_NOISY_PROP_ERROR_LEVEL "error_level"
#define TK_OSTREAM_NOISY_PROP_ERROR_COUNT "error_count"
#define TK_OSTREAM_NOISY_PROP_WRITE_COUNT "write_count"

#define TK_OSTREAM_NOISY(obj) ((tk_ostream_noisy_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_NOISY_H*/
