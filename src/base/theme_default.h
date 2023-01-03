/**
 * File:   theme_default.h
 * Author: AWTK Develop Team
 * Brief:  theme default impl
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THEME_DEFAULT_H
#define TK_THEME_DEFAULT_H

#include "base/theme.h"

BEGIN_C_DECLS

/**
 * @class theme_default_t
 * 窗体样式默认实现。
 * @annotation ["fake"]
 * 负责管理缺省的窗体样式数据，方便实现style\_const。
 *
 */

/**
 * @method theme_default_create
 * 创建窗体样式对象。
 * @annotation ["constructor"]
 * @param {const uint8_t*} data 窗体样式数据。
 *
 * @return {theme_t*} 返回窗体样式对象。
 */
theme_t* theme_default_create(const uint8_t* data);

/**
 * @method theme_default_create_ex
 * 创建窗体样式对象。
 * @annotation ["constructor"]
 * @param {const uint8_t*} data 窗体样式数据。
 * @param {bool_t} need_free_data 销毁时是否释放数据。
 *
 * @return {theme_t*} 返回窗体样式对象。
 */
theme_t* theme_default_create_ex(const uint8_t* data, bool_t need_free_data);

END_C_DECLS

#endif /*TK_THEME_DEFAULT_H*/
