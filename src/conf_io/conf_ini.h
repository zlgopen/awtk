/**
 * File:   ini.h
 * Author: AWTK Develop Team
 * Brief:  ini 
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
 * @parent object_t
 * @annotation ["fake"]
 * 
 * conf ini对象。
 */

/**
 * @method conf_ini_load 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {object_t*} 返回配置对象。
 */
object_t* conf_ini_load(const char* url, bool_t create_if_not_exist);

/*public for test*/
conf_doc_t* conf_doc_load_ini(const char* data);
ret_t conf_doc_save_ini(conf_doc_t* doc, str_t* str);

END_C_DECLS

#endif /*TK_CONF_INI_H*/
