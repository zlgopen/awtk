/**
 * File:   istream_stats.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on memory
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

#ifndef TK_ISTREAM_STATS_H
#define TK_ISTREAM_STATS_H

#include "tkc/istream.h"

BEGIN_C_DECLS

struct _tk_istream_stats_t;
typedef struct _tk_istream_stats_t tk_istream_stats_t;

/**
 * @class tk_istream_stats_t
 * @parent tk_istream_t
 *
 * 带统计功能的输入流。
 *
 */
struct _tk_istream_stats_t {
  tk_istream_t istream;
  tk_istream_t* impl_istream;
  int count;
};

/**
 * @method tk_istream_stats_create
 *
 * 创建istream对象。
 *
 * @param {tk_istream_t*} impl 被统计的输入流对象
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_stats_create(tk_istream_t* impl);

#define TK_ISTREAM_STATS(obj) ((tk_istream_stats_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_STATS_H*/
