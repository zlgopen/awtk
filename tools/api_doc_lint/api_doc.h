/**
 * File:   api_doc.h
 * Author: AWTK Develop Team
 * Brief:  检查 api doc是否合法, 支持自动修正
 *
 * Copyright (c) 2022 - 2023 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-02-28 Wang JunSheng <wangjunsheng@zlg.cn> created
 *
 */

#include "code_assist.h"
#include "tkc/log.h"

BEGIN_C_DECLS

typedef void (*log_hook_t)(void* ctx, log_level_t level, const char* s);
bool_t check_api_doc(code_assist_t* ca, const char* filename, log_hook_t hook, void* log_ctx, bool_t auto_fix, bool_t* checked);

/* 方便自动测试 */
typedef void (*auto_fix_hook_t)(const char* path, const char* buff, uint32_t size);
bool_t check_api_doc2(code_assist_t* ca, const char* filename, log_hook_t hook, void* log_ctx, bool_t auto_fix, auto_fix_hook_t fix_hook, bool_t* checked);

END_C_DECLS