/**
 * File:   data_writer_factory.h
 * Author: AWTK Develop Team
 * Brief:  data writer factory
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
 * 2020-03-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_WRITER_FACTORY_H
#define TK_DATA_WRITER_FACTORY_H

#include "tkc/darray.h"
#include "tkc/data_writer.h"

BEGIN_C_DECLS

typedef data_writer_t* (*data_writer_create_t)(const char* args);

/**
 * @class data_writer_factory_t
 * data writer工厂。
 *
 */
typedef struct _data_writer_factory_t {
  /*private*/
  darray_t creators;
} data_writer_factory_t;

/**
 * @method data_writer_factory
 * 获取缺省的data writer工厂对象。
 * @annotation ["constructor"]
 * @return {data_writer_factory_t*} 返回data writer工厂对象。
 */
data_writer_factory_t* data_writer_factory(void);

/**
 * @method data_writer_factory_set
 * 设置缺省的data writer工厂对象。
 * @param {data_writer_factory_t*} factory data writer工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_writer_factory_set(data_writer_factory_t* factory);

/**
 * @method data_writer_factory_create
 * 创建data writer工厂对象。
 * @annotation ["constructor"]
 *
 * @return {data_writer_factory_t*} 返回data writer工厂对象。
 */
data_writer_factory_t* data_writer_factory_create(void);

/**
 * @method data_writer_factory_register
 * 注册data writer创建函数。
 * @param {data_writer_factory_t*} factory writer工厂对象。
 * @param {const char*} protocol 协议(如file)。
 * @param {data_writer_create_t} create data writer创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_writer_factory_register(data_writer_factory_t* factory, const char* protocol,
                                   data_writer_create_t create);
/**
 * @method data_writer_factory_create_writer
 * 创建指定类型的data writer对象。
 * @annotation ["constructor"]
 * @param {data_writer_factory_t*} factory data writer工厂对象。
 * @param {const char*} url URL。
 *
 * @return {data_writer_t*} 返回data writer对象。
 */
data_writer_t* data_writer_factory_create_writer(data_writer_factory_t* factory, const char* url);

/**
 * @method data_writer_factory_destroy
 * 析构并释放data writer工厂对象。
 * @param {data_writer_factory_t*} factory data writer工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_writer_factory_destroy(data_writer_factory_t* factory);

END_C_DECLS

#endif /*TK_DATA_WRITER_FACTORY_H*/
