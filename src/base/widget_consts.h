/**
 * File:   widget_consts.h
 * Author: AWTK Develop Team
 * Brief:  widget property names
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_CONSTS_H
#define TK_WIDGET_CONSTS_H

#include "base/types_def.h"

BEGIN_C_DECLS

/*widget props*/

/**
 * @enum widget_prop_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_PROP_
 * 控件的属性。
 */

/**
 * @const WIDGET_PROP_EXEC
 * 用于执行某些特殊的命令（比如控制动画的启停），主要是方便MVVM通过属性来控制动画。
 */
#define WIDGET_PROP_EXEC "exec"

/**
 * @const WIDGET_PROP_X
 * X坐标。
 */
#define WIDGET_PROP_X "x"

/**
 * @const WIDGET_PROP_Y
 * Y坐标。
 */
#define WIDGET_PROP_Y "y"

/**
 * @const WIDGET_PROP_W
 * 宽度。
 */
#define WIDGET_PROP_W "w"

/**
 * @const WIDGET_PROP_H
 * 高度。
 */
#define WIDGET_PROP_H "h"

/**
 * @const WIDGET_PROP_INPUTING
 * inputing。
 */
#define WIDGET_PROP_INPUTING "inputing"

/**
 * @const WIDGET_PROP_CARET_X
 * caret x。
 */
#define WIDGET_PROP_CARET_X "caret_x"

/**
 * @const WIDGET_PROP_CARET_Y
 * caret y。
 */
#define WIDGET_PROP_CARET_Y "caret_y"

/**
 * @const WIDGET_PROP_DIRTY_RECT_TOLERANCE
 * 脏矩形超出控件本身大小的最大范围。
 */
#define WIDGET_PROP_DIRTY_RECT_TOLERANCE "dirty_rect_tolerance"

/**
 * @const WIDGET_PROP_BIDI
 * bidi type(rtl,ltr,auto,wrtl,wltr,lro,rlo)。
 */
#define WIDGET_PROP_BIDI "bidi"

/**
 * @const WIDGET_PROP_CANVAS
 * Canvas。
 */
#define WIDGET_PROP_CANVAS "canvas"

/**
 * @const WIDGET_PROP_LOCALIZE_OPTIONS
 * Canvas。
 */
#define WIDGET_PROP_LOCALIZE_OPTIONS "localize_options"

/**
 * @const WIDGET_PROP_NATIVE_WINDOW
 * Native Window。
 */
#define WIDGET_PROP_NATIVE_WINDOW "native_window"

/**
 * @const WIDGET_PROP_HIGHLIGHT
 * dialog highlight。
 */
#define WIDGET_PROP_HIGHLIGHT "highlight"

/**
 * @const WIDGET_PROP_BAR_SIZE
 * slider中的bar的的宽度或高度。
 */
#define WIDGET_PROP_BAR_SIZE "bar_size"

/**
 * @const WIDGET_PROP_OPACITY
 * 不透明度。
 */
#define WIDGET_PROP_OPACITY "opacity"

/**
 * @const WIDGET_PROP_MIN_W
 * 最小宽度。
 */
#define WIDGET_PROP_MIN_W "min_w"

/**
 * @const WIDGET_PROP_MAX_W
 * 最大宽度。
 */
#define WIDGET_PROP_MAX_W "max_w"

/**
 * @const WIDGET_PROP_CHILDREN_LAYOUT
 * 子控件布局参数。
 */
#define WIDGET_PROP_CHILDREN_LAYOUT "children_layout"

/**
 * @const WIDGET_PROP_LAYOUT
 * 子控件布局参数(过时)。
 */
#define WIDGET_PROP_LAYOUT "layout"

/**
 * @const WIDGET_PROP_SELF_LAYOUT
 * 控件布局参数。
 */
#define WIDGET_PROP_SELF_LAYOUT "self_layout"

/**
 * @const WIDGET_PROP_LAYOUT_W
 * layout宽度。
 */
#define WIDGET_PROP_LAYOUT_W "layout_w"

/**
 * @const WIDGET_PROP_LAYOUT_H
 * layout高度。
 */
#define WIDGET_PROP_LAYOUT_H "layout_h"

/**
 * @const WIDGET_PROP_VIRTUAL_W
 * 虚拟宽度。
 */
#define WIDGET_PROP_VIRTUAL_W "virtual_w"

/**
 * @const WIDGET_PROP_VIRTUAL_H
 * 虚拟高度。
 */
#define WIDGET_PROP_VIRTUAL_H "virtual_h"

/**
 * @const WIDGET_PROP_NAME
 * 名称。
 */
#define WIDGET_PROP_NAME "name"

/**
 * @const WIDGET_PROP_TYPE
 * 类型。
 */
#define WIDGET_PROP_TYPE "type"

/**
 * @const WIDGET_PROP_CLOSABLE
 * 是否可以关闭。
 */
#define WIDGET_PROP_CLOSABLE "closable"

/**
 * @const WIDGET_PROP_POINTER_CURSOR
 * 鼠标指针。
 */
#define WIDGET_PROP_POINTER_CURSOR "pointer_cursor"

