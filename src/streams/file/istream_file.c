/**
 * File:   istream_file.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on file
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "streams/file/istream_file.h"

static int32_t tk_istream_file_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(stream);

  return fs_file_read(istream_file->file, buff, max_size);
}

static ret_t tk_istream_file_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(stream);
  if (!fs_file_eof(istream_file->file)) {
    return RET_OK;
  } else {
    return RET_EOS;
  }
}

static ret_t tk_istream_file_seek(tk_istream_t* stream, uint32_t offset) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(stream);

  return fs_file_seek(istream_file->file, offset);
}

static int32_t tk_istream_file_tell(tk_istream_t* stream) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(stream);

  return fs_file_tell(istream_file->file);
}

static bool_t tk_istream_file_eos(tk_istream_t* stream) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(stream);

  return fs_file_eof(istream_file->file);
}

static ret_t tk_istream_file_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t tk_istream_file_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_IS_EOS)) {
    value_set_bool(v, fs_file_eof(istream_file->file));

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_istream_file_on_destroy(tk_object_t* obj) {
  tk_istream_file_t* istream_file = TK_ISTREAM_FILE(obj);

  fs_file_close(istream_file->file);
  istream_file->file = NULL;

  return RET_OK;
}

static const object_vtable_t s_tk_istream_file_vtable = {.type = "tk_istream_file",
                                                         .desc = "tk_istream_file",
                                                         .size = sizeof(tk_istream_file_t),
                                                         .on_destroy = tk_istream_file_on_destroy,
                                                         .get_prop = tk_istream_file_get_prop,
                                                         .set_prop = tk_istream_file_set_prop};

tk_istream_t* tk_istream_file_create_ex(const char* filename, const char* mode) {
  tk_object_t* obj = NULL;
  fs_file_t* file = NULL;
  tk_istream_file_t* istream_file = NULL;
  return_value_if_fail(filename != NULL && mode != NULL, NULL);

  file = fs_open_file(os_fs(), filename, mode);
  return_value_if_fail(file != NULL, NULL);

  obj = tk_object_create(&s_tk_istream_file_vtable);
  istream_file = TK_ISTREAM_FILE(obj);
  if (istream_file == NULL) {
    fs_file_close(file);
    return_value_if_fail(istream_file != NULL, NULL);
  }

  istream_file->file = file;
  TK_ISTREAM(obj)->read = tk_istream_file_read;
  TK_ISTREAM(obj)->seek = tk_istream_file_seek;
  TK_ISTREAM(obj)->tell = tk_istream_file_tell;
  TK_ISTREAM(obj)->eos = tk_istream_file_eos;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_file_wait_for_data;

  return TK_ISTREAM(obj);
}

tk_istream_t* tk_istream_file_create(const char* filename) {
  return tk_istream_file_create_ex(filename, "rb");
}
