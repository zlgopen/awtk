/**
 * File:   istream_process.h
 * Author: AWTK Develop Team
 * Brief:  input stream on process
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-09-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_ISTREAM_PROCESS_H
#define TK_ISTREAM_PROCESS_H

#include "tkc/fs.h"
#include "tkc/istream.h"
#include "process_helper.h"

BEGIN_C_DECLS

struct _tk_istream_process_t;
typedef struct _tk_istream_process_t tk_istream_process_t;

/**
 * @class tk_istream_process_t
 * @parent tk_istream_t
 *
 * 基于子程序实现的输入流。
 *
 */
struct _tk_istream_process_t {
  tk_istream_t istream;

  /*private*/
  process_handle_t handle;
};

/**
 * @method tk_istream_process_create
 *
 * 创建istream对象。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_process_create(void);

#define TK_ISTREAM_PROCESS(obj) ((tk_istream_process_t*)(obj))

/* internal */
ret_t tk_istream_process_set_handle(tk_istream_t* istream, process_handle_t handle);

END_C_DECLS

#endif /*TK_ISTREAM_PROCESS_H*/