/**
 * @const WIDGET_PROP_VALUE
 * 值。
 */
#define WIDGET_PROP_VALUE "value"

/**
 * @const WIDGET_PROP_LENGTH
 * 长度。
 */
#define WIDGET_PROP_LENGTH "length"

/**
 * @const WIDGET_PROP_TEXT
 * 文本。
 */
#define WIDGET_PROP_TEXT "text"

/**
 * @const WIDGET_PROP_TR_TEXT
 * 待翻译文本。
 */
#define WIDGET_PROP_TR_TEXT "tr_text"

/**
 * @const WIDGET_PROP_STYLE
 * style。
 */
#define WIDGET_PROP_STYLE "style"

/**
 * @const WIDGET_PROP_ENABLE
 * 是否启用。
 */
#define WIDGET_PROP_ENABLE "enable"

/**
 * @const WIDGET_PROP_FEEDBACK
 * 是否启用按键音等反馈。
 */
#define WIDGET_PROP_FEEDBACK "feedback"

/**
 * @const WIDGET_PROP_FLOATING
 * 是否启用floating布局。
 */
#define WIDGET_PROP_FLOATING "floating"

/**
 * @const WIDGET_PROP_MARGIN
 * 边距。
 */
#define WIDGET_PROP_MARGIN "margin"

/**
 * @const WIDGET_PROP_SPACING
 * 间距。
 */
#define WIDGET_PROP_SPACING "spacing"

/**
 * @const WIDGET_PROP_LEFT_MARGIN
 * 左边距。
 */
#define WIDGET_PROP_LEFT_MARGIN "left_margin"

/**
 * @const WIDGET_PROP_RIGHT_MARGIN
 * 右边距。
 */
#define WIDGET_PROP_RIGHT_MARGIN "right_margin"

/**
 * @const WIDGET_PROP_TOP_MARGIN
 * 顶边距。
 */
#define WIDGET_PROP_TOP_MARGIN "top_margin"

/**
 * @const WIDGET_PROP_BOTTOM_MARGIN
 * 底边距。
 */
#define WIDGET_PROP_BOTTOM_MARGIN "bottom_margin"

/**
 * @const WIDGET_PROP_STEP
 * 步长。
 */
#define WIDGET_PROP_STEP "step"

/**
 * @const WIDGET_PROP_VISIBLE
 * 是否可见。
 */
#define WIDGET_PROP_VISIBLE "visible"

/**
 * @const WIDGET_PROP_SENSITIVE
 * 是否接受用户事件。
 */
#define WIDGET_PROP_SENSITIVE "sensitive"

/**
 * @const WIDGET_PROP_ANIMATION
 * 控件动画。
 */
#define WIDGET_PROP_ANIMATION "animation"

/**
 * @const WIDGET_PROP_ANIM_HINT
 * 窗口动画。
 */
#define WIDGET_PROP_ANIM_HINT "anim_hint"

/**
 * @const WIDGET_PROP_FULLSCREEN
 * 窗口设置为全部大小。
 */
#define WIDGET_PROP_FULLSCREEN "fullscreen"

/**
 * @const WIDGET_PROP_DISABLE_ANIM
 * 禁用窗口动画。
 */
#define WIDGET_PROP_DISABLE_ANIM "disable_anim"

/**
 * @const WIDGET_PROP_OPEN_ANIM_HINT
 * 打开窗口时的动画。
 */
#define WIDGET_PROP_OPEN_ANIM_HINT "open_anim_hint"

/**
 * @const WIDGET_PROP_CLOSE_ANIM_HINT
 * 关闭窗口时的动画。
 */
#define WIDGET_PROP_CLOSE_ANIM_HINT "close_anim_hint"

/**
 * @const WIDGET_PROP_MIN
 * 最小值。
 */
#define WIDGET_PROP_MIN "min"

/**
 * @const WIDGET_PROP_ACTION_TEXT
 * 软键盘上action按钮的文本。
 */
#define WIDGET_PROP_ACTION_TEXT "action_text"

/**
 * @const WIDGET_PROP_TIPS
 * 提示信息。
 */
#define WIDGET_PROP_TIPS "tips"

/**
 * @const WIDGET_PROP_TR_TIPS
 * 需要翻译的提示信息。
 */
#define WIDGET_PROP_TR_TIPS "tr_tips"

/**
 * @const WIDGET_PROP_INPUT_TYPE
 * 输入类型。
 */
#define WIDGET_PROP_INPUT_TYPE "input_type"

/**
 * @const WIDGET_PROP_KEYBOARD
 * 自定义软键盘资源名称。
 */
#define WIDGET_PROP_KEYBOARD "keyboard"

/**
 * @const WIDGET_PROP_DEFAULT_FOCUSED_CHILD
 * 缺省获得焦点的子控件(可用控件名或类型)。
 */
#define WIDGET_PROP_DEFAULT_FOCUSED_CHILD "default_focused_child"

/**
 * @const WIDGET_PROP_READONLY
 * 只读模式。
 */
#define WIDGET_PROP_READONLY "readonly"

/**
 * @const WIDGET_PROP_PASSWORD_VISIBLE
 * 密码是否可见。
 */
