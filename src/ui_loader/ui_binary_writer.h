/**
 * File:   ui_binary_writer.h
 * Author: AWTK Develop Team
 * Brief:  ui_builder write widget info into buffer.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_BINARY_WRITER_H
#define TK_UI_BINARY_WRITER_H

#include "tkc/buffer.h"
#include "base/ui_builder.h"

BEGIN_C_DECLS

/**
 * @class ui_binary_writer_t
 * @parent ui_builder_t
 *
 * 生成二进制格式的UI描述数据。
 *
 */
typedef struct _ui_binary_writer_t {
  ui_builder_t builder;
  wbuffer_t* wbuffer;
} ui_binary_writer_t;

/**
 * @method ui_binary_writer_init
 * @annotation ["constructor"]
 *
 * 初始化ui\_binary\_writer对象。
 *
 * @param {ui_binary_writer_t*} writer writer对象。
 * @param {wbuffer_t*} wbuffer 保存结果的buffer。
 *
 * @return {ui_builder_t*} 返回ui\_builder对象。
 */
ui_builder_t* ui_binary_writer_init(ui_binary_writer_t* writer, wbuffer_t* wbuffer);

END_C_DECLS

#endif /*TK_UI_BINARY_WRITER_H*/
