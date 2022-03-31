/**
 * File:   iostream_stats.h
 * Author: AWTK Develop Team
 * Brief:  iostream base on mem
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

#ifndef TK_IOSTREAM_STATS_H
#define TK_IOSTREAM_STATS_H

#include "tkc/fs.h"
#include "tkc/iostream.h"

BEGIN_C_DECLS

#define TK_ISTREAM_STATS_PROP_READ "i_stats_read"
#define TK_OSTREAM_STATS_PROP_WRITE "o_stats_write"

#define TK_ISTREAM_STATS_CMD_RESET "i_stats_reset"
#define TK_OSTREAM_STATS_CMD_RESET "o_stats_reset"

struct _tk_iostream_stats_t;
typedef struct _tk_iostream_stats_t tk_iostream_stats_t;

/**
 * @class tk_iostream_stats_t
 * @parent tk_iostream_t
 *
 * 带统计功能的输入输出流。
 *
 */
struct _tk_iostream_stats_t {
  tk_iostream_t iostream;
  tk_istream_t* istream;
  tk_ostream_t* ostream;

  tk_iostream_t* impl;
};

/**
 * @method tk_iostream_stats_create
 *
 * 创建iostream对象。
 *
 * @param {tk_iostream_t*} impl 被统计的流对象。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_stats_create(tk_iostream_t* impl);

#define TK_IOSTREAM_STATS(obj) ((tk_iostream_stats_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_STATS_H*/
