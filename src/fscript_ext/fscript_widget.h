/**
 * File:   fscript_widget.h
 * Author: AWTK Develop Team
 * Brief:  widget functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-08 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_WIDGET_H
#define TK_FSCRIPT_WIDGET_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_widget_register
 * @export none
 * 注册widget函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_widget_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_WIDGET_H*/
