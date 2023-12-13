/**
 * File:   csv_row_object.h
 * Author: AWTK Develop Team
 * Brief:  csv row object
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License row for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-12-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CSV_ROW_OBJECT_H
#define TK_CSV_ROW_OBJECT_H

#include "tkc/object.h"
#include "csv_file_object.h"

BEGIN_C_DECLS

/**
 * @class csv_row_object_t
 * @parent tk_object_t
 * 将cvs row包装成object对象。
 * 
 */

typedef struct _csv_row_object_t {
  tk_object_t object;

  /*private*/
  csv_row_t row;
  tk_object_t* csv;
} csv_row_object_t;

/**
 * @method csv_row_object_create
 *
 * 将csv_row对象包装成object。
 *
 * @param {tk_object_t*} csv csv对象。
 * @param {const char*} init 初始化数据。
 * 
 * @return {tk_object_t*} 返回对象。
 */
tk_object_t* csv_row_object_create(tk_object_t* csv, const char* init);

END_C_DECLS

#endif /*TK_CSV_ROW_OBJECT_H*/