#define WIDGET_PROP_PASSWORD_VISIBLE "password_visible"

/**
 * @const WIDGET_PROP_ACTIVE
 * 是否处于active状态。
 */
#define WIDGET_PROP_ACTIVE "active"

/**
 * @const WIDGET_PROP_VERTICAL
 * 是否为垂直模式。
 */
#define WIDGET_PROP_VERTICAL "vertical"

/**
 * @const WIDGET_PROP_SHOW_TEXT
 * 是否显示文本。
 */
#define WIDGET_PROP_SHOW_TEXT "show_text"

/**
 * @const WIDGET_PROP_XOFFSET
 * X方向的偏移。
 */
#define WIDGET_PROP_XOFFSET "xoffset"

/**
 * @const WIDGET_PROP_YOFFSET
 * Y方向的偏移。
 */
#define WIDGET_PROP_YOFFSET "yoffset"

/**
 * @const WIDGET_PROP_ALIGN_V
 * 垂直对齐模式。
 */
#define WIDGET_PROP_ALIGN_V "align_v"

/**
 * @const WIDGET_PROP_ALIGN_H
 * 水平对齐模式。
 */
#define WIDGET_PROP_ALIGN_H "align_h"

/**
 * @const WIDGET_PROP_AUTO_PLAY
 * 是否自动播放或指定播放的时间。
 */
#define WIDGET_PROP_AUTO_PLAY "auto_play"

/**
 * @const WIDGET_PROP_LOOP
 * 是否循环播放或循环播放的次数。
 */
#define WIDGET_PROP_LOOP "loop"

/**
 * @const WIDGET_PROP_AUTO_FIX
 * 是否启用自动更正功能。
 */
#define WIDGET_PROP_AUTO_FIX "auto_fix"

/**
 * @const WIDGET_PROP_SELECT_NONE_WHEN_FOCUSED
 * 编辑器在获得焦点时是否不选中文本。
 */
#define WIDGET_PROP_SELECT_NONE_WHEN_FOCUSED "select_none_when_focused"

/**
 * @const WIDGET_PROP_OPEN_IM_WHEN_FOCUSED
 * 编辑器在获得焦点时是否打开输入法。
 */
#define WIDGET_PROP_OPEN_IM_WHEN_FOCUSED "open_im_when_focused"

/**
 * @const WIDGET_PROP_X_MIN
 * X最小值。
 */
#define WIDGET_PROP_X_MIN "x_min"

/**
 * @const WIDGET_PROP_X_MAX
 * X最大值。
 */
#define WIDGET_PROP_X_MAX "x_max"

/**
 * @const WIDGET_PROP_Y_MIN
 * Y最小值。
 */
#define WIDGET_PROP_Y_MIN "y_min"

/**
 * @const WIDGET_PROP_Y_MAX
 * Y最大值。
 */
#define WIDGET_PROP_Y_MAX "y_max"

/**
 * @const WIDGET_PROP_MAX
 * 最大值。
 */
#define WIDGET_PROP_MAX "max"

/**
 * @const WIDGET_PROP_GRAB_KEYS
 * 让窗口管理器直接把按键发给自己。
 */
#define WIDGET_PROP_GRAB_KEYS "grab_keys"

/**
 * @const WIDGET_PROP_ROW
 * 行数或每行的高度。
 */
#define WIDGET_PROP_ROW "row"

/**
 * @const WIDGET_PROP_STATE_FOR_STYLE
 * 控件状态。
 */
#define WIDGET_PROP_STATE_FOR_STYLE "state_for_style"

/**
 * @const WIDGET_PROP_THEME
 * 窗口主题名称。
 */
#define WIDGET_PROP_THEME "theme"

/**
 * @const WIDGET_PROP_STAGE
 * window stage
 */
#define WIDGET_PROP_STAGE "stage"

/**
 * @const WIDGET_PROP_IMAGE_MANAGER
 * 图片管理器。
 */
#define WIDGET_PROP_IMAGE_MANAGER "image_manager"

/**
 * @const WIDGET_PROP_ASSETS_MANAGER
 * 资源管理器。
 */
#define WIDGET_PROP_ASSETS_MANAGER "assets_manager"

/**
 * @const WIDGET_PROP_LOCALE_INFO
 * locale_info。
 */
#define WIDGET_PROP_LOCALE_INFO "locale_info"

/**
 * @const WIDGET_PROP_FONT_MANAGER
 * 字体管理器。
 */
#define WIDGET_PROP_FONT_MANAGER "font_manager"

/**
 * @const WIDGET_PROP_THEME_OBJ
 * 窗口的主题对象。
 */
#define WIDGET_PROP_THEME_OBJ "theme_obj"

/**
 * @const WIDGET_PROP_DEFAULT_THEME_OBJ
 * 缺省的主题对象。
 */
#define WIDGET_PROP_DEFAULT_THEME_OBJ "default_theme_obj"

/**
 * @const WIDGET_PROP_ITEM_WIDTH
 * 项的宽度。
 */
#define WIDGET_PROP_ITEM_WIDTH "item_width"

/**
 * @const WIDGET_PROP_ITEM_HEIGHT
 * 项的高度。
 */
