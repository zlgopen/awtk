/**
 * File:   shortcut.h
 * Author: AWTK Develop Team
 * Brief:  shortcut
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
 * 2019-05-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SHORTCUT_H
#define TK_SHORTCUT_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class shortcut_t
 * 快捷键。
 */
typedef struct _shortcut_t {
  /**
   * @property {bool_t} is_valid
   * @annotation ["readable"]
   * 是否有效。
   */
  uint32_t is_valid : 1;
  /**
   * @property {uint32_t} key
   * @annotation ["readable"]
   * 键值。
   */
  uint32_t key;
  /**
   * @property {bool_t} alt
   * @annotation ["readable"]
   * alt键是否按下。
   */
  uint32_t alt : 1;
  /**
   * @property {bool_t} lalt
   * @annotation ["readable"]
   * left alt键是否按下。
   */
  uint32_t lalt : 1;
  /**
   * @property {bool_t} ralt
   * @annotation ["readable"]
   * right alt键是否按下。
   */
  uint32_t ralt : 1;
  /**
   * @property {bool_t} ctrl
   * @annotation ["readable"]
   * ctrl键是否按下。
   */
  uint32_t ctrl : 1;
  /**
   * @property {bool_t} lctrl
   * @annotation ["readable"]
   * left ctrl键是否按下。
   */
  uint32_t lctrl : 1;
  /**
   * @property {bool_t} rctrl
   * @annotation ["readable"]
   * right ctrl键是否按下。
   */
  uint32_t rctrl : 1;
  /**
   * @property {bool_t} shift
   * @annotation ["readable"]
   * shift键是否按下。
   */
  uint32_t shift : 1;
  /**
   * @property {bool_t} lshift
   * @annotation ["readable"]
   * left shift键是否按下。
   */
  uint32_t lshift : 1;
  /**
   * @property {bool_t} rshift
   * @annotation ["readable"]
   * right shift键是否按下。
   */
  uint32_t rshift : 1;
  /**
   * @property {bool_t} cmd
   * @annotation ["readable"]
   * cmd/win键是否按下。
   */
  uint32_t cmd : 1;
  /**
   * @property {bool_t} menu
   * @annotation ["readable"]
   * menu键是否按下。
   */
  uint32_t menu : 1;
} shortcut_t;

/**
 * @method shortcut_init
 * 初始化快捷键对象。
 * @annotation ["constructor"]
 *
 * @return {shortcut_t*} 返回快捷键对象。
 */
shortcut_t* shortcut_init(shortcut_t* shortcut, uint32_t key);

/**
 * @method shortcut_init_with_str
 * 初始化快捷键对象。
 * @param {shortcut_t*} shortcut 快捷键对象。
 * @annotation ["constructor"]
 *
 * @return {shortcut_t*} 返回快捷键对象。
 */
shortcut_t* shortcut_init_with_str(shortcut_t* shortcut, const char* str);

/**
 * @method shortcut_equal
 * 比较两个快捷键对象。
 * @param {shortcut_t*} shortcut1 快捷键对象1。
 * @param {shortcut_t*} shortcut2 快捷键对象2。
 *
 * @return {bool_t} 返回TRUE表示相同，否则表示不同。
 */
bool_t shortcut_equal(shortcut_t* shortcut1, shortcut_t* shortcut2);

/**
 * @method shortcut_match
 * 检查事件与快捷键是否匹配。
 * @param {shortcut_t*} filter 过滤器快捷键对象。
 * @param {shortcut_t*} event 事件快捷键对象。
 *
 * @return {bool_t} 返回TRUE表示匹配，否则表示不匹配。
 */
bool_t shortcut_match(shortcut_t* filter, shortcut_t* event);

END_C_DECLS

#endif /*TK_SHORTCUT_H*/
