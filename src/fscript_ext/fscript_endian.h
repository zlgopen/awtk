/**
 * File:   fscript_endian.h
 * Author: AWTK Develop Team
 * Brief:  endian functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_ENDIAN_H
#define TK_FSCRIPT_ENDIAN_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_endian_register
 * 注册大端小断转换函数等函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_endian_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_ENDIAN_H*/