#define WIDGET_PROP_ITEM_HEIGHT "item_height"

/**
 * @const WIDGET_PROP_DEFAULT_ITEM_HEIGHT
 * 项的缺省高度。
 */
#define WIDGET_PROP_DEFAULT_ITEM_HEIGHT "default_item_height"

/**
 * @const WIDGET_PROP_XSLIDABLE
 * X方向是否可拖动。
 */
#define WIDGET_PROP_XSLIDABLE "xslidable"

/**
 * @const WIDGET_PROP_YSLIDABLE
 * Y方向是否可拖动。
 */
#define WIDGET_PROP_YSLIDABLE "yslidable"

/**
 * @const WIDGET_PROP_REPEAT
 * 重复次数。
 */
#define WIDGET_PROP_REPEAT "repeat"

/**
 * @const WIDGET_PROP_ENABLE_LONG_PRESS
 * 是否启用长按。
 */
#define WIDGET_PROP_ENABLE_LONG_PRESS "enable_long_press"

/**
 * @const WIDGET_PROP_ANIMATABLE
 * 是否启用动画。
 */
#define WIDGET_PROP_ANIMATABLE "animatable"

/**
 * @const WIDGET_PROP_AUTO_HIDE_SCROLL_BAR
 * 是否自动隐藏滚动条。
 */
#define WIDGET_PROP_AUTO_HIDE_SCROLL_BAR "auto_hide_scroll_bar"

/**
 * @const WIDGET_PROP_IMAGE
 * 图片名称。
 */
#define WIDGET_PROP_IMAGE "image"

/**
 * @const WIDGET_PROP_FORMAT
 * 显示格式。
 */
#define WIDGET_PROP_FORMAT "format"

/**
 * @const WIDGET_PROP_DRAW_TYPE
 * 图片绘制类型。
 */
#define WIDGET_PROP_DRAW_TYPE "draw_type"

/**
 * @const WIDGET_PROP_SELECTABLE
 * 是否可选择。
 */
#define WIDGET_PROP_SELECTABLE "selectable"

/**
 * @const WIDGET_PROP_CLICKABLE
 * 是否可点击。
 */
#define WIDGET_PROP_CLICKABLE "clickable"

/**
 * @const WIDGET_PROP_SCALE_X
 * X方向缩放比例。
 */
#define WIDGET_PROP_SCALE_X "scale_x"

/**
 * @const WIDGET_PROP_SCALE_Y
 * Y方向缩放比例。
 */
#define WIDGET_PROP_SCALE_Y "scale_y"

/**
 * @const WIDGET_PROP_ANCHOR_X
 * x锚点。
 */
#define WIDGET_PROP_ANCHOR_X "anchor_x"

/**
 * @const WIDGET_PROP_ANCHOR_Y
 * y锚点。
 */
#define WIDGET_PROP_ANCHOR_Y "anchor_y"

/**
 * @const WIDGET_PROP_ROTATION
 * 选中角度(幅度)
 */
#define WIDGET_PROP_ROTATION "rotation"

/**
 * @const WIDGET_PROP_COMPACT
 * 紧凑模式。
 */
#define WIDGET_PROP_COMPACT "compact"

/**
 * @const WIDGET_PROP_SCROLLABLE
 * 是否支持滚动。
 */
#define WIDGET_PROP_SCROLLABLE "scrollable"

/**
 * @const WIDGET_PROP_ICON
 * 图标名称。
 */
#define WIDGET_PROP_ICON "icon"

/**
 * @const WIDGET_PROP_OPTIONS
 * 选项集合。
 */
#define WIDGET_PROP_OPTIONS "options"

/**
 * @const WIDGET_PROP_SELECTED
 * 是否被选中。
 */
#define WIDGET_PROP_SELECTED "selected"

/**
 * @const WIDGET_PROP_CHECKED
 * 是否被勾选。
 */
#define WIDGET_PROP_CHECKED "checked"

/**
 * @const WIDGET_PROP_ACTIVE_ICON
 * active状态下的图标。
 */
#define WIDGET_PROP_ACTIVE_ICON "active_icon"

/**
 * @const WIDGET_PROP_LOAD_UI
 * 动态加载UI名字。
 */
#define WIDGET_PROP_LOAD_UI "load_ui"

/**
 * @const WIDGET_PROP_OPEN_WINDOW
 * 要打开窗口的名称。
 */
#define WIDGET_PROP_OPEN_WINDOW "open_window"

/**
 * @const WIDGET_PROP_SELECTED_INDEX
 * 被选中项的索引。
 */
#define WIDGET_PROP_SELECTED_INDEX "selected_index"

/**
 * @const WIDGET_PROP_CLOSE_WHEN_CLICK
 * 点击窗口时关闭窗口。
 */
#define WIDGET_PROP_CLOSE_WHEN_CLICK "close_when_click"

/**
 * @const WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE
 * 点击窗口外部时关闭窗口。
 */
#define WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE "close_when_click_outside"

/**
 * @const WIDGET_PROP_LINE_GAP
 * 行间距。
 */
#define WIDGET_PROP_LINE_GAP "line_gap"

/**
 * @const WIDGET_PROP_BG_COLOR
 * 背景颜色(仅仅使用于color tile)。
 */
