/**
 * File:   fscript_ext.h
 * Author: AWTK Develop Team
 * Brief:  ext functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_EXT_H
#define TK_FSCRIPT_EXT_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_ext_init
 * 注册扩展函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_ext_init(void);

END_C_DECLS

#endif /*TK_FSCRIPT_EXT_H*/
