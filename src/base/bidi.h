/**
 * File:   bidi.h
 * Author: AWTK Develop Team
 * Brief:  Unicode Bidirectional Algorithm.
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
 * 2020-07-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BIDI_H
#define TK_BIDI_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @enum bidi_type_t 
 * @annotation ["scriptable"]
 * @prefix BIDI_TYPE_
 * bidi 类型常量定义。
 */
typedef enum _bidi_type_t {
  /**
   * @const BIDI_TYPE_AUTO
   * 自动检查。
   */
  BIDI_TYPE_AUTO = 0,
  /**
   * @const BIDI_TYPE_LTR
   * Left-To-Right letter。
   */
  BIDI_TYPE_LTR,
  /**
   * @const BIDI_TYPE_RTL
   * Right-To-Left letter。
   */
  BIDI_TYPE_RTL,
  /**
   * @const BIDI_TYPE_LRO
   * Left-To-Right letter Override。
   */
  BIDI_TYPE_LRO,
  /**
   * @const BIDI_TYPE_RLO
   * Right-To-Left letter Override。
   */
  BIDI_TYPE_RLO,
  /**
   * @const BIDI_TYPE_WLTR
   * Weak Left To Right paragraph。
   */
  BIDI_TYPE_WLTR,
  /**
   * @const BIDI_TYPE_WRTL
   * Weak Right To Left paragraph。
   */
  BIDI_TYPE_WRTL
} bidi_type_t;

/**
 * @class bidi_t
 * Unicode Bidirectional Algorithm.
 * 
 */
typedef struct _bidi_t {
  /**
   * @property {wchar_t*} vis_strx
   * @annotation ["readable"]
   * 用于显示的字符串(存放log2vis的结果)。
   */
  wchar_t* vis_str;
  /**
   * @property {uint32_t} vis_str_size
   * @annotation ["readable"]
   * 用于显示的字符串长度(存放log2vis的结果)。
   */
  uint32_t vis_str_size;
  /**
   * @property {bidi_type_t} request_type
   * @annotation ["readable"]
   * 请求的类型。
   */
  bidi_type_t request_type;
  /**
   * @property {bidi_type_t} resolved_type
   * @annotation ["readable"]
   * 实际的类型。
   */
  bidi_type_t resolved_type;
  /**
   * @property {int32_t*} positions_L_to_V
   * @annotation ["readable"]
   * logical位置与visual位置的映射。
   */
  int32_t* positions_L_to_V;
  /**
   * @property {int32_t*} positions_V_to_L
   * @annotation ["readable"]
   * visual位置与logical位置的映射。
   */
  int32_t* positions_V_to_L;
  /*private*/
  bool_t alloc_l2v;
  bool_t alloc_v2l;
  wchar_t vis_str_static[128];
} bidi_t;

#ifdef WITH_TEXT_BIDI
/**
 * @method bidi_init
 * 初始化bidi对象。
 * @annotation ["deconstructor"]
 * @param {bidi_t*} bidi bidi对象。
 * @param {bool_t} alloc_l2v 是否为positions_L_to_V分配空间。 
 * @param {bool_t} alloc_v2l 是否为positions_V_to_L分配空间。 
 * @param {bidi_type_t} type 类型。 
 *
 * @return {bidi_t*} 返回bidi对象。
 */
bidi_t* bidi_init(bidi_t* bidi, bool_t alloc_l2v, bool_t alloc_v2l, bidi_type_t type);

/**
 * @method bidi_type_from_name
 * 将bidi类型的名称转换成类型。
 * @annotation ["static"]
 * @param {const char*} name 类型名称(取值：rtl,ltr,auto,wrtl,wltr,lro,rlo)。 
 *
 * @return {bidi_t*} 返回bidi对象。
 */
bidi_type_t bidi_type_from_name(const char* name);

/**
 * @method bidi_log2vis
 * 将字符串转成用于显示的字符串，输出结果放在bidi->vis_str中。
 * 
 * @param {bidi_t*} bidi bidi对象。
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} size 字符串的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bidi_log2vis(bidi_t* bidi, const wchar_t* str, uint32_t size);

/**
 * @method bidi_deinit
 * 释放bidi对象相关资源。
 * @annotation ["deconstructor"]
 * @param {bidi_t*} bidi bidi对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bidi_deinit(bidi_t* bidi);
#else
static inline bidi_t* bidi_init(bidi_t* bidi, bool_t alloc_l2v, bool_t alloc_v2l,
                                bidi_type_t type) {
  return_value_if_fail(bidi != NULL, NULL);
  memset(bidi, 0x00, sizeof(bidi_t));

  return bidi;
}

static inline ret_t bidi_log2vis(bidi_t* bidi, const wchar_t* str, uint32_t size) {
  return_value_if_fail(bidi != NULL, RET_BAD_PARAMS);
  bidi->vis_str = (wchar_t*)str;
  bidi->vis_str_size = size;

  return RET_OK;
}

static bidi_type_t bidi_type_from_name(const char* name) {
  return BIDI_TYPE_AUTO;
}

static inline ret_t bidi_deinit(bidi_t* bidi) {
  return_value_if_fail(bidi != NULL, RET_BAD_PARAMS);
  return RET_OK;
}
#endif /*WITH_TEXT_BIDI*/

END_C_DECLS

#endif /*TK_BIDI_H*/
