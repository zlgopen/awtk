/**
 * File:   style.h
 * Author: AWTK Develop Team
 * Brief:  style interface
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_STYLE_H
#define TK_STYLE_H

#include "base/color.h"
#include "base/widget_consts.h"

BEGIN_C_DECLS

/**
 * @enum style_type_t
 * @prefix STYLE
 * 类型常量定义。
 */
typedef enum _style_id_t {
  /**
   * @const STYLE_ID_BG_COLOR
   * 背景颜色。
   */
  STYLE_ID_BG_COLOR = 1,
  /**
   * @const STYLE_ID_BG_COLOR
   * 前景颜色。
   */
  STYLE_ID_FG_COLOR,
  /**
   * @const STYLE_ID_FONT_NAME
   * 字体名称。
   */
  STYLE_ID_FONT_NAME,
  /**
   * @const STYLE_ID_FONT_SIZE
   * 字体大小。
   */
  STYLE_ID_FONT_SIZE,
  /**
   * @const STYLE_ID_FONT_STYLE
   * 字体风格(粗体、斜体等)。
   */
  STYLE_ID_FONT_STYLE,
  /**
   * @const STYLE_ID_TEXT_COLOR
   * 文本颜色。
   */
  STYLE_ID_TEXT_COLOR,
  /**
   * @const STYLE_ID_TIPS_TEXT_COLOR
   * 提示文本颜色。
   */
  STYLE_ID_TIPS_TEXT_COLOR,
  /**
   * @const STYLE_ID_TEXT_ALIGN_H
   * 文本水平对齐的方式。
   */
  STYLE_ID_TEXT_ALIGN_H,
  /**
   * @const STYLE_ID_TEXT_ALIGN_V
   * 文本垂直对齐的方式。
   */
  STYLE_ID_TEXT_ALIGN_V,
  /**
   * @const STYLE_ID_BORDER_COLOR
   * 边框颜色。
   */
  STYLE_ID_BORDER_COLOR,
  /**
   * @const STYLE_ID_BORDER
   * 边框类型。
   */
  STYLE_ID_BORDER,
  /**
   * @const STYLE_ID_BG_IMAGE
   * 图片的名称。
   */
  STYLE_ID_BG_IMAGE,
  /**
   * @const STYLE_ID_BG_IMAGE_DRAW_TYPE
   * 图片的显示方式。
   */
  STYLE_ID_BG_IMAGE_DRAW_TYPE,
  /**
   * @const STYLE_ID_ICON
   * 图标的名称。
   */
  STYLE_ID_ICON,
  /**
   * @const STYLE_ID_FG_IMAGE
   * 图片的名称。
   */
  STYLE_ID_FG_IMAGE,
  /**
   * @const STYLE_ID_FG_IMAGE_DRAW_TYPE
   * 图片的显示方式。
   */
  STYLE_ID_FG_IMAGE_DRAW_TYPE,
  /**
   * @const STYLE_ID_MARGIN
   * 边距。
   */
  STYLE_ID_MARGIN,
  /**
   * @const STYLE_ID_ICON_AT
   * 图标的位置。
   */
  STYLE_ID_ICON_AT
} style_id_t;

/*遍历的回调函数*/
typedef ret_t (*tk_on_style_item_t)(void* ctx, uint32_t widget_state, style_id_t id,
                                    const char* val);

struct _style_t;
typedef struct _style_t style_t;

typedef ret_t (*style_notify_widget_state_changed_t)(style_t* s, widget_t* widget);

typedef bool_t (*style_is_valid_t)(style_t* s);
typedef int32_t (*style_get_int_t)(style_t* s, style_id_t id, int32_t defval);
typedef color_t (*style_get_color_t)(style_t* s, style_id_t id, color_t defval);
typedef const char* (*style_get_str_t)(style_t* s, style_id_t id, const char* defval);

