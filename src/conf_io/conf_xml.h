﻿/**
 * File:   conf_xml.h
 * Author: AWTK Develop Team
 * Brief:  xml 
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
 * 2022-05-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_XML_H
#define TK_CONF_XML_H

#include "tkc/str.h"
#include "tkc/buffer.h"
#include "conf_io/conf_obj.h"

BEGIN_C_DECLS

/**
 * @class conf_xml_t
 * @parent tk_object_t
 * @annotation ["fake"]
 * conf xml对象。
 *
 * 示例
 *
 *```c 
 *  char filename[MAX_PATH + 1] = {0};
 *  path_prepend_temp_path(filename, "test.xml");
 *
 *  const char *xml_data1 = "<root><name>awplc</name><age>18</age><weight>60.5</weight></root>";
 *  ENSURE(file_write(filename, xml_data1, strlen(xml_data1)) == RET_OK);
 *
 *  // 从文件加载
 *  tk_object_t *xml = conf_xml_load(filename, FALSE);
 *
 *  // 获取数据。注意：从文本节点获取数据，需要加上@text
 *  ENSURE(tk_str_eq(tk_object_get_prop_str(xml, "root.name.@text"), "awplc"));
 *  ENSURE(tk_object_get_prop_int(xml, "root.age.@text", 0) == 18);
 *  ENSURE(tk_object_get_prop_double(xml, "root.weight.@text", 0) == 60.5);
 *
 *  // 销毁对象
 *  TK_OBJECT_UNREF(xml);
 *
 *  // 从内存加载
 *  const char *xml_data2 = "<root name=\"awplc\" age=\"18\" weight=\"60.5\"></root>";
 *  xml = conf_xml_load_from_buff(xml_data2, strlen(xml_data2), FALSE);
 *
 *  // 获取数据
 *  ENSURE(tk_str_eq(tk_object_get_prop_str(xml, "root.name"), "awplc"));
 *  ENSURE(tk_object_get_prop_int(xml, "root.age", 0) == 18);
 *  ENSURE(tk_object_get_prop_double(xml, "root.weight", 0) == 60.5);
 *
 *  // 设置数据
 *  ENSURE(tk_object_set_prop_int(xml, "root.age", 20) == RET_OK);
 *  ENSURE(tk_object_get_prop_int(xml, "root.age", 0) == 20);
 *
 *  // 保存到文件
 *  ENSURE(conf_xml_save_as(xml, filename) == RET_OK);
 *  ENSURE(file_exist(filename) == TRUE);
 *
 *  // 销毁对象
 *  TK_OBJECT_UNREF(xml); 
 *```        
 */

/**
 * @method conf_xml_create 
 * 创建一个空的conf对象。 
 * @annotation ["constructor"]
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_xml_create(void);

/**
 * @method conf_xml_load 
 * 从指定URL加载XML对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_xml_load(const char* url, bool_t create_if_not_exist);

/**
 * @method conf_xml_load_from_buff
 * 从内存加载XML对象。 
 * @annotation ["constructor"]
 * 
 * @param {const void*} buff 数据。
 * @param {uint32_t} size  数据长度。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_xml_load_from_buff(const void* buff, uint32_t size, bool_t create_if_not_exist);

/**
 * @method conf_xml_save_to_buff
 * 将obj保存为XML格式到内存。
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {wbuffer_t*} wb 返回结果(不要初始化，使用完成后要调用wbuffer_deinit)。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_xml_save_to_buff(tk_object_t* obj, wbuffer_t* wb);

/**
 * @method conf_xml_save_as
 * 将doc对象保存到指定URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_xml_save_as(tk_object_t* obj, const char* url);

/*public for test*/

/**
 * @method conf_doc_load_xml
 *
 * 从缓存区加载xml格式的conf doc对象。
 *
 * @annotation ["global"]
 * 
 * @param {const char*} data 数据。
 * 
 * @return {conf_doc_t*} 返回conf_doc对象。
 */
conf_doc_t* conf_doc_load_xml(const char* data);

/**
 * @method conf_doc_save_xml
 * 
 * 保存conf doc对象为xml格式。 
 * @annotation ["global"]
 * 
 * @param {conf_doc_t*} doc conf doc对象。
 * @param {str_t*} str 保存结果。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_doc_save_xml(conf_doc_t* doc, str_t* str);

/*用于获取文本内容*/
#define CONF_XML_TEXT "@text"

END_C_DECLS

#endif /*TK_CONF_XML_H*/
