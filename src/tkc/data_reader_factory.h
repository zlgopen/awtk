/**
 * File:   data_reader_factory.h
 * Author: AWTK Develop Team
 * Brief:  data reader factory
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

#ifndef TK_DATA_READER_FACTORY_H
#define TK_DATA_READER_FACTORY_H

#include "tkc/darray.h"
#include "tkc/data_reader.h"

BEGIN_C_DECLS

typedef data_reader_t* (*data_reader_create_t)(const char* args);

/**
 * @class data_reader_factory_t
 * data reader工厂。
 *
 */
typedef struct _data_reader_factory_t {
  /*private*/
  darray_t creators;
} data_reader_factory_t;

/**
 * @method data_reader_factory
 * 获取缺省的data reader工厂对象。
 * @annotation ["constructor"]
 * @return {data_reader_factory_t*} 返回data reader工厂对象。
 */
data_reader_factory_t* data_reader_factory(void);

/**
 * @method data_reader_factory_set
 * 设置缺省的data reader工厂对象。
 * @param {data_reader_factory_t*} factory data reader工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_reader_factory_set(data_reader_factory_t* factory);

/**
 * @method data_reader_factory_create
 * 创建data reader工厂对象。
 * @annotation ["constructor"]
 *
 * @return {data_reader_factory_t*} 返回data reader工厂对象。
 */
data_reader_factory_t* data_reader_factory_create(void);

/**
 * @method data_reader_factory_register
 * 注册data reader创建函数。
 * @param {data_reader_factory_t*} factory reader工厂对象。
 * @param {const char*} protocol 协议(如file)。
 * @param {data_reader_create_t} create data reader创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_reader_factory_register(data_reader_factory_t* factory, const char* protocol,
                                   data_reader_create_t create);
/**
 * @method data_reader_factory_create_reader
 * 创建指定类型的data reader对象。
 * @annotation ["constructor"]
 * @param {data_reader_factory_t*} factory data reader工厂对象。
 * @param {const char*} url URL。
 *
 * @return {data_reader_t*} 返回data reader对象。
 */
data_reader_t* data_reader_factory_create_reader(data_reader_factory_t* factory, const char* url);

/**
 * @method data_reader_factory_destroy
 * 析构并释放data reader工厂对象。
 * @param {data_reader_factory_t*} factory data reader工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_reader_factory_destroy(data_reader_factory_t* factory);

END_C_DECLS

#endif /*TK_DATA_READER_FACTORY_H*/
