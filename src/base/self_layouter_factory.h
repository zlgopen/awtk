/**
 * File:   self_layouter_factory.h
 * Author: AWTK Develop Team
 * Brief:  self layouter factory
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SELF_LAYOUTER_FACTORY_H
#define TK_SELF_LAYOUTER_FACTORY_H

#include "tkc/darray.h"
#include "base/self_layouter.h"

BEGIN_C_DECLS

/**
 * @class self_layouter_factory_t
 * 控件自身布局算法工厂。
 *
 */
typedef struct _self_layouter_factory_t {
  /*private*/
  darray_t creators;
} self_layouter_factory_t;

/**
 * @method self_layouter_factory
 * 获取缺省的控件自身布局算法工厂对象。
 * @annotation ["constructor"]
 * @return {self_layouter_factory_t*} 返回控件自身布局算法工厂对象。
 */
self_layouter_factory_t* self_layouter_factory(void);

/**
 * @method self_layouter_factory_set
 * 设置缺省的控件自身布局算法工厂对象。
 * @param {self_layouter_factory_t*} factory 控件自身布局算法工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_factory_set(self_layouter_factory_t* factory);

/**
 * @method self_layouter_factory_create
 * 创建控件自身布局算法工厂对象。
 * @annotation ["constructor"]
 *
 * @return {self_layouter_factory_t*} 返回控件自身布局算法工厂对象。
 */
self_layouter_factory_t* self_layouter_factory_create(void);

/**
 * @method self_layouter_factory_register
 * 注册控件自身布局算法创建函数。
 * @param {self_layouter_factory_t*} factory layouter工厂对象。
 * @param {const char*} type 控件自身布局算法类型。
 * @param {self_layouter_create_t} create 控件自身布局算法创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_factory_register(self_layouter_factory_t* factory, const char* type,
                                     self_layouter_create_t create);
/**
 * @method self_layouter_factory_create_layouter
 * 创建指定类型的控件自身布局算法对象。
 * @annotation ["constructor"]
 * @param {self_layouter_factory_t*} factory 控件自身布局算法工厂对象。
 * @param {const char*} type 类型。
 *
 * @return {self_layouter_t*} 返回控件自身布局算法对象。
 */
self_layouter_t* self_layouter_factory_create_layouter(self_layouter_factory_t* factory,
                                                       const char* type);

/**
 * @method self_layouter_factory_destroy
 * 析构并释放控件自身布局算法工厂对象。
 * @param {self_layouter_factory_t*} factory 控件自身布局算法工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_factory_destroy(self_layouter_factory_t* factory);

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_FACTORY_H*/
