﻿/**
 * File:   fscript_json.h
 * Author: AWTK Develop Team
 * Brief:  json functions for fscript
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-07 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_JSON_H
#define TK_FSCRIPT_JSON_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_json_register
 * 注册json函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_json_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_JSON_H*/
