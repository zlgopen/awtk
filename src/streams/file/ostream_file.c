/**
 * File:   ostream_file.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on file
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "streams/file/ostream_file.h"

static int32_t tk_ostream_file_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  tk_ostream_file_t* ostream_file = TK_OSTREAM_FILE(stream);

  return fs_file_write(ostream_file->file, buff, max_size);
}

static ret_t tk_ostream_file_seek(tk_ostream_t* stream, uint32_t offset) {
  tk_ostream_file_t* ostream_file = TK_OSTREAM_FILE(stream);

  return fs_file_seek(ostream_file->file, offset);
}

static ret_t tk_ostream_file_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t tk_ostream_file_get_prop(object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t tk_ostream_file_on_destroy(object_t* obj) {
  tk_ostream_file_t* ostream_file = TK_OSTREAM_FILE(obj);

  fs_file_close(ostream_file->file);
  ostream_file->file = NULL;

  return RET_OK;
}

static const object_vtable_t s_tk_ostream_file_vtable = {.type = "tk_ostream_file",
                                                         .desc = "tk_ostream_file",
                                                         .size = sizeof(tk_ostream_file_t),
                                                         .on_destroy = tk_ostream_file_on_destroy,
                                                         .get_prop = tk_ostream_file_get_prop,
                                                         .set_prop = tk_ostream_file_set_prop};

tk_ostream_t* tk_ostream_file_create(const char* filename) {
  object_t* obj = NULL;
  fs_file_t* file = NULL;
  tk_ostream_file_t* ostream_file = NULL;
  return_value_if_fail(filename != NULL, NULL);

  file = fs_open_file(os_fs(), filename, "wb+");
  return_value_if_fail(file != NULL, NULL);

  obj = object_create(&s_tk_ostream_file_vtable);
  ostream_file = TK_OSTREAM_FILE(obj);
  if (ostream_file == NULL) {
    fs_file_close(file);
    return_value_if_fail(ostream_file != NULL, NULL);
  }

  ostream_file->file = file;
  TK_OSTREAM(obj)->write = tk_ostream_file_write;
  TK_OSTREAM(obj)->seek = tk_ostream_file_seek;

  return TK_OSTREAM(obj);
}
