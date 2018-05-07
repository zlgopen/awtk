/**
 * File:   locale.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  locale
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * @class locale_t
 * @scriptable
 * locale
 */
typedef struct _locale_t {
  /**
   * @property {char*} country;
   * @readonly
   * 国家或地区。如：CN
   */
  char country[3];
  /**
   * @property {char*} language
   * @readonly
   * 语言。如：zh
   */
  char language[3];

  const resource_info_t* strs;
  emitter_t emitter;
} tklocale_t;

/**
 * @method locale
 * 获取缺省locale。
 * @constructor
 *
 * @return {tklocale_t*} 返回locale对象。
 */
tklocale_t* locale(void);

/**
 * @method locale_set
 * 设置缺省locale。
 * @param {tklocale_t*} locale locale对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_set(tklocale_t* locale);

/**
 * @method locale_create
 * 创建locale。
 * @constructor
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {tklocale_t*} 返回locale对象。
 */
tklocale_t* locale_create(const char* language, const char* country);

/**
 * @method locale_init
 * 初始化locale。
 * @constructor
 * @param {tklocale_t*} locale locale对象。
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {tklocale_t*} 返回locale对象。
 */
tklocale_t* locale_init(tklocale_t* locale, const char* language, const char* country);

/**
 * @method locale_tr
 * 翻译字符串。
 * @param {tklocale_t*} locale locale对象。
 * @param {char*} text 待翻译的文本。
 *
 * @return {char*} 返回翻译之后的字符串。
 */
const char* locale_tr(tklocale_t* locale, const char* text);

/**
 * @method locale_change
 * 设置当前的国家和语言。
 * @param {tklocale_t*} locale locale对象。
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_change(tklocale_t* locale, const char* language, const char* country);

/**
 * @method locale_on
 * 注册指定事件的处理函数。
 * @scriptable custom
 * @param {tklocale_t*} locale 控件对象。
 * @param {event_type_t} type 事件类型，目前固定为EVT_LOCALE_CHANGED。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于locale_off。
 */
uint32_t locale_on(tklocale_t* locale, event_type_t type, event_func_t on_event, void* ctx);

/**
 * @method locale_off
 * 注销指定事件的处理函数。
 * @scriptable custom
 * @param {tklocale_t*} locale 控件对象。
 * @param {uint32_t} id locale_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_off(tklocale_t* locale, uint32_t id);

/**
 * @method locale_deinit
 * @param {tklocale_t*} locale locale对象。
 * 释放全部资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_deinit(tklocale_t* locale);

/**
 * @method locale_destroy
 * @param {tklocale_t*} locale locale对象。
 * 释放全部资源并销毁locale对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_destroy(tklocale_t* locale);

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
