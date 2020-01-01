/**
 * File:   dialog_highlighter_builts.h
 * Author: AWTK Develop Team
 * Brief:  builts dialog_highlighter
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied highlighterrranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIALOG_HIGHLIGHTER_BUILTINS_H
#define TK_DIALOG_HIGHLIGHTER_BUILTINS_H

#include "base/dialog_highlighter.h"

BEGIN_C_DECLS

/**
 * @method dialog_highlighter_register_builtins
 * @annotation ["private"]
 *
 * 注册内置的对话框高亮策略。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_register_builtins(void);

END_C_DECLS

#endif /*TK_DIALOG_HIGHLIGHTER_BUILTINS_H*/
