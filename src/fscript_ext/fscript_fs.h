/**
 * File:   fscript_fs.h
 * Author: AWTK Develop Team
 * Brief:  fs functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-07 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_FS_H
#define TK_FSCRIPT_FS_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_fs_register
 * 注册文件系统函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_fs_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_FS_H*/
