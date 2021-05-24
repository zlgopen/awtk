/**
 * File:   style.h
 * Author: AWTK Develop Team
 * Brief:  style interface
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/color.h"
#include "base/theme.h"
#include "base/widget_consts.h"

BEGIN_C_DECLS

/**
 * @enum style_id_t
 * @prefix STYLE_ID_
 * @annotation ["scriptable", "string"]
 * style常量定义。
 */
/**
 * @const STYLE_ID_BG_COLOR
 * 背景颜色。
 */
#define STYLE_ID_BG_COLOR "bg_color"

/**
 * @const STYLE_ID_FG_COLOR
 * 前景颜色。
 */
#define STYLE_ID_FG_COLOR "fg_color"

/**
 * @const STYLE_ID_MASK_COLOR
 * 蒙版颜色。
 */
#define STYLE_ID_MASK_COLOR "mask_color"

/**
 * @const STYLE_ID_FONT_NAME
 * 字体名称。
 */
#define STYLE_ID_FONT_NAME "font_name"
/**
 * @const STYLE_ID_FONT_SIZE
 * 字体大小。
 */
#define STYLE_ID_FONT_SIZE "font_size"

/**
 * @const STYLE_ID_FONT_STYLE
 * 字体风格(粗体、斜体等)。
 */
#define STYLE_ID_FONT_STYLE "font_style"

/**
 * @const STYLE_ID_TEXT_COLOR
 * 文本颜色。
 */
#define STYLE_ID_TEXT_COLOR "text_color"

/**
 * @const STYLE_ID_HIGHLIGHT_FONT_NAME
 * 高亮文本的字体名称。
 */
#define STYLE_ID_HIGHLIGHT_FONT_NAME "highlight_font_name"

/**
 * @const STYLE_ID_HIGHLIGHT_FONT_SIZE
 * 高亮文本的字体大小。
 */
#define STYLE_ID_HIGHLIGHT_FONT_SIZE "highlight_font_size"

/**
 * @const STYLE_ID_HIGHLIGHT_TEXT_COLOR
 * 高亮文本的文本颜色。
 */
#define STYLE_ID_HIGHLIGHT_TEXT_COLOR "highlight_text_color"

/**
 * @const STYLE_ID_TIPS_TEXT_COLOR
 * 提示文本颜色。
 */
#define STYLE_ID_TIPS_TEXT_COLOR "tips_text_color"

/**
 * @const STYLE_ID_TEXT_ALIGN_H
 * 文本水平对齐的方式。
 */
#define STYLE_ID_TEXT_ALIGN_H "text_align_h"

/**
 * @const STYLE_ID_TEXT_ALIGN_V
 * 文本垂直对齐的方式。
 */
#define STYLE_ID_TEXT_ALIGN_V "text_align_v"

/**
 * @const STYLE_ID_BORDER_COLOR
 * 边框颜色。
 */
#define STYLE_ID_BORDER_COLOR "border_color"

/**
 * @const STYLE_ID_BORDER_WIDTH
 * 边框线宽。
 */
#define STYLE_ID_BORDER_WIDTH "border_width"

/**
 * @const STYLE_ID_BORDER
 * 边框类型。
 */
#define STYLE_ID_BORDER "border"

/**
 * @const STYLE_ID_BG_IMAGE
 * 图片的名称。
 */
#define STYLE_ID_BG_IMAGE "bg_image"

/**
 * @const STYLE_ID_BG_IMAGE_DRAW_TYPE
 * 图片的显示方式。
 */
#define STYLE_ID_BG_IMAGE_DRAW_TYPE "bg_image_draw_type"

/**
 * @const STYLE_ID_ICON
 * 图标的名称。
 */
#define STYLE_ID_ICON "icon"

/**
 * @const STYLE_ID_FG_IMAGE
 * 图片的名称。
 */
#define STYLE_ID_FG_IMAGE "fg_image"

/**
 * @const STYLE_ID_FG_IMAGE_DRAW_TYPE
 * 图片的显示方式。
 */
#define STYLE_ID_FG_IMAGE_DRAW_TYPE "fg_image_draw_type"

/**
 * @const STYLE_ID_SPACER
 * 间距。
 */
#define STYLE_ID_SPACER "spacer"

/**
 * @const STYLE_ID_MARGIN
 * 边距。
 */
#define STYLE_ID_MARGIN "margin"

/**
 * @const STYLE_ID_MARGIN_LEFT
 * 左边距。
 */
#define STYLE_ID_MARGIN_LEFT "margin_left"

/**
 * @const STYLE_ID_MARGIN_RIGHT
 * 右边距。
 */
