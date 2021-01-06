/**
 * File:   fscript_iostream_file.h
 * Author: AWTK Develop Team
 * Brief:  iostream_file functions for fscript
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

#ifndef TK_FSCRIPT_IOSTREAM_FILE_H
#define TK_FSCRIPT_IOSTREAM_FILE_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_iostream_file_register
 * 注册iostream_file相关函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_iostream_file_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_IOSTREAM_FILE_H*/
