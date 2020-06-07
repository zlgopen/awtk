/**
 * File:   ubjson.h
 * Author: AWTK Develop Team
 * Brief:  ubjson 
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

#ifndef TK_CONF_UBJSON_H
#define TK_CONF_UBJSON_H

#include "conf_io/conf_obj.h"
#include "ubjson/ubjson_writer.h"
#include "ubjson/ubjson_reader.h"

BEGIN_C_DECLS

/**
 * @class conf_ubjson_t
 * @parent object_t
 * @annotation ["fake"]
 * 
 * conf json对象。
 */

/**
 * @method conf_ubjson_load 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {object_t*} 返回配置对象。
 */
object_t* conf_ubjson_load(const char* url, bool_t create_if_not_exist);

/*public for test*/
conf_doc_t* conf_doc_load_ubjson(const void* data, uint32_t size);
ret_t conf_doc_save_ubjson(conf_doc_t* doc, ubjson_writer_t* writer);

END_C_DECLS

#endif /*TK_CONF_UBJSON_H*/
