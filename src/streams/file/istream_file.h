/**
 * File:   istream_file.h
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

#ifndef TK_ISTREAM_FILE_H
#define TK_ISTREAM_FILE_H

#include "tkc/fs.h"
#include "tkc/istream.h"

BEGIN_C_DECLS

struct _tk_istream_file_t;
typedef struct _tk_istream_file_t tk_istream_file_t;

/**
 * @class tk_istream_file_t
 * @parent tk_istream_t
 *
 * 基于文件实现的输入流。
 *
 */
struct _tk_istream_file_t {
  tk_istream_t istream;

  fs_file_t* file;
};

/**
 * @method tk_istream_file_create
 *
 * 创建istream对象。
 *
 * @param {const char*} filename 文件名。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_file_create(const char* filename);

#define TK_ISTREAM_FILE(obj) ((tk_istream_file_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_FILE_H*/