#define WIDGET_PROP_BG_COLOR "bg_color"

/**
 * @const WIDGET_PROP_BORDER_COLOR
 * 边框颜色(仅仅使用于color tile)。
 */
#define WIDGET_PROP_BORDER_COLOR "border_color"

/**
 * @const WIDGET_PROP_DELAY
 * 延迟时间(毫秒)
 */
#define WIDGET_PROP_DELAY "delay"

/**
 * @const WIDGET_PROP_IS_KEYBOARD
 * 是否为键盘。
 */
#define WIDGET_PROP_IS_KEYBOARD "is_keyboard"

/**
 * @const WIDGET_PROP_FOCUSED
 * 是否为焦点控件。
 */
#define WIDGET_PROP_FOCUSED "focused"

/**
 * @const WIDGET_PROP_FOCUS
 * (过时请用focused)。
 */
#define WIDGET_PROP_FOCUS "focus"

/**
 * @const WIDGET_PROP_FOCUSABLE
 * 是否支持焦点停留。
 */
#define WIDGET_PROP_FOCUSABLE "focusable"

/**
 * @const WIDGET_PROP_WITH_FOCUS_STATE
 * 是否支持焦点状态(如果希望style支持焦点状态，但有不希望焦点停留，可用本属性)。
 */
#define WIDGET_PROP_WITH_FOCUS_STATE "with_focus_state"

/**
 * @const WIDGET_PROP_MOVE_FOCUS_PREV_KEY
 * 将焦点移到前一个的键值。
 */
#define WIDGET_PROP_MOVE_FOCUS_PREV_KEY "move_focus_prev_key"

/**
 * @const WIDGET_PROP_MOVE_FOCUS_NEXT_KEY
 * 将焦点移到后一个的键值。
 */
#define WIDGET_PROP_MOVE_FOCUS_NEXT_KEY "move_focus_next_key"

/**
 * @const WIDGET_PROP_MOVE_FOCUS_UP_KEY
 * 将焦点向上移动的键值。
 */
#define WIDGET_PROP_MOVE_FOCUS_UP_KEY "move_focus_up_key"

/**
 * @const WIDGET_PROP_MOVE_FOCUS_DOWN_KEY
 * 将焦点向下移动的键值。
 */
#define WIDGET_PROP_MOVE_FOCUS_DOWN_KEY "move_focus_down_key"

/**
 * @const WIDGET_PROP_MOVE_FOCUS_LEFT_KEY
 * 将焦点向左移动的键值。
 */
#define WIDGET_PROP_MOVE_FOCUS_LEFT_KEY "move_focus_left_key"

/**
 * @const WIDGET_PROP_MOVE_FOCUS_RIGHT_KEY
 * 将焦点向右移动的键值。
 */
#define WIDGET_PROP_MOVE_FOCUS_RIGHT_KEY "move_focus_right_key"

/**
 * @enum widget_type_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_TYPE_
 * 控件的类型。
 */

/**
 * @const WIDGET_TYPE_NONE
 * 无特殊类型。
 */
#define WIDGET_TYPE_NONE "widget"

/**
 * @const WIDGET_TYPE_WINDOW_MANAGER
 * 窗口管理器。
 */
#define WIDGET_TYPE_WINDOW_MANAGER "window_manager"

/**
 * @const WIDGET_TYPE_NORMAL_WINDOW
 * 普通窗口。
 */
#define WIDGET_TYPE_NORMAL_WINDOW "window"

/**
 * @const WIDGET_TYPE_OVERLAY
 * overlay窗口。
 */
#define WIDGET_TYPE_OVERLAY "overlay"

/**
 * @const WIDGET_TYPE_TOOL_BAR
 * 工具条。
 */
#define WIDGET_TYPE_TOOL_BAR "tool_bar"

/**
 * @const WIDGET_TYPE_DIALOG
 * 对话框。
 */
#define WIDGET_TYPE_DIALOG "dialog"

/**
 * @const WIDGET_TYPE_POPUP
 * 弹出窗口。
 */
#define WIDGET_TYPE_POPUP "popup"

/**
 * @const WIDGET_TYPE_SYSTEM_BAR
 * system bar window
 */
#define WIDGET_TYPE_SYSTEM_BAR "system_bar"

/**
 * @const WIDGET_TYPE_SYSTEM_BAR_BOTTOM
 * system bar window ato bottom
 */
#define WIDGET_TYPE_SYSTEM_BAR_BOTTOM "system_bar_bottom"

/**
 * @const WIDGET_TYPE_SPRITE
 * 精灵窗口。
 */
#define WIDGET_TYPE_SPRITE "sprite"

/**
 * @const WIDGET_TYPE_KEYBOARD
 * 键盘窗口。
 */
#define WIDGET_TYPE_KEYBOARD "keyboard"

/**
 * @const WIDGET_TYPE_DND
 * 拖放状态窗口。
 */
#define WIDGET_TYPE_DND "dnd"

/**
 * @const WIDGET_TYPE_LABEL
 * 文本控件。
 */
#define WIDGET_TYPE_LABEL "label"

