/**
 * File:   ext_widgets.h
 * Author: AWTK Develop Team
 * Brief:  ext widgets
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-08-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EXT_WIDGETS_H
#define TK_EXT_WIDGETS_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class ext_widgets_t
 * 扩展控件。
 * @annotation ["scriptable", "fake"]
 *
 */

/**
 * @method tk_ext_widgets_init
 * 初始化AWTK扩展控件。
 * @alias ext_widgets_init
 * @annotation ["static", "scriptable"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_ext_widgets_init(void);

END_C_DECLS

#endif /*TK_EXT_WIDGETS_H*/
