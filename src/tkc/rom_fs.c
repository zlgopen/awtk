/**
 * File:   rom_file.c
 * Author: AWTK Develop Team
 * Brief:  posix file api for rom data.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/asset_info.h"

#ifndef SEEK_SET
#define SEEK_SET 0 /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1 /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* set file offset to EOF plus offset */
#endif

#include "tkc/mem.h"
#include "tkc/rom_fs.h"

rom_file_t* rom_fopen(const char* name, const char* mode) {
  const asset_info_t* res = (const asset_info_t*)(name);
  return_value_if_fail(res != NULL && mode != NULL, NULL);

  return rom_fopen_buff(res->data, res->size);
}

rom_file_t* rom_fopen_buff(const uint8_t* data, uint32_t capacity) {
  rom_file_t* f = NULL;
  return_value_if_fail(data != NULL, NULL);

  f = TKMEM_ZALLOC(rom_file_t);
  return_value_if_fail(f != NULL, NULL);

  f->cursor = 0;
  f->data = data;
  f->capacity = capacity;

  return f;
}

size_t rom_fread(void* ptr, size_t size, size_t nitems, rom_file_t* f) {
  size_t available = 0;
  size_t nr = size * nitems;
  return_value_if_fail(size > 0, 0);
  return_value_if_fail(ptr != NULL && f != NULL, 0);

  available = f->capacity - f->cursor;
  nr = tk_min(nr, available);

  memcpy(ptr, f->data + f->cursor, nr);
  f->cursor += nr;

  return nr / size;
}

size_t rom_fwrite(const void* ptr, size_t size, size_t nitems, rom_file_t* f) {
  return_value_if_fail(ptr != NULL && f != NULL, 0);
  (void)size;
  (void)nitems;

  return nitems;
}

long rom_ftell(rom_file_t* f) {
  return_value_if_fail(f != NULL, -1);

  return f->cursor;
}

int rom_feof(rom_file_t* f) {
  return_value_if_fail(f != NULL, 1);

  return f->cursor >= f->capacity;
}

int rom_ferror(rom_file_t* f) {
  return 0;
}

int rom_fseek(rom_file_t* f, long offset, int whence) {
  return_value_if_fail(f != NULL, 1);

  switch (offset) {
    case SEEK_SET: {
      f->cursor = whence;
      break;
    }
    case SEEK_END: {
      f->cursor = f->capacity;
    }
    default: {
      f->cursor += whence;
      break;
    }
  }

  return 0;
}

ret_t rom_fclose(rom_file_t* f) {
  return_value_if_fail(f != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(f);

  return RET_OK;
}