/**
 * @const WIDGET_TYPE_BUTTON
 * 按钮控件。
 */
#define WIDGET_TYPE_BUTTON "button"

/**
 * @const WIDGET_TYPE_IMAGE
 * 图片控件。
 */
#define WIDGET_TYPE_IMAGE "image"

/**
 * @const WIDGET_TYPE_EDIT
 * 文本编辑控件。
 */
#define WIDGET_TYPE_EDIT "edit"

/**
 * @const WIDGET_TYPE_PROGRESS_BAR
 * 进度条控件。
 */
#define WIDGET_TYPE_PROGRESS_BAR "progress_bar"

/**
 * @const WIDGET_TYPE_GROUP_BOX
 * 分组控件。
 */
#define WIDGET_TYPE_GROUP_BOX "group_box"

/**
 * @const WIDGET_TYPE_CHECK_BUTTON
 * 多选按钮控件。
 */
#define WIDGET_TYPE_CHECK_BUTTON "check_button"

/**
 * @const WIDGET_TYPE_RADIO_BUTTON
 * 单选按钮控件。
 */
#define WIDGET_TYPE_RADIO_BUTTON "radio_button"

/**
 * @const WIDGET_TYPE_DIALOG_TITLE
 * 对话框标题。
 */
#define WIDGET_TYPE_DIALOG_TITLE "dialog_title"

/**
 * @const WIDGET_TYPE_DIALOG_CLIENT
 * 对话框客户区域。
 */
#define WIDGET_TYPE_DIALOG_CLIENT "dialog_client"

/**
 * @const WIDGET_TYPE_SLIDER
 * 滑块控件。
 */
#define WIDGET_TYPE_SLIDER "slider"

/**
 * @const WIDGET_TYPE_VIEW
 * 视图控件。
 */
#define WIDGET_TYPE_VIEW "view"

/**
 * @const WIDGET_TYPE_COMBO_BOX
 * 下拉选择框控件。
 */
#define WIDGET_TYPE_COMBO_BOX "combo_box"

/**
 * @const WIDGET_TYPE_COMBO_BOX_ITEM
 * 下拉选择框的列表项控件。
 */
#define WIDGET_TYPE_COMBO_BOX_ITEM "combo_box_item"

/**
 * @const WIDGET_TYPE_SLIDE_VIEW
 * 滑动视图控件。
 */
#define WIDGET_TYPE_SLIDE_VIEW "slide_view"

/**
 * @const WIDGET_TYPE_SLIDE_INDICATOR
 * 滑动视图的指示器控件。
 */
#define WIDGET_TYPE_SLIDE_INDICATOR "slide_indicator"

/**
 * @const WIDGET_TYPE_SLIDE_INDICATOR_ARC
 * 滑动视图的指示器控件（圆弧显示）。
 */
#define WIDGET_TYPE_SLIDE_INDICATOR_ARC "slide_indicator_arc"

/**
 * @const WIDGET_TYPE_PAGES
 * 多页控件。
 */
#define WIDGET_TYPE_PAGES "pages"

/**
 * @const WIDGET_TYPE_TAB_BUTTON
 * 标签按钮控件。
 */
#define WIDGET_TYPE_TAB_BUTTON "tab_button"

/**
 * @const WIDGET_TYPE_TAB_CONTROL
 * 标签控件。
 */
#define WIDGET_TYPE_TAB_CONTROL "tab_control"

/**
 * @const WIDGET_TYPE_TAB_BUTTON_GROUP
 * 标签按钮分组控件。
 */
#define WIDGET_TYPE_TAB_BUTTON_GROUP "tab_button_group"

/**
 * @const WIDGET_TYPE_BUTTON_GROUP
 * 按钮分组控件。
 */
#define WIDGET_TYPE_BUTTON_GROUP "button_group"

/**
 * @const WIDGET_TYPE_CANDIDATES
 * 候选字控件。
 */
#define WIDGET_TYPE_CANDIDATES "candidates"

/**
 * @const WIDGET_TYPE_SPIN_BOX
 * 数值编辑控件。
 */
#define WIDGET_TYPE_SPIN_BOX "spin_box"

/**
 * @const WIDGET_TYPE_DRAGGER
 * 拖动块控件。
 */
#define WIDGET_TYPE_DRAGGER "dragger"

/**
 * @const WIDGET_TYPE_SCROLL_BAR
 * 滚动条控件。
 */
#define WIDGET_TYPE_SCROLL_BAR "scroll_bar"

/**
 * @const WIDGET_TYPE_SCROLL_BAR_DESKTOP
 * 桌面版滚动条控件。
 */
#define WIDGET_TYPE_SCROLL_BAR_DESKTOP "scroll_bar_d"

/**
 * @const WIDGET_TYPE_SCROLL_BAR_MOBILE
 * 移动版滚动条控件。
 */
#define WIDGET_TYPE_SCROLL_BAR_MOBILE "scroll_bar_m"

/**
 * @const WIDGET_TYPE_SCROLL_VIEW
 * 滚动视图控件。
 */
#define WIDGET_TYPE_SCROLL_VIEW "scroll_view"

/**
 * @const WIDGET_TYPE_LIST_VIEW
 * 列表视图控件。
 */
