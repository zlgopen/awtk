/**
 * File:   path.h
 * Author: AWTK Develop Team
 * Brief:  path
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-08-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PATH_H
#define TK_PATH_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

ret_t path_basename(const char* path, char* result, int32_t size);
ret_t path_extname(const char* path, char* result, int32_t size);
ret_t path_dirname(const char* path, char* result, int32_t size);
ret_t path_normalize(const char* path, char* result, int32_t size);
ret_t path_abs(const char* path, char* result, int32_t size);
ret_t path_build(char* result, int32_t size, ...);
ret_t path_replace_basename(char* result, int32_t size, const char* filename, const char* basename);

ret_t path_cwd(char path[MAX_PATH + 1]);
ret_t path_exe(char path[MAX_PATH + 1]);
ret_t path_app_root(char path[MAX_PATH + 1]);

bool_t path_exist(const char* path);
ret_t path_create(const char* path);
ret_t path_remove(const char* path);

END_C_DECLS

#endif /*TK_PATH_H*/
