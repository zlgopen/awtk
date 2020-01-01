/**
 * File:   theme.h
 * Author: AWTK Develop Team
 * Brief:  theme interface
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THEME_H
#define TK_THEME_H

#include "tkc/color.h"
#include "base/widget_consts.h"

BEGIN_C_DECLS

/**
 * @class theme_t
 * @annotation ["scriptable"]
 * 主题。
 *
 * 负责管理缺省的主题数据，方便实现style\_const。
 *
 */
typedef struct _theme_t {
  const uint8_t* data;
} theme_t;

/**
 * @method theme
 * 获取缺省的主题对象。
 * @alias theme_instance
 * @annotation ["constructor", "scriptable"]
 * @return {theme_t*} 返回主题对象。
 */
theme_t* theme(void);

/**
 * @method theme_set
 * 设置缺省的主题对象。
 * @param {theme_t*} theme 主题对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_set(theme_t* theme);

/**
 * @method theme_create
 * 创建主题对象。
 * @annotation ["constructor"]
 * @param {const uint8_t*} data 主题数据。
 *
 * @return {theme_t*} 返回主题对象。
 */
theme_t* theme_create(const uint8_t* data);

/**
 * @method theme_init
 * 初始化主题对象。
 * @annotation ["constructor"]
 * @param {theme_t*} theme 主题对象。
 * @param {const uint8_t*} data 主题数据。
 *
 * @return {theme_t*} 返回主题对象。
 */
theme_t* theme_init(theme_t* theme, const uint8_t* data);

/**
 * @method theme_find_style
 * 查找满足条件的style。
 * @param {theme_t*} data 主题对象。
 * @param {const char*} widget_type 控件的类型名。
 * @param {const char*} name style的名称。
 * @param {const char*} widget_state 控件的状态。
 *
 * @return {theme_t*} 返回主题对象。
 */
const uint8_t* theme_find_style(theme_t* t, const char* widget_type, const char* name,
                                const char* widget_state);
/**
 * @method theme_deinit
 * 析构主题对象。
 * @param {theme_t*} theme 主题对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_deinit(theme_t* theme);

/**
 * @method theme_destroy
 * 析构并释放主题对象。
 * @param {theme_t*} theme 主题对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t theme_destroy(theme_t* theme);

/*data related*/
uint32_t style_data_get_int(const uint8_t* s, const char* name, uint32_t defval);
color_t style_data_get_color(const uint8_t* s, const char* name, color_t defval);
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
