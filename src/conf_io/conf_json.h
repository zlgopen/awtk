/**
 * File:   json.h
 * Author: AWTK Develop Team
 * Brief:  json 
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "conf_io/conf_obj.h"

BEGIN_C_DECLS

/**
 * @class conf_json_t
 * @parent object_t
 * @annotation ["fake"]
 * 
 * conf json对象。
 */

/**
 * @method conf_json_load 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {object_t*} 返回配置对象。
 */
object_t* conf_json_load(const char* url, bool_t create_if_not_exist);

/*public for test*/

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
 * @method conf_doc_save_json 
 * 
 * @annotation ["global"]
 * 
 * @param {conf_doc_t*} doc doc对象。
 * @param {str_t*} str 返回结果的字符串对象。 
 * 
 * @return {object_t*} 返回配置对象。
 */
ret_t conf_doc_save_json(conf_doc_t* doc, str_t* str);

END_C_DECLS

#endif /*TK_CONF_JSON_H*/
