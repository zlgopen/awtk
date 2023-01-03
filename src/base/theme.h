/**
 * File:   theme.h
 * Author: AWTK Develop Team
 * Brief:  theme interface
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

#ifndef TK_THEME_H
#define TK_THEME_H

#include "base/theme_data.h"

BEGIN_C_DECLS

struct _theme_t;
typedef struct _theme_t theme_t;

typedef ret_t (*theme_on_data_t)(void* ctx, const char* widget_type, const char* name,
                                 const char* widget_state, const uint8_t* data);

typedef ret_t (*theme_foreach_t)(theme_t* theme, theme_on_data_t on_data, void* ctx);
typedef const uint8_t* (*theme_find_style_t)(theme_t* theme, const char* widget_type,
                                             const char* name, const char* widget_state);
typedef ret_t (*theme_set_stype_data_t)(theme_t* theme, const uint8_t* data);
typedef const char* (*theme_get_style_type_t)(theme_t* theme);
typedef ret_t (*theme_destroy_t)(theme_t* theme);

/**
 * @class theme_t
 * @annotation ["scriptable"]
 * 窗体样式。
 *
 */
struct _theme_t {
  const uint8_t* data;
  bool_t need_free_data;

  theme_foreach_t foreach;
  theme_find_style_t find_style;
  theme_set_stype_data_t set_style_data;
  theme_get_style_type_t get_style_type;
  theme_destroy_t theme_destroy;
};

/**
 * @method theme
 * 获取缺省的窗体样式对象。
 * @alias theme_instance
 * @annotation ["constructor", "scriptable"]
 * @return {theme_t*} 返回窗体样式对象。
 */
theme_t* theme(void);

/**
 * @method theme_set
 * 设置缺省的窗体样式对象。
 * @param {theme_t*} theme 窗体样式对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_set(theme_t* theme);

/**
 * @method theme_foreach
 * 遍历全部数据。
 * @param {theme_t*} theme 窗体样式对象。
 * @param {theme_on_data_t} on_data 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_foreach(theme_t* theme, theme_on_data_t on_data, void* ctx);

/**
 * @method theme_find_style
 * 查找满足条件的style。
 * @param {theme_t*} theme 窗体样式对象。
 * @param {const char*} widget_type 控件的类型名。
 * @param {const char*} name style的名称。
 * @param {const char*} widget_state 控件的状态。
 *
 * @return {const uint8_t*} 返回风格对象数据。
 */
const uint8_t* theme_find_style(theme_t* theme, const char* widget_type, const char* name,
                                const char* widget_state);

/**
 * @method theme_get_style_type
 * 获取窗体样式的风格类型。
 * @param {theme_t*} theme 窗体样式对象。
 *
 * @return {const char*} 返回风格类型。
 */
const char* theme_get_style_type(theme_t* theme);

/**
 * @method theme_set_theme_data
 * 设置窗体样式的数据。
 * @param {theme_t*} theme 窗体样式对象。
 * @param {const uint8_t*} data 窗体样式数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_set_theme_data(theme_t* theme, const uint8_t* data);

/**
 * @method theme_destroy
 * 析构并释放窗体样式对象。
 * @param {theme_t*} theme 窗体样式对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_destroy(theme_t* theme);

/**
 * @method theme_load_from_data
 * 加载窗体样式对象。
 * @annotation ["constructor"]
 * @param {const char*} name 资源名。
 * @param {const uint8_t*} data 窗体样式数据。
 * @param {uint32_t} size 资源长度。
 *
 * @return {theme_t*} 返回窗体样式对象。
 */
theme_t* theme_load_from_data(const char* name, const uint8_t* data, uint32_t size);

#define TK_DEFAULT_STYLE "default"
#define THEME_DEFAULT_STYLE_TYPE "style_const"

END_C_DECLS

#endif /*TK_THEME_H*/
