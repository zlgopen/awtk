/**
 * File:   fs.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  simple fs api
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-04-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FS_H
#define TK_FS_H

#include "base/types_def.h"

BEGIN_C_DECLS

ret_t fs_unlink(const char* name);
int32_t fs_file_size(const char* name);
void* fs_read_file(const char* name, uint32_t* size);
int32_t fs_read_file_part(const char* name, void* buff, uint32_t size, uint32_t offset);
ret_t fs_write_file(const char* name, const void* buff, uint32_t size);

END_C_DECLS

#endif /*TK_FS_H*/
