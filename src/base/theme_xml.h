/**
 * File:   theme_xml.h
 * Author: AWTK Develop Team
 * Brief:  load theme data from xml
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
 * 2022-05-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THEME_XML_H
#define TK_THEME_XML_H

#include "base/theme.h"

BEGIN_C_DECLS

/**
 * @class theme_xml_t
 * XML格式的窗体样式。
 * @annotation ["fake"]
 */

/**
 * @method theme_xml_create
 * 创建窗体样式对象。
 * @annotation ["constructor"]
 * @param {const char*} xml XML格式窗体样式数据。
 *
 * @return {theme_t*} 返回窗体样式对象。
 */
theme_t* theme_xml_create(const char* xml);

/**
 * @method theme_xml_gen
 * 生成二进制的数据。
 * @param {const char*} xml XML格式窗体样式数据。
 * @param {uint32_t*} size 用于返回数据长度。
 *
 * @return {uint8_t*} 返回二进制的数据。
 */
uint8_t* theme_xml_gen(const char* xml, uint32_t* size);

END_C_DECLS

#endif /*TK_THEME_XML_H*/
