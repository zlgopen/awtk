/**
 * File:   json.h
 * Author: AWTK Develop Team
 * Brief:  json
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
 * 2020-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_JSON_H
#define TK_CONF_JSON_H

#include "tkc/str.h"
#include "tkc/buffer.h"
#include "conf_io/conf_obj.h"

BEGIN_C_DECLS

/**
 * @class conf_json_t
 * @parent tk_object_t
 * @annotation ["fake"]
 *
 * conf json对象。
 *
 * 示例
 *
 *```c
 * char filename[MAX_PATH + 1] = {0};
 * path_prepend_temp_path(filename, "test.json");
 *
 * const char *json_data1 = "{"
 *                          "{\"root\":{"
 *                          "\"name\":\"awplc\","
 *                          "\"age\":18,"
 *                          "\"weight\":60.5"
 *                          "}"
 *                          "}";
 * ENSURE(file_write(filename, json_data1, strlen(json_data1)) == RET_OK);
 *
 * // 从文件加载
 * tk_object_t *json = conf_json_load(filename, FALSE);
 *
 * // 获取数据。
 * ENSURE(tk_str_eq(tk_object_get_prop_str(json, "root.name"), "awplc"));
 * ENSURE(tk_object_get_prop_int(json, "root.age", 0) == 18);
 * ENSURE(tk_object_get_prop_double(json, "root.weight", 0) == 60.5);
 *
 * // 销毁对象
 * TK_OBJECT_UNREF(json);
 *
 * // 从内存加载
 * json = conf_json_load_from_buff(json_data1, strlen(json_data1), FALSE);
 *
 * // 获取数据
 * ENSURE(tk_str_eq(tk_object_get_prop_str(json, "root.name"), "awplc"));
 * ENSURE(tk_object_get_prop_int(json, "root.age", 0) == 18);
 * ENSURE(tk_object_get_prop_double(json, "root.weight", 0) == 60.5);
 *
 * // 设置数据
 * ENSURE(tk_object_set_prop_int(json, "root.age", 20) == RET_OK);
 * ENSURE(tk_object_get_prop_int(json, "root.age", 0) == 20);
 *
 * // 保存到文件
 * ENSURE(conf_json_save_as(json, filename) == RET_OK);
 * ENSURE(file_exist(filename) == TRUE);
 *
 * // 销毁对象
 * TK_OBJECT_UNREF(json);
 *```
 */

/**
 * @method conf_json_create
 * 创建一个空的conf对象。
 * @annotation ["constructor"]
 *
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_create(void);

/**
 * @method conf_json_load
 * 从指定URL加载JSON对象。
 * @annotation ["constructor"]
 *
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。
 *
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_load(const char* url, bool_t create_if_not_exist);

/**
 * @method conf_json_load_ex
 * 从指定URL加载JSON对象。
 * @annotation ["constructor"]
 *
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。
 * @param {bool_t} use_extend_type 是否使用拓展类型。
 *
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_load_ex(const char* url, bool_t create_if_not_exist, bool_t use_extend_type);

/**
 * @method conf_json_load_from_buff
 * 从内存加载JSON对象。
 * @annotation ["constructor"]
 *
 * @param {const void*} buff 数据。
 * @param {uint32_t} size  数据长度。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。
 *
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_load_from_buff(const void* buff, uint32_t size, bool_t create_if_not_exist);

/**
 * @method conf_json_save_to_buff
 * 将obj保存为JSON格式到内存。
 *
 * @param {tk_object_t*} obj doc对象。
 * @param {wbuffer_t*} wb 返回结果(不要初始化，使用完成后要调用wbuffer_deinit)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_json_save_to_buff(tk_object_t* obj, wbuffer_t* wb);

/**
 * @method conf_json_save_as
 * 将doc对象保存到指定URL。
 * @annotation ["static"]
 *
 * @param {tk_object_t*} obj doc对象。
 * @param {const char*} url 保存的位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_json_save_as(tk_object_t* obj, const char* url);

/**
 * @method conf_doc_load_json
 *
 * @annotation ["global"]
 *
 * @param {const char*} data JSON数据。
 * @param {int32_t} size JSON数据长度。
 *
 * @return {conf_doc_t*} 返回doc对象。
 */
conf_doc_t* conf_doc_load_json(const char* data, int32_t size);

/**
 * @method conf_node_load_json
 * 将 JSON 字符串加载到指定路径的节点。
 *
 * @param {conf_doc_t*} doc doc 对象。
 * @param {const char*} path 节点路径。
 * @param {const char*} data JSON 字符串。
 * @param {int32_t} size JSON 字符串长度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t conf_node_load_json(conf_doc_t* doc, const char* path, const char* data, int32_t size);

/**
 * @method conf_doc_save_json
 *
 * @annotation ["global"]
 *
 * @param {conf_doc_t*} doc doc对象。
 * @param {str_t*} str 保存结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_doc_save_json(conf_doc_t* doc, str_t* str);

/**
 * @method conf_doc_save_json_ex
 * 功能描述
 *
 * @param {conf_doc_t*} doc doc对象
 * @param {str_t*}      str 保存结果
 * @param {uint32_t}    indent 缩进
 *
 * @return {ret_t} 返回 ret_t值
 */
ret_t conf_doc_save_json_ex(conf_doc_t* doc, str_t* str, uint32_t indent);

END_C_DECLS

#endif /*TK_CONF_JSON_H*/
