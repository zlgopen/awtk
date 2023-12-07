/**
 * File:   csv_file_object.h
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
 * 返回值：
   * RET_OK: 保留
   * RET_STOP: 停止解析
   * RET_FAIL: 忽略此行
 */
typedef ret_t (*csv_file_object_filter_t)(void* ctx, tk_object_t* args, uint32_t index, csv_row_t* row);

/**
 * @class csv_file_object_t
 * @parent tk_object_t
 * 将cvs file包装成object对象。
 * 
 * 示例
 * 
 * ```c
 *  char filename[MAX_PATH + 1] = {0};
 *  path_prepend_temp_path(filename, "test.csv");
 * 
 *  const char *csv_data1 = "name,age,weight\n"
 *                          "awplc,18,60.5\n"; 
 *  ENSURE(file_write(filename, csv_data1, strlen(csv_data1)) == RET_OK);
 * 
 *  // 从文件加载
 *  tk_object_t *csv = csv_file_object_load(filename, ',');
 * 
 *  // 获取数据: 通过属性名
 *  ENSURE(tk_str_eq(tk_object_get_prop_str(csv, "[1].name"), "awplc"));
 *  ENSURE(tk_object_get_prop_int(csv, "[1].age", 0) == 18);
 *  ENSURE(tk_object_get_prop_double(csv, "[1].weight", 0) == 60.5);
 * 
 *  // 获取数据: 通过属性索引
 *  ENSURE(tk_str_eq(tk_object_get_prop_str(csv, "[1].0"), "awplc"));
 *  ENSURE(tk_object_get_prop_int(csv, "[1].1", 0) == 18);
 *  ENSURE(tk_object_get_prop_double(csv, "[1].2", 0) == 60.5);
 * 
 *  // 销毁对象
 *  TK_OBJECT_UNREF(csv);
 * 
 *  // 从内存加载
 *  csv = csv_file_object_load_from_buff(csv_data1, strlen(csv_data1), ',');
 * 
 *  // 获取数据: 通过属性名
 *  ENSURE(tk_str_eq(tk_object_get_prop_str(csv, "[1].name"), "awplc"));
 *  ENSURE(tk_object_get_prop_int(csv, "[1].age", 0) == 18);
 *  ENSURE(tk_object_get_prop_double(csv, "[1].weight", 0) == 60.5);
 * 
 *  // 获取数据: 通过属性索引
 *  ENSURE(tk_str_eq(tk_object_get_prop_str(csv, "[1].0"), "awplc"));
 *  ENSURE(tk_object_get_prop_int(csv, "[1].1", 0) == 18);
 *  ENSURE(tk_object_get_prop_double(csv, "[1].2", 0) == 60.5);
 * 
 *  // 设置数据
 *  ENSURE(tk_object_set_prop_int(csv, "[1].age", 20) == RET_OK);
 *  ENSURE(tk_object_get_prop_int(csv, "[1].age", 0) == 20);
 * 
 *  // 保存到文件
 *  ENSURE(csv_file_object_save_as(csv, filename) == RET_OK);
 *  ENSURE(file_exist(filename) == TRUE);
 * 
 *  // 销毁对象
 *  TK_OBJECT_UNREF(csv);
 * ```
 */

typedef struct _csv_file_object_t {
  tk_object_t object;

  /*private*/
  csv_file_t* csv;
  str_t str;
  
  tk_object_t* query_args;
  uint32_t* rows_map;
  uint32_t rows_map_size;
  uint32_t rows_map_capacity;
  csv_file_object_filter_t filter;
  void* filter_ctx;
} csv_file_object_t;

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
 * @method csv_file_object_set_filter
 * 设置过滤器。
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {csv_file_object_filter_t} filter 过滤器。
 * @param {void*} ctx 上下文。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t csv_file_object_set_filter(tk_object_t* obj, csv_file_object_filter_t filter, void* ctx);

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

#define CSV_QUERY_PREFIX "query."
#define CSV_CMD_QUERY "query"
#define CSV_CMD_QUERY_ARG_CLEAR "clear"

END_C_DECLS

#endif /*TK_CSV_FILE_OBJECT_H*/
