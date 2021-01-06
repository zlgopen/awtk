/**
 * File:   fscript_istream.h
 * Author: AWTK Develop Team
 * Brief:  wrap istream for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-06 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_ISTREAM_H
#define TK_FSCRIPT_ISTREAM_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_istream_register
 * 注册istream相关等函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_istream_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_ISTREAM_H*/