#define WIDGET_TYPE_LIST_VIEW "list_view"

/**
 * @const WIDGET_TYPE_LIST_VIEW_H
 * 水平列表视图控件。
 */
#define WIDGET_TYPE_LIST_VIEW_H "list_view_h"

/**
 * @const WIDGET_TYPE_LIST_ITEM
 * 列表项控件。
 */
#define WIDGET_TYPE_LIST_ITEM "list_item"

/**
 * @const WIDGET_TYPE_COLOR_PICKER
 * 颜色选择器控件。
 */
#define WIDGET_TYPE_COLOR_PICKER "color_picker"

/**
 * @const WIDGET_TYPE_COLOR_COMPONENT
 * 颜色选择器组件控件。
 */
#define WIDGET_TYPE_COLOR_COMPONENT "color_component"

/**
 * @const WIDGET_TYPE_COLOR_TILE
 * 颜色块控件。
 */
#define WIDGET_TYPE_COLOR_TILE "color_tile"

/**
 * @const WIDGET_TYPE_CLIP_VIEW
 * 裁剪控件。
 */
#define WIDGET_TYPE_CLIP_VIEW "clip_view"

/**
 * @const WIDGET_TYPE_RICH_TEXT
 * 富文本控件。
 */
#define WIDGET_TYPE_RICH_TEXT "rich_text"

/**
 * @const WIDGET_TYPE_APP_BAR
 * AppBar控件。
 */
#define WIDGET_TYPE_APP_BAR "app_bar"

/**
 * @const WIDGET_TYPE_GRID
 * 网格控件。
 */
#define WIDGET_TYPE_GRID "grid"

/**
 * @const WIDGET_TYPE_GRID_ITEM
 * 网格项目控件。
 */
#define WIDGET_TYPE_GRID_ITEM "grid_item"

/**
 * @const WIDGET_TYPE_ROW
 * 行控件。
 */
#define WIDGET_TYPE_ROW "row"

/**
 * @const WIDGET_TYPE_COLUMN
 * 列控件。
 */
#define WIDGET_TYPE_COLUMN "column"

/**
 * @const WIDGET_TYPE_CALIBRATION_WIN
 * 电阻屏校准窗口。
 */
#define WIDGET_TYPE_CALIBRATION_WIN "calibration_win"

/**
 * @enum window_stage_t
 * @annotation ["scriptable"]
 * @prefix WINDOW_STAGE_
 * 窗口的生命周期常量定义。
 */
typedef enum _window_stage_t {
  /**
   * @const WINDOW_STAGE_NONE
   * 初始状态。
   */
  WINDOW_STAGE_NONE = 0,
  /**
   * @const WINDOW_STAGE_CREATED
   * 创建完成。
   */
  WINDOW_STAGE_CREATED,
  /**
   * @const WINDOW_STAGE_OPENED
   * 窗口已经打开(窗口打开动画完成后)
   */
  WINDOW_STAGE_OPENED,
  /**
   * @const WINDOW_STAGE_CLOSED
   * 窗口已关闭。
   */
  WINDOW_STAGE_CLOSED,
  /**
   * @const WINDOW_STAGE_SUSPEND
   * 窗口挂起状态。
   */
  WINDOW_STAGE_SUSPEND
} window_stage_t;

/**
 * @enum window_closable_t
 * @annotation ["scriptable"]
 * @prefix WINDOW_CLOSABLE_
 * 窗口的closable常量定义。
 */
typedef enum _window_closable_t {
  /**
   * @const WINDOW_CLOSABLE_YES
   * 窗口可关闭。
   */
  WINDOW_CLOSABLE_YES = 0,
  /**
   * @const WINDOW_CLOSABLE_NO
   * 窗口不可关闭。
   */
  WINDOW_CLOSABLE_NO,
  /**
   * @const WINDOW_CLOSABLE_CONFIRM
   * 窗口需要确认后才能关闭。
   */
  WINDOW_CLOSABLE_CONFIRM
} window_closable_t;

/**
 * @enum widget_state_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_
 * 控件状态常量定义。
 *
 * 这里指定常用的状态值，扩展控件可以在自己的头文件中定义私有的状态。
 */

/**
 * @const WIDGET_STATE_NONE
 * 无效状态。
 */
#define WIDGET_STATE_NONE ""
/**
 * @const WIDGET_STATE_NORMAL
 * 正常状态。
 */
#define WIDGET_STATE_NORMAL "normal"

/**
 * @const WIDGET_STATE_PRESSED
 * 指针按下状态。
 */
#define WIDGET_STATE_PRESSED "pressed"
/**
 * @const WIDGET_STATE_OVER
 * 指针悬浮状态。
 */
#define WIDGET_STATE_OVER "over"

/**
 * @const WIDGET_STATE_DISABLE
 * 禁用状态。
 */
#define WIDGET_STATE_DISABLE "disable"

/**
 * @const WIDGET_STATE_FOCUSED
 * 聚焦状态。
 */
#define WIDGET_STATE_FOCUSED "focused"

/**
 * @const WIDGET_STATE_CHECKED
 * 勾选状态。
 */
#define WIDGET_STATE_CHECKED "checked"

