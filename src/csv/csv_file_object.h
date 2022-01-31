/**
 * File:   object_csv_file.h
 * Author: AWTK Develop Team
 * Brief:  csv file object
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-07-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CSV_FILE_OBJECT_H
#define TK_CSV_FILE_OBJECT_H

#include "tkc/object.h"
#include "csv_file.h"

BEGIN_C_DECLS

/**
 * @class csv_file_object_t
 * @parent tk_object_t
 * @annotation["fake"]
 * 将cvs file包装成object对象。
 */

/**
 * @method csv_file_object_create
 *
 * 将csv_file对象包装成object。
 *
 * @param {csv_file_t*} csv csv对象(由object释放)。
 * 
 * @return {tk_object_t*} 返回对象。
 */
tk_object_t* csv_file_object_create(csv_file_t* csv);

/**
 * @method csv_file_object_get_csv
 *
 * 获取csv对象。
 *
 * @param {tk_object_t*} obj obj对象。
 * 
 * @return {csv_file_t*} 返回csv对象。
 */
csv_file_t* csv_file_object_get_csv(tk_object_t* obj);

END_C_DECLS

#endif /*TK_CSV_FILE_OBJECT_H*/
