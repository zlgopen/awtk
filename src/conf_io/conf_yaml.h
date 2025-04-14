/**
 * File:   yaml.h
 * Author: AWTK Develop Team
 * Brief:  yaml 
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_YAML_H
#define TK_CONF_YAML_H

#include "tkc/str.h"
#include "tkc/buffer.h"
#include "conf_io/conf_obj.h"

BEGIN_C_DECLS

/**
 * @class conf_yaml_t
 * @parent tk_object_t
 * @annotation ["fake"]
 * 
 * conf yaml对象。
 *
 * 示例
 *
 *```c 
 * char filename[MAX_PATH + 1] = {0};
 * path_prepend_temp_path(filename, "test.yaml");
 *
 * const char *yaml_data1 = "root:\n"
 *                         "  name:awplc\n"
 *                         "  age:18\n"
 *                         "  weight:60.5\n"; 
 * ENSURE(file_write(filename, yaml_data1, strlen(yaml_data1)) == RET_OK);
 *
 * // 从文件加载
 * tk_object_t *yaml = conf_yaml_load(filename, FALSE);
 *
 * // 获取数据。
 * ENSURE(tk_str_eq(tk_object_get_prop_str(yaml, "root.name"), "awplc"));
 * ENSURE(tk_object_get_prop_int(yaml, "root.age", 0) == 18);
 * ENSURE(tk_object_get_prop_double(yaml, "root.weight", 0) == 60.5);
 *
 * // 销毁对象
 * TK_OBJECT_UNREF(yaml);
 *
 * // 从内存加载
 * yaml = conf_yaml_load_from_buff(yaml_data1, strlen(yaml_data1), FALSE);
 *
 * // 获取数据
 * ENSURE(tk_str_eq(tk_object_get_prop_str(yaml, "root.name"), "awplc"));
 * ENSURE(tk_object_get_prop_int(yaml, "root.age", 0) == 18);
 * ENSURE(tk_object_get_prop_double(yaml, "root.weight", 0) == 60.5);
 *
 * // 设置数据
 * ENSURE(tk_object_set_prop_int(yaml, "root.age", 20) == RET_OK);
 * ENSURE(tk_object_get_prop_int(yaml, "root.age", 0) == 20);
 *
 * // 保存到文件
 * ENSURE(conf_yaml_save_as(yaml, filename) == RET_OK);
 * ENSURE(file_exist(filename) == TRUE);
 *
 * // 销毁对象
 * TK_OBJECT_UNREF(yaml);
 *```        
 */

/**
 * @method conf_yaml_create 
 * 创建一个空的conf对象。 
 * @annotation ["constructor"]
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_yaml_create(void);

/**
 * @method conf_yaml_load 
 * 从指定URL加载YAML对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_yaml_load(const char* url, bool_t create_if_not_exist);

/**
 * @method conf_yaml_load_from_buff
 * 从内存加载YAML对象。 
 * @annotation ["constructor"]
 * 
 * @param {const void*} buff 数据。
 * @param {uint32_t} size  数据长度。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_yaml_load_from_buff(const void* buff, uint32_t size, bool_t create_if_not_exist);

/**
 * @method conf_yaml_save_to_buff
 * 将obj保存为YAML格式到内存。
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {wbuffer_t*} wb 返回结果(不要初始化，使用完成后要调用wbuffer_deyamlt)。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_yaml_save_to_buff(tk_object_t* obj, wbuffer_t* wb);

/**
 * @method conf_yaml_save_as
 * 将doc对象保存到指定URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_yaml_save_as(tk_object_t* obj, const char* url);

/*public for test*/

/**
 * @method conf_doc_load_yaml
 *
 * 从缓存区加载yaml格式的conf doc对象。
 *
 * @annotation ["global"]
 * 
 * @param {const char*} data 数据。
 * 
 * @return {conf_doc_t*} 返回conf_doc对象。
 */
conf_doc_t* conf_doc_load_yaml(const char* data);

/**
 * @method conf_doc_save_yaml
 * 
 * 保存conf doc对象为yaml格式。 
 * @annotation ["global"]
 * 
 * @param {conf_doc_t*} doc conf doc对象。
 * @param {str_t*} str 保存结果。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_doc_save_yaml(conf_doc_t* doc, str_t* str);

END_C_DECLS

#endif /*TK_CONF_YAML_H*/