typedef const char* (*style_get_name_t)(style_t* s);
typedef ret_t (*style_set_name_t)(style_t* s, const char* name);
typedef ret_t (*style_set_int_t)(style_t* s, widget_state_t state, style_id_t id, uint32_t val);
typedef ret_t (*style_set_color_t)(style_t* s, widget_state_t state, style_id_t id, color_t val);
typedef ret_t (*style_set_str_t)(style_t* s, widget_state_t state, style_id_t id, const char* val);

typedef ret_t (*style_foreach_t)(style_t* s, tk_on_style_item_t on_style_item, void* ctx);
typedef ret_t (*style_destroy_t)(style_t* s);

typedef struct _style_vtable_t {
  style_notify_widget_state_changed_t notify_widget_state_changed;

  style_is_valid_t is_valid;
  style_get_int_t get_int;
  style_get_str_t get_str;
  style_get_color_t get_color;

  style_set_name_t set_name;
  style_get_name_t get_name;

  style_set_int_t set_int;
  style_set_color_t set_color;
  style_set_str_t set_str;

  style_foreach_t foreach;

  style_destroy_t destroy;
} style_vtable_t;

/**
 * @class style_t
 * @annotation ["scriptable"]
 * 控件风格。
 */
struct _style_t {
  const style_vtable_t* vt;
};

/**
 * @method style_notify_widget_state_changed
 * widget状态改变时，通知style更新数据。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_notify_widget_state_changed(style_t* s, widget_t* widget);

/**
 * @method style_is_valid
 * 检查style对象是否有效
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 *
 * @return {bool_t} 返回是否有效。
 */
bool_t style_is_valid(style_t* s);

/**
 * @method style_get_int
 * 获取指定id的整数格式的值。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {style_id_t} id 属性ID。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回整数格式的值。
 */
int32_t style_get_int(style_t* s, style_id_t id, int32_t defval);

/**
 * @method style_get_color
 * 获取指定id的颜色值。
 * @param {style_t*} s style对象。
 * @param {style_id_t} id 属性ID。
 * @param {color_t} defval 缺省值。
 *
 * @return {color_t} 返回颜色值。
 */
color_t style_get_color(style_t* s, style_id_t id, color_t defval);

/**
 * @method style_get_str
 * 获取指定id的字符串格式的值。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {style_id_t} id 属性ID。
 * @param {const char*} defval 缺省值。
 *
 * @return {const char*} 返回字符串格式的值。
 */
const char* style_get_str(style_t* s, style_id_t id, const char* defval);

/**
 * @method style_get_name
 * 获取style的名称(子类可以选择是否实现)。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 *
 * @return {const char*} style的名称。
 */
const char* style_get_name(style_t* s);

/**
 * @method style_set_name
 * 设置style的名称(子类可以选择是否实现)。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_set_name(style_t* s, const char* name);

/**
 * @method style_set_int
 * 设置指定id整数格式的值(子类可以选择是否实现)。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {widget_state_t} state 控件状态。
 * @param {style_id_t} id 属性ID。
 * @param {int32_t} val 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_set_int(style_t* s, widget_state_t state, style_id_t id, uint32_t val);

/**
 * @method style_set_color
 * 设置指定id的颜色值(子类可以选择是否实现)。
 * @param {style_t*} s style对象。
 * @param {widget_state_t} state 控件状态。
 * @param {style_id_t} id 属性ID。
 * @param {color_t} val 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_set_color(style_t* s, widget_state_t state, style_id_t id, color_t val);

/**
 * @method style_set_str
 * 设置指定id字符串的值(子类可以选择是否实现)。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {widget_state_t} state 控件状态。
 * @param {style_id_t} id 属性ID。
 * @param {const char*} val 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_set_str(style_t* s, widget_state_t state, style_id_t id, const char* val);

/**
 * @method style_foreach
 * 遍历。对每项调用回调函数on_style_item。
 * @param {style_t*} s style对象。
 * @param {tk_on_style_item_t} on_style_item 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_foreach(style_t* s, tk_on_style_item_t on_style_item, void* ctx);

/**
 * @method style_destroy
 * 销毁style对象
 * @param {style_t*} s style对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_destroy(style_t* s);

END_C_DECLS

#endif /*TK_STYLE_H*/