#define STYLE_ID_MARGIN_RIGHT "margin_right"

/**
 * @const STYLE_ID_MARGIN_TOP
 * 顶边距。
 */
#define STYLE_ID_MARGIN_TOP "margin_top"

/**
 * @const STYLE_ID_MARGIN_BOTTOM
 * 底边距。
 */
#define STYLE_ID_MARGIN_BOTTOM "margin_bottom"

/**
 * @const STYLE_ID_ICON_AT
 * 图标的位置。
 */
#define STYLE_ID_ICON_AT "icon_at"

/**
 * @const STYLE_ID_ACTIVE_ICON
 * Active图标的名称。
 */
#define STYLE_ID_ACTIVE_ICON "active_icon"

/**
 * @const STYLE_ID_X_OFFSET
 * X方向的偏移，方便实现按下的效果。
 */
#define STYLE_ID_X_OFFSET "x_offset"

/**
 * @const STYLE_ID_Y_OFFSET
 * Y方向的偏移，方便实现按下的效果。
 */
#define STYLE_ID_Y_OFFSET "y_offset"

/**
 * @const STYLE_ID_SELECTED_BG_COLOR
 * 编辑器中选中区域的背景颜色。
 */
#define STYLE_ID_SELECTED_BG_COLOR "selected_bg_color"

/**
 * @const STYLE_ID_SELECTED_FG_COLOR
 * 编辑器中选中区域的前景颜色。
 */
#define STYLE_ID_SELECTED_FG_COLOR "selected_fg_color"

/**
 * @const STYLE_ID_SELECTED_TEXT_COLOR
 * 编辑器中选中区域的文本颜色。
 */
#define STYLE_ID_SELECTED_TEXT_COLOR "selected_text_color"

/**
 * @const STYLE_ID_ROUND_RADIUS
 * 圆角半径(仅在WITH_VGCANVAS定义时生效)。
 */
#define STYLE_ID_ROUND_RADIUS "round_radius"

/**
 * @const STYLE_ID_ROUND_RADIUS_TOP_LETF
 * 左上角圆角半径(仅在WITH_VGCANVAS定义时生效)。
 */
#define STYLE_ID_ROUND_RADIUS_TOP_LETF "round_radius_top_left"

/**
 * @const STYLE_ID_ROUND_RADIUS_TOP_RIGHT
 * 右上角圆角半径(仅在WITH_VGCANVAS定义时生效)。
 */
#define STYLE_ID_ROUND_RADIUS_TOP_RIGHT "round_radius_top_right"

/**
 * @const STYLE_ID_ROUND_RADIUS_BOTTOM_LETF
 * 左下角圆角半径(仅在WITH_VGCANVAS定义时生效)。
 */
#define STYLE_ID_ROUND_RADIUS_BOTTOM_LETF "round_radius_bottom_left"

/**
 * @const STYLE_ID_ROUND_RADIUS_BOTTOM_RIGHT
 * 右下角圆角半径(仅在WITH_VGCANVAS定义时生效)。
 */
#define STYLE_ID_ROUND_RADIUS_BOTTOM_RIGHT "round_radius_bottom_right"

/**
 * @const STYLE_ID_CHILDREN_LAYOUT
 * 子控件布局参数。
 */
#define STYLE_ID_CHILDREN_LAYOUT "children_layout"

/**
 * @const STYLE_ID_SELF_LAYOUT
 * 控件布局参数。
 */
#define STYLE_ID_SELF_LAYOUT "self_layout"

/**
 * @const STYLE_ID_FOCUSABLE
 * 是否支持焦点停留。
 */
#define STYLE_ID_FOCUSABLE "focusable"

/**
 * @const STYLE_ID_FEEDBACK
 * 是否启用按键音、触屏音和震动等反馈。
 */
#define STYLE_ID_FEEDBACK "feedback"

struct _style_t;
typedef struct _style_t style_t;

typedef bool_t (*style_is_valid_t)(style_t* s);
typedef int32_t (*style_get_int_t)(style_t* s, const char* name, int32_t defval);
typedef uint32_t (*style_get_uint_t)(style_t* s, const char* name, uint32_t defval);
typedef color_t (*style_get_color_t)(style_t* s, const char* name, color_t defval);
typedef const char* (*style_get_str_t)(style_t* s, const char* name, const char* defval);

typedef ret_t (*style_set_t)(style_t* s, const char* state, const char* name, const value_t* value);

typedef ret_t (*style_notify_widget_state_changed_t)(style_t* s, widget_t* widget);
typedef ret_t (*style_update_state_t)(style_t* s, theme_t* theme, const char* widget_type,
                                      const char* style_name, const char* widget_state);

