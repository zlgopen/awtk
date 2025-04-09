/**
 * File:   locale_info_xml.h
 * Author: AWTK Develop Team
 * Brief:  locale_info_xml
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-04-08 Li PeiHuang <lipeihuang@zlg.cn> created
 *
 */

#ifndef TK_LOCALE_INFO_XML_H
#define TK_LOCALE_INFO_XML_H

#include "base/locale_info.h"
#include "conf_io/conf_xml.h"

BEGIN_C_DECLS

/**
 * @class locale_info_xml_t
 * @parent locale_info_t
 * @annotation ["scriptable"]
 * 本地化信息。
 * locale_info_t 的子类。
 * 提供从 xml 文件中获取本地化信息的功能。
 *
 * 注意：fallback_tr2 回调已被设置用于从xml文件中获取本地化信息，不可再重复设置，否则将导致功能失效！
 *
 */
typedef struct _locale_info_xml_t {
  locale_info_t info;

  /*private*/
  conf_doc_t* doc;
  str_t* strings_url;
  str_t* language;
  value_t* text_value;
} locale_info_xml_t;

/**
 * @method locale_info_xml_create
 * 创建locale_info_xml。
 * @annotation ["constructor"]
 * @param {const char*} language 语言。
 * @param {const char*} country 国家或地区。
 *
 * @return {locale_info_t*} 返回locale_info对象。
 */
locale_info_t* locale_info_xml_create(const char* language, const char* country);

/**
 * @method locale_info_xml_set_url
 * 设置本地化文件路径。
 * @param {locale_info_t*} locale_info locale_info 对象。
 * @param {const char*} strings_url 本地化文件路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_xml_set_url(locale_info_t* locale_info, const char* strings_url);

/**
 * @method locale_info_xml_set_assets_manager
 * 设置资源管理器对象。
 * @param {locale_info_t*} locale_info locale_info 对象。
 * @param {assets_manager_t*} assets_manager 资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_xml_set_assets_manager(locale_info_t* locale_info,
                                         assets_manager_t* assets_manager);

/**
 * @method locale_info_xml_destroy
 * 释放全部资源并销毁 locale_info_xml 对象。
 * @param {locale_info_t*} locale_info locale_info 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t locale_info_xml_destroy(locale_info_t* locale_info);

END_C_DECLS

#endif /*TK_LOCALE_INFO_XML_H*/