/**
 * File:   widgets.h
 * Author: AWTK Develop Team
 * Brief:  ext widgets
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
 * 2018-08-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGETS_H
#define TK_WIDGETS_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @method tk_widgets_init
 * 初始化AWTK控件。
 * @annotation ["global"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_widgets_init(void);

END_C_DECLS

#endif /*TK_WIDGETS_H*/
