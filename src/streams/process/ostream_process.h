/**
 * File:   ostream_process.h
 * Author: AWTK Develop Team
 * Brief:  output stream base on process
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_OSTREAM_PROCESS_H
#define TK_OSTREAM_PROCESS_H

#include "tkc/fs.h"
#include "tkc/ostream.h"
#include "process_helper.h"

BEGIN_C_DECLS

struct _tk_ostream_process_t;
typedef struct _tk_ostream_process_t tk_ostream_process_t;

/**
 * @class tk_ostream_process_t
 * 基于串口实现的输出流。
 *
 * input stream base on fd
 *
 */
struct _tk_ostream_process_t {
  tk_ostream_t ostream;

  /*private*/
  process_handle_t handle;
};

/**
 * @method tk_ostream_process_create
 *
 * 创建ostream对象。
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_process_create(void);

#define TK_OSTREAM_PROCESS(obj) ((tk_ostream_process_t*)(obj))

/* internal */
ret_t tk_ostream_process_set_handle(tk_ostream_t* ostream, process_handle_t handle);

END_C_DECLS

#endif /*TK_OSTREAM_PROCESS_H*/
