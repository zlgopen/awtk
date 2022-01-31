/**
 * File:   theme.h
 * Author: AWTK Develop Team
 * Brief:  theme interface
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/color.h"
#include "base/gradient.h"
#include "base/widget_consts.h"

BEGIN_C_DECLS

#define THEME_DEFAULT_STYLE_TYPE "style_const"

struct _theme_t;
typedef struct _theme_t theme_t;

typedef const uint8_t* (*theme_find_style_t)(theme_t* theme, const char* widget_type,
                                             const char* name, const char* widget_state);
typedef ret_t (*theme_destroy_t)(theme_t* theme);

typedef ret_t (*theme_set_theme_data_t)(theme_t* theme, const uint8_t* data);
typedef const char* (*theme_get_style_type_t)(theme_t* theme);

/**
 * @class theme_t
 * @annotation ["scriptable"]
 * 窗体样式。
 *
 * 负责管理缺省的窗体样式数据，方便实现style\_const。
 *
 */
struct _theme_t {
  const uint8_t* data;

  theme_find_style_t find_style;
  theme_destroy_t theme_destroy;

  theme_set_theme_data_t set_style_data;
  theme_get_style_type_t get_style_type;
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
 * @method theme_default_create
 * 创建窗体样式对象。
 * @annotation ["constructor"]
 * @param {const uint8_t*} data 窗体样式数据。
 *
 * @return {theme_t*} 返回窗体样式对象。
 */
theme_t* theme_default_create(const uint8_t* data);

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

/*data related*/
int32_t style_data_get_int(const uint8_t* s, const char* name, int32_t defval);
uint32_t style_data_get_uint(const uint8_t* s, const char* name, uint32_t defval);
color_t style_data_get_color(const uint8_t* s, const char* name, color_t defval);
gradient_t* style_data_get_gradient(const uint8_t* s, const char* name, gradient_t* gradient);
const char* style_data_get_str(const uint8_t* s, const char* name, const char* defval);

/*public for tools only*/
#define THEME_MAGIC 0xFAFBFCFD
#define TK_DEFAULT_STYLE "default"

#pragma pack(push, 1)

typedef struct _theme_header_t {
  uint32_t magic;
  uint32_t version;
  uint32_t nr;
} theme_header_t;

typedef struct _theme_item_t {
  uint32_t offset;
  char state[TK_NAME_LEN + 1];
  char name[TK_NAME_LEN + 1];
  char widget_type[TK_NAME_LEN + 1];
} theme_item_t;

typedef struct _style_name_value_header_t {
  uint32_t type : 8;
  uint32_t name_size : 8;
  uint32_t value_size : 16;
} style_name_value_header_t;

typedef struct _style_name_value_t {
  uint32_t type : 8;
  uint32_t name_size : 8;
  uint32_t value_size : 16;
  const char name[4];
} style_name_value_t;

#pragma pack(pop)

#define STYLE_NAME_SIZE_MAX 255
#define STYLE_VALUE_SIZE_MAX 1023

END_C_DECLS

#endif /*TK_THEME_H*/
