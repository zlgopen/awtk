/**
 * File:   children_layouter_factory.h
 * Author: AWTK Develop Team
 * Brief:  children layouter factory
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

#ifndef TK_CHILDREN_LAYOUTER_FACTORY_H
#define TK_CHILDREN_LAYOUTER_FACTORY_H

#include "tkc/darray.h"
#include "base/children_layouter.h"

BEGIN_C_DECLS

/**
 * @class children_layouter_factory_t
 * 子控件布局算法工厂。
 *
 */
typedef struct _children_layouter_factory_t {
  /*private*/
  darray_t creators;
} children_layouter_factory_t;

/**
 * @method children_layouter_factory
 * 获取缺省的子控件布局算法工厂对象。
 * @annotation ["constructor"]
 * @return {children_layouter_factory_t*} 返回子控件布局算法工厂对象。
 */
children_layouter_factory_t* children_layouter_factory(void);

/**
 * @method children_layouter_factory_set
 * 设置缺省的子控件布局算法工厂对象。
 * @param {children_layouter_factory_t*} factory 子控件布局算法工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_factory_set(children_layouter_factory_t* factory);

/**
 * @method children_layouter_factory_create
 * 创建子控件布局算法工厂对象。
 * @annotation ["constructor"]
 *
 * @return {children_layouter_factory_t*} 返回子控件布局算法工厂对象。
 */
children_layouter_factory_t* children_layouter_factory_create(void);

/**
 * @method children_layouter_factory_register
 * 注册子控件布局算法创建函数。
 * @param {children_layouter_factory_t*} factory layouter工厂对象。
 * @param {const char*} type 子控件布局算法类型。
 * @param {children_layouter_create_t} create 子控件布局算法创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_factory_register(children_layouter_factory_t* factory, const char* type,
                                         children_layouter_create_t create);
/**
 * @method children_layouter_factory_create_layouter
 * 创建指定类型的子控件布局算法对象。
 * @annotation ["constructor"]
 * @param {children_layouter_factory_t*} factory 子控件布局算法工厂对象。
 * @param {const char*} type 类型。
 *
 * @return {children_layouter_t*} 返回子控件布局算法对象。
 */
children_layouter_t* children_layouter_factory_create_layouter(children_layouter_factory_t* factory,
                                                               const char* type);

/**
 * @method children_layouter_factory_destroy
 * 析构并释放子控件布局算法工厂对象。
 * @param {children_layouter_factory_t*} factory 子控件布局算法工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_factory_destroy(children_layouter_factory_t* factory);

children_layouter_t* children_layouter_create(const char* params);

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_FACTORY_H*/
