/**
 * File:   ubjson.h
 * Author: AWTK Develop Team
 * Brief:  ubjson 
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
 * @method conf_ubjson_create 
 * 创建一个空的conf对象。 
 * @annotation ["constructor"]
 * 
 * @return {object_t*} 返回配置对象。
 */
object_t* conf_ubjson_create(void);

/**
 * @method conf_ubjson_load 
 * 从指定URL加载UBJSON对象。 
 * 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {object_t*} 返回配置对象。
 */
object_t* conf_ubjson_load(const char* url, bool_t create_if_not_exist);

/**
 * @method conf_ubjson_save_as
 * 将doc对象保存到指定URL。
 * @annotation ["static"]
 * 
 * @param {object_t*} obj doc对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_ubjson_save_as(object_t* obj, const char* url);

/**
 * @method conf_doc_load_ubjson
 *
 * 从缓存区加载ubjson格式的conf doc对象。
 *
 * @annotation ["global"]
 * 
 * @param {const void*} data 数据。
 * @param {uint32_t} size 数据长度。
 * 
 * @return {conf_doc_t*} 返回conf_doc对象。
 */
conf_doc_t* conf_doc_load_ubjson(const void* data, uint32_t size);

/**
 * @method conf_doc_save_ubjson
 * 
 * 保存conf doc对象为ubjson格式。 
 * @annotation ["global"]
 * 
 * @param {conf_doc_t*} doc conf doc对象。
 * @param {ubjson_writer_t*} writer ubjson writer对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_doc_save_ubjson(conf_doc_t* doc, ubjson_writer_t* writer);

END_C_DECLS

#endif /*TK_CONF_UBJSON_H*/
