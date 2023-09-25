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

/**
 * @method csv_file_object_load 
 * 从指定文件加载CSV对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} filename 文件名。
 * @param {char} sep 分隔符。
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* csv_file_object_load(const char* filename, char sep);

/**
 * @method csv_file_object_load_from_buff
 * 从内存加载CSV对象。 
 * @annotation ["constructor"]
 * 
 * @param {const void*} buff 数据。
 * @param {uint32_t} size  数据长度。
 * @param {char} sep 分隔符。
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* csv_file_object_load_from_buff(const void* buff, uint32_t size, char sep);

/**
 * @method csv_file_object_save_to_buff
 * 将obj保存为CSV格式到内存。
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {wbuffer_t*} wb 返回结果(不要初始化，使用完成后要调用wbuffer_deinit)。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t csv_file_object_save_to_buff(tk_object_t* obj, wbuffer_t* wb);

/**
 * @method csv_file_object_save_as
 * 将doc对象保存到指定文件。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {const char*} filename 文件名。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t csv_file_object_save_as(tk_object_t* obj, const char* filename);

END_C_DECLS

#endif /*TK_CSV_FILE_OBJECT_H*/
