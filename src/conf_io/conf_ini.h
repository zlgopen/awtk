/**
 * File:   ini.h
 * Author: AWTK Develop Team
 * Brief:  ini 
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-05-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_INI_H
#define TK_CONF_INI_H

#include "tkc/str.h"
#include "conf_io/conf_obj.h"

BEGIN_C_DECLS

/**
 * @class conf_ini_t
 * @parent tk_object_t
 * @annotation ["fake"]
 * 
 * conf ini对象。
 */

/**
 * @method conf_ini_create 
 * 创建一个空的conf对象。 
 * @annotation ["constructor"]
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_ini_create(void);

/**
 * @method conf_ini_load 
 * 从指定URL加载INI对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_ini_load(const char* url, bool_t create_if_not_exist);

/**
 * @method conf_ini_save_as
 * 将doc对象保存到指定URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_ini_save_as(tk_object_t* obj, const char* url);

/*public for test*/

/**
 * @method conf_doc_load_ini
 *
 * 从缓存区加载ini格式的conf doc对象。
 *
 * @annotation ["global"]
 * 
 * @param {const char*} data 数据。
 * 
 * @return {conf_doc_t*} 返回conf_doc对象。
 */
conf_doc_t* conf_doc_load_ini(const char* data);

/**
 * @method conf_doc_save_ini
 * 
 * 保存conf doc对象为ini格式。 
 * @annotation ["global"]
 * 
 * @param {conf_doc_t*} doc conf doc对象。
 * @param {str_t*} str 保存结果。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_doc_save_ini(conf_doc_t* doc, str_t* str);

END_C_DECLS

#endif /*TK_CONF_INI_H*/