typedef ret_t (*style_destroy_t)(style_t* s);

typedef ret_t (*style_set_style_data_t)(style_t* s, const uint8_t* data, const char* state);
typedef const char* (*style_get_style_state_t)(style_t* s);
typedef const char* (*style_get_style_type_t)(style_t* s);

typedef struct _style_vtable_t {
  bool_t is_mutable;
  style_is_valid_t is_valid;
  style_get_int_t get_int;
  style_get_uint_t get_uint;
  style_get_str_t get_str;
  style_get_color_t get_color;
  style_update_state_t update_state;
  style_notify_widget_state_changed_t notify_widget_state_changed;

  style_set_t set;
  style_destroy_t destroy;
  style_get_style_type_t get_style_type;
  style_set_style_data_t set_style_data;
  style_get_style_state_t get_style_state;
} style_vtable_t;

/**
 * @class style_t
 * @annotation ["scriptable"]
 * 控件风格。
 *
 * widget从style对象中，获取诸如字体、颜色和图片相关的参数，根据这些参数来绘制界面。
 *
 * ```c
 * style_t* style = widget->astyle;
 * int32_t margin = style_get_int(style, STYLE_ID_MARGIN, 2);
 * int32_t icon_at = style_get_int(style, STYLE_ID_ICON_AT, ICON_AT_AUTO);
 * uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
 * ```
 *
 * 属性名称的请参考[style\_id](style_id_t.md)
 *
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
 * 获取指定name的整数格式的值。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {const char*} name 属性名。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回整数格式的值。
 */
int32_t style_get_int(style_t* s, const char* name, int32_t defval);

/**
 * @method style_get_uint
 * 获取指定name的无符号整数格式的值。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {const char*} name 属性名。
 * @param {uint32_t} defval 缺省值。
 *
 * @return {uint32_t} 返回无符号整数格式的值。
 */
uint32_t style_get_uint(style_t* s, const char* name, uint32_t defval);

/**
 * @method style_get_color
 * 获取指定name的颜色值。
 * @param {style_t*} s style对象。
 * @param {const char*} name 属性名。
 * @param {color_t} defval 缺省值。
 *
 * @return {color_t} 返回颜色值。
 */
color_t style_get_color(style_t* s, const char* name, color_t defval);

/**
 * @method style_get_str
 * 获取指定name的字符串格式的值。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {const char*} name 属性名。
 * @param {const char*} defval 缺省值。
 *
 * @return {const char*} 返回字符串格式的值。
 */
const char* style_get_str(style_t* s, const char* name, const char* defval);

/**
 * @method style_set
 * 设置指定状态的指定属性的值(仅仅对mutable的style有效)。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {const char*} state 状态。
 * @param {const char*} name 属性名。
 * @param {const value_t*} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_set(style_t* s, const char* state, const char* name, const value_t* value);

/**
 * @method style_set_style_data
 * 把风格对象数据设置到风格对象中
 * @param {style_t*} s style对象。
 * @param {const uint8_t*} data 风格对象数据
 * @param {const char*} state 风格状态
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_set_style_data(style_t* s, const uint8_t* data, const char* state);

/**
 * @method style_update_state
 * 更新风格对象的状态以及对应的数据
 * 备注：根据 widget_type 和 style_name 以及 widget_state 在 theme 对象中查找对应的数据并且更新到 style 对象中
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 * @param {theme_t*} theme theme对象。
 * @param {const char*} widget_type 控件的类型名。
 * @param {const char*} style_name style的名称。
 * @param {const char*} widget_state 控件的状态。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_update_state(style_t* s, theme_t* theme, const char* widget_type,
                         const char* style_name, const char* widget_state);

/**
 * @method style_get_style_state
 * 获取风格对象的风格状态
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 *
 * @return {const char*} 返回风格状态。
 */
const char* style_get_style_state(style_t* s);

/**
 * @method style_is_mutable
 * 检查style是否是mutable的。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t style_is_mutable(style_t* s);

/**
 * @method style_get_style_type
 * 获取 style 的风格类型。
 * @annotation ["scriptable"]
 * @param {style_t*} s style对象。
 *
 * @return {const char*} 返回风格类型。
 */
const char* style_get_style_type(style_t* s);

/**
 * @method style_destroy
 * 销毁style对象
 * @param {style_t*} s style对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_destroy(style_t* s);

/*helper functions*/
/**
 * @method style_normalize_value
 * 对值进行正规化。
 * @param {const char*} name 名称。
 * @param {const char*} value 值。
 * @param {value_t*} out 返回的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_normalize_value(const char* name, const char* value, value_t* out);

END_C_DECLS

#endif /*TK_STYLE_H*/
