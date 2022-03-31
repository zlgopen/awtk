/**
 * File:   ostream_stats.h
 * Author: AWTK Develop Team
 * Brief:  output stream base on memory
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 20220308 vih created
 *
 */

#ifndef TK_OSTREAM_STATS_H
#define TK_OSTREAM_STATS_H

#include "tkc/ostream.h"

BEGIN_C_DECLS

struct _tk_ostream_stats_t;
typedef struct _tk_ostream_stats_t tk_ostream_stats_t;

/**
 * @class tk_ostream_stats_t
 * @parent tk_ostream_t
 *
 * 带统计功能的输出流。
 *
 */
struct _tk_ostream_stats_t {
  tk_ostream_t ostream;
  tk_ostream_t* impl_ostream;

  uint32_t count;
};

/**
 * @method tk_ostream_stats_create
 *
 * 创建ostream对象。
 *
 * @param {tk_ostream_t*} impl 被统计的输出流对象
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_stats_create(tk_ostream_t* impl);

#define TK_OSTREAM_STATS(obj) ((tk_ostream_stats_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_STATS_H*/
