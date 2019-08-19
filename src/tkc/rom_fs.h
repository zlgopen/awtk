/**
 * File:   rom_file.h
 * Author: AWTK Develop Team
 * Brief:  posix file api for rom data.
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ROM_FS_H
#define TK_ROM_FS_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _rom_file_t {
  uint32_t cursor;
  uint32_t capacity;
  const uint8_t* data;
} rom_file_t;

rom_file_t* rom_fopen(const char* name, const char* mode);
rom_file_t* rom_fopen_buff(const uint8_t* data, uint32_t capacity);
long rom_ftell(rom_file_t* f);
int rom_feof(rom_file_t* f);
int rom_ferror(rom_file_t* f);
int rom_fseek(rom_file_t* f, long offset, int whence);
size_t rom_fread(void* ptr, size_t size, size_t nitems, rom_file_t* f);
size_t rom_fwrite(const void* ptr, size_t size, size_t nitems, rom_file_t* f);
ret_t rom_fclose(rom_file_t* f);

END_C_DECLS

#endif /*TK_ROM_FS_H*/
