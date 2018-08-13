/**
 * File:   tklocale.h
 * Author: AWTK Develop Team
 * Brief:  tklocale
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LOCALE_H
#define TK_LOCALE_H

#include "base/emitter.h"
#include "base/resource_manager.h"

BEGIN_C_DECLS

/**
 * @class tklocale_t
 * @annotation ["scriptable"]
 * tklocale
 */
typedef struct _tklocale_t {
  /**
   * @property {char*} country;
   * @annotation ["readable"]
   * 国家或地区。如：CN
   */
  char country[3];
  /**
   * @property {char*} language
   * @annotation ["readable"]
   * 语言。如：zh
   */
  char language[3];

  const resource_info_t* strs;

  emitter_t* emitter;
} tklocale_t;

/**
 * @method tklocale
 * 获取缺省tklocale。
 * @alias tklocale_instance
 * @annotation ["constructor", "scriptable"]
 *
 * @return {tklocale_t*} 返回tklocale对象。
 */
tklocale_t* tklocale(void);

/**
 * @method tklocale_set
 * 设置缺省tklocale。
 * @param {tklocale_t*} tklocale tklocale对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tklocale_set(tklocale_t* tklocale);

/**
 * @method tklocale_create
 * 创建tklocale。
 * @annotation ["constructor"]
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {tklocale_t*} 返回tklocale对象。
 */
tklocale_t* tklocale_create(const char* language, const char* country);

/**
 * @method tklocale_init
 * 初始化tklocale。
 * @annotation ["constructor"]
 * @param {tklocale_t*} tklocale tklocale对象。
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {tklocale_t*} 返回tklocale对象。
 */
tklocale_t* tklocale_init(tklocale_t* tklocale, const char* language, const char* country);

/**
 * @method tklocale_tr
 * 翻译字符串。
 * @annotation ["scriptable"]
 * @param {tklocale_t*} tklocale tklocale对象。
 * @param {char*} text 待翻译的文本。
 *
 * @return {char*} 返回翻译之后的字符串。
 */
const char* tklocale_tr(tklocale_t* tklocale, const char* text);

/**
 * @method tklocale_change
 * 设置当前的国家和语言。
 * @annotation ["scriptable"]
 * @param {tklocale_t*} tklocale tklocale对象。
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tklocale_change(tklocale_t* tklocale, const char* language, const char* country);

/**
 * @method tklocale_on
 * 注册指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {tklocale_t*} tklocale 控件对象。
 * @param {event_type_t} type 事件类型，目前固定为EVT_LOCALE_CHANGED。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于tklocale_off。
 */
uint32_t tklocale_on(tklocale_t* tklocale, event_type_t type, event_func_t on_event, void* ctx);

/**
 * @method tklocale_off
 * 注销指定事件的处理函数。
 * @annotation ["scriptable"]
 * @param {tklocale_t*} tklocale 控件对象。
 * @param {uint32_t} id tklocale_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tklocale_off(tklocale_t* tklocale, uint32_t id);

/**
 * @method tklocale_deinit
 * @param {tklocale_t*} tklocale tklocale对象。
 * 释放全部资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tklocale_deinit(tklocale_t* tklocale);

/**
 * @method tklocale_destroy
 * @param {tklocale_t*} tklocale tklocale对象。
 * 释放全部资源并销毁tklocale对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tklocale_destroy(tklocale_t* tklocale);

typedef struct _str_pair_t {
  uint32_t key;
  uint32_t value;
} str_pair_t;

typedef struct _str_table_t {
  uint16_t version;
  uint16_t nr;
  str_pair_t strs[1];
} str_table_t;

const char* str_table_lookup(const str_table_t* table, const char* key);

END_C_DECLS

#endif /*TK_LOCALE_H*/
