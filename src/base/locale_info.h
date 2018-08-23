/**
 * File:   locale_info.h
 * Author: AWTK Develop Team
 * Brief:  locale_info
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
#include "base/assets_manager.h"

BEGIN_C_DECLS

/**
 * @class locale_info_t
 * @annotation ["scriptable"]
 * locale_info
 */
typedef struct _locale_info_t {
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

  const asset_info_t* strs;

  emitter_t* emitter;
} locale_info_t;

/**
 * @method locale_info
 * 获取缺省locale_info。
 * @alias locale_info_instance
 * @annotation ["constructor", "scriptable"]
 *
 * @return {locale_info_t*} 返回locale_info对象。
 */
locale_info_t* locale_info(void);

/**
 * @method locale_info_set
 * 设置缺省locale_info。
 * @param {locale_info_t*} locale_info locale_info对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_set(locale_info_t* locale_info);

/**
 * @method locale_info_create
 * 创建locale_info。
 * @annotation ["constructor"]
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {locale_info_t*} 返回locale_info对象。
 */
locale_info_t* locale_info_create(const char* language, const char* country);

/**
 * @method locale_info_init
 * 初始化locale_info。
 * @annotation ["constructor"]
 * @param {locale_info_t*} locale_info locale_info对象。
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {locale_info_t*} 返回locale_info对象。
 */
locale_info_t* locale_info_init(locale_info_t* locale_info, const char* language,
                                const char* country);

/**
 * @method locale_info_tr
 * 翻译字符串。
 * @annotation ["scriptable"]
 * @param {locale_info_t*} locale_info locale_info对象。
 * @param {char*} text 待翻译的文本。
 *
 * @return {char*} 返回翻译之后的字符串。
 */
const char* locale_info_tr(locale_info_t* locale_info, const char* text);

/**
 * @method locale_info_change
 * 设置当前的国家和语言。
 * @annotation ["scriptable"]
 * @param {locale_info_t*} locale_info locale_info对象。
 * @param {char*} language 语言。
 * @param {char*} country 国家或地区。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_change(locale_info_t* locale_info, const char* language, const char* country);

/**
 * @method locale_info_on
 * 注册指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {locale_info_t*} locale_info 控件对象。
 * @param {event_type_t} type 事件类型，目前固定为EVT_LOCALE_CHANGED。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于locale_info_off。
 */
uint32_t locale_info_on(locale_info_t* locale_info, event_type_t type, event_func_t on_event,
                        void* ctx);

/**
 * @method locale_info_off
 * 注销指定事件的处理函数。
 * @annotation ["scriptable"]
 * @param {locale_info_t*} locale_info 控件对象。
 * @param {uint32_t} id locale_info_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_off(locale_info_t* locale_info, uint32_t id);

/**
 * @method locale_info_deinit
 * @param {locale_info_t*} locale_info locale_info对象。
 * 释放全部资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_deinit(locale_info_t* locale_info);

/**
 * @method locale_info_destroy
 * @param {locale_info_t*} locale_info locale_info对象。
 * 释放全部资源并销毁locale_info对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_destroy(locale_info_t* locale_info);

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