/**
 * @const WIDGET_STATE_UNCHECKED
 * 没勾选状态。
 */
#define WIDGET_STATE_UNCHECKED "unchecked"

/**
 * @const WIDGET_STATE_EMPTY
 * 编辑器无内容状态。
 */
#define WIDGET_STATE_EMPTY "empty"

/**
 * @const WIDGET_STATE_EMPTY_FOCUS
 * 编辑器无内容同时聚焦的状态。
 */
#define WIDGET_STATE_EMPTY_FOCUS "empty_focus"

/**
 * @const WIDGET_STATE_ERROR
 * 输入错误状态。
 */
#define WIDGET_STATE_ERROR "error"

/**
 * @const WIDGET_STATE_SELECTED
 * 选中状态。
 */
#define WIDGET_STATE_SELECTED "selected"

/*for active/selected/current */
/**
 * @const WIDGET_STATE_NORMAL_OF_CHECKED
 * 正常状态(选中项)。
 */
#define WIDGET_STATE_NORMAL_OF_CHECKED "normal_of_checked"

/**
 * @const WIDGET_STATE_PRESSED_OF_CHECKED
 * 指针按下状态(选中项)。
 */
#define WIDGET_STATE_PRESSED_OF_CHECKED "pressed_of_checked"

/**
 * @const WIDGET_STATE_OVER_OF_CHECKED
 * 指针悬浮状态(选中项)。
 */
#define WIDGET_STATE_OVER_OF_CHECKED "over_of_checked"

/**
 * @const WIDGET_STATE_DISABLE_OF_CHECKED
 * 禁用状态(选中项)。
 */
#define WIDGET_STATE_DISABLE_OF_CHECKED "disable_of_checked"

/**
 * @const WIDGET_STATE_FOCUSED_OF_CHECKED
 * 焦点状态(选中项)。
 */
#define WIDGET_STATE_FOCUSED_OF_CHECKED "focused_of_checked"

/**
 * @const WIDGET_STATE_NORMAL_OF_ACTIVE
 * 正常状态(当前项)。
 */
#define WIDGET_STATE_NORMAL_OF_ACTIVE "normal_of_active"

/**
 * @const WIDGET_STATE_PRESSED_OF_ACTIVE
 * 指针按下状态(当前项)。
 */
#define WIDGET_STATE_PRESSED_OF_ACTIVE "pressed_of_active"

/**
 * @const WIDGET_STATE_OVER_OF_ACTIVE
 * 指针悬浮状态(当前项)。
 */
#define WIDGET_STATE_OVER_OF_ACTIVE "over_of_active"

/**
 * @const WIDGET_STATE_DISABLE_OF_ACTIVE
 * 禁用状态(当前项)。
 */
#define WIDGET_STATE_DISABLE_OF_ACTIVE "disable_of_active"

/**
 * @const WIDGET_STATE_FOCUSED_OF_ACTIVE
 * 焦点状态(当前项)。
 */
#define WIDGET_STATE_FOCUSED_OF_ACTIVE "focused_of_active"

/**
 * @enum widget_cursor_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_
 * 控件鼠标光标常量定义。
 *
 */

/**
 * @const WIDGET_CURSOR_DEFAULT
 * 默认光标。
 */
#define WIDGET_CURSOR_DEFAULT "cursor"
/**
 * @const WIDGET_CURSOR_EDIT
 * 文本选择光标。
 */
#define WIDGET_CURSOR_EDIT "cursor_text"
/**
 * @const WIDGET_CURSOR_HAND
 * 手指光标。
 */
#define WIDGET_CURSOR_HAND "cursor_hand"
/**
 * @const WIDGET_CURSOR_WAIT
 * 等待光标。
 */
#define WIDGET_CURSOR_WAIT "cursor_wait"
/**
 * @const WIDGET_CURSOR_CROSS
 * 叉光标。
 */
#define WIDGET_CURSOR_CROSS "cursor_cross"
/**
 * @const WIDGET_CURSOR_NO
 * Slashed circle or crossbones。
 */
#define WIDGET_CURSOR_NO "cursor_no"
/**
 * @const WIDGET_CURSOR_SIZENWSE
 * Double arrow pointing northwest and southeast。
 */
#define WIDGET_CURSOR_SIZENWSE "cursor_size_nwse"
/**
 * @const WIDGET_CURSOR_SIZENESW
 * Double arrow pointing northeast and southwest。
 */
#define WIDGET_CURSOR_SIZENESW "cursor_size_nesw"

/**
 * @const WIDGET_CURSOR_SIZEWE
 * Double arrow pointing west and east。
 */
#define WIDGET_CURSOR_SIZEWE "cursor_size_we"
/**
 * @const WIDGET_CURSOR_SIZENS
 * Double arrow pointing north and south。
 */
#define WIDGET_CURSOR_SIZENS "cursor_size_ns"
/**
 * @const WIDGET_CURSOR_SIZEALL
 * Four pointed arrow pointing north, south, east, and west。
 */
#define WIDGET_CURSOR_SIZEALL "cursor_size_all"

END_C_DECLS

#endif /*TK_WIDGET_CONSTS_H*/
