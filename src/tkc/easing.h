/**
 * File:   easing.h
 * Author: AWTK Develop Team
 * Brief: easing functions.
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EASING_H
#define TK_EASING_H

#include "tkc/darray.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef float_t (*easing_func_t)(float_t k);

typedef struct _easing_name_func_t {
  key_type_value_t* type_name_value;
  easing_func_t easing_func;
} easing_name_func_t;

/**
 * @enum easing_type_t
 * @prefix EASING_
 * @annotation ["scriptable"]
 * 缓动作动画常量定义。
 */
typedef enum _easing_type_t {
  /**
   * @const EASING_LINEAR
   * EASING_LINEAR。
   */
  EASING_LINEAR = 0,
  /**
   * @const EASING_QUADRATIC_IN
   * EASING_QUADRATIC_IN
   */
  EASING_QUADRATIC_IN,
  /**
   * @const EASING_QUADRATIC_OUT
   * EASING_QUADRATIC_OUT
   */
  EASING_QUADRATIC_OUT,
  /**
   * @const EASING_QUADRATIC_INOUT
   * EASING_QUADRATIC_INOUT
   */
  EASING_QUADRATIC_INOUT,
  /**
   * @const EASING_CUBIC_IN
   * EASING_CUBIC_IN
   */
  EASING_CUBIC_IN,
  /**
   * @const EASING_CUBIC_OUT
   * EASING_CUBIC_OUT
   */
  EASING_CUBIC_OUT,
  /**
   * @const EASING_SIN_IN
   * EASING_SIN_IN
   */
  EASING_SIN_IN,
  /**
   * @const EASING_SIN_OUT
   * EASING_SIN_OUT
   */
  EASING_SIN_OUT,
  /**
   * @const EASING_SIN_INOUT
   * EASING_SIN_OUT
   */
  EASING_SIN_INOUT,
  /**
   * @const EASING_POW_IN
   * EASING_POW_IN
   */
  EASING_POW_IN,
  /**
   * @const EASING_POW_OUT
   * EASING_POW_OUT
   */
  EASING_POW_OUT,
  /**
   * @const EASING_POW_INOUT
   * EASING_POW_INOUT
   */
  EASING_POW_INOUT,
  /**
   * @const EASING_CIRCULAR_IN
   * EASING_CIRCULAR_IN
   */
  EASING_CIRCULAR_IN,
  /**
   * @const EASING_CIRCULAR_OUT
   * EASING_CIRCULAR_OUT
   */
  EASING_CIRCULAR_OUT,
  /**
   * @const EASING_CIRCULAR_INOUT
   * EASING_CIRCULAR_INOUT
   */
  EASING_CIRCULAR_INOUT,
  /**
   * @const EASING_ELASTIC_IN
   * EASING_ELASTIC_IN
   */
  EASING_ELASTIC_IN,
  /**
   * @const EASING_ELASTIC_OUT
   * EASING_ELASTIC_OUT
   */
  EASING_ELASTIC_OUT,
  /**
   * @const EASING_ELASTIC_INOUT
   * EASING_ELASTIC_INOUT
   */
  EASING_ELASTIC_INOUT,
  /**
   * @const EASING_BACK_IN
   * EASING_BACK_IN
   */
  EASING_BACK_IN,
  /**
   * @const EASING_BACK_OUT
   * EASING_BACK_OUT
   */
  EASING_BACK_OUT,
  /**
   * @const EASING_BACK_INOUT
   * EASING_BACK_INOUT
   */
  EASING_BACK_INOUT,
  /**
   * @const EASING_BOUNCE_IN
   * EASING_BOUNCE_IN
   */
  EASING_BOUNCE_IN,
  /**
   * @const EASING_BOUNCE_OUT
   * EASING_BOUNCE_OUT
   */
  EASING_BOUNCE_OUT,
  /**
   * @const EASING_BOUNCE_INOUT
   * EASING_BOUNCE_INOUT
   */
  EASING_BOUNCE_INOUT,
  EASING_FUNC_NR
} easing_type_t;

/**
 * @class easing_t
 * @annotation ["fake"]
 * 动画趋势类。
 *
 */

/**
 * @method easing_get
 * 获取对应类型的操作函数。
 * @param {easing_type_t} type 类型。
 *
 * @return {easing_func_t} 返回对应的操作函数地址。
 */
easing_func_t easing_get(easing_type_t type);

/**
 * @method easing_register
 * 注册指定名称的动画趋势。
 * @param {const char*} type_name 类型名称。
 * @param {easing_func_t} easing_func 动画趋势函数。
 *
 * @return {uint32_t} 返回对应类型。
 */
uint32_t easing_register(const char* type_name, easing_func_t easing_func);

/*public for awtk only*/
ret_t easing_init(void);
ret_t easing_deinit(void);
darray_t* easing_name_func_darray(void);

END_C_DECLS

#endif /*TK_EASING_H*/
