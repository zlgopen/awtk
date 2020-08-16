/**
 * File:   combo_box.h
 * Author: AWTK Develop Team
 * Brief:  combo_box
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMBO_BOX_H
#define TK_COMBO_BOX_H

#include "widgets/edit.h"

BEGIN_C_DECLS

typedef struct _combo_box_option_t {
  char* text;
  int32_t value;
  struct _combo_box_option_t* next;
} combo_box_option_t;

typedef widget_t* (*combo_box_custom_open_popup_t)(widget_t* combobox);

/**
 * @class combo_box_t
 * @parent edit_t
 * @annotation ["scriptable","design","widget"]
 * 下拉列表控件。
 *
 * 点击右边的按钮，可弹出一个下拉列表，从中选择一项作为当前的值。
 *
 * combo\_box\_t是[edit\_t](edit_t.md)的子类控件，edit\_t的函数均适用于combo\_box\_t控件。
 *
 * 在xml中使用"combo_box"标签创建下拉列表控件。
 *
 * 列表选项可以直接写在"options"属性中。如：
 *
 * ```xml
 * <combo_box readonly="true" x="10" y="bottom:5" w="200" h="30" tr_text="ok"
 *options="1:ok;2:cancel;"/>
 * ```
 *
 * 列表选项也可以放在独立的窗口中，用属性"open_window"指定窗口的名称。如：
 *
 * ```xml
 * <combo_box open_window="language" readonly="true" x="10" y="bottom:50" w="200" h="30"
 *tr_text="english"/>
 * ```
 *
 * language.xml:
 *
 * ```xml
 * <popup close_when_click_outside="true" h="80" >
 *  <list_view x="0"  y="0" w="100%" h="100%" item_height="30">
 *   <scroll_view name="view" x="0"  y="0" w="-12" h="100%">
 *     <combo_box_item tr_text="english"/>
 *     <combo_box_item tr_text="chinese" />
 *   </scroll_view>
 *   <scroll_bar_d name="bar" x="right" y="0" w="12" h="100%" value="0"/>
 * </list_view>
 * </popup>
 * ```
 *
 * > 更多用法请参考：[combo_box.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/combo_box.xml)
 *
 * 
 *
 * 如果在文本比较长时，希望在获得焦点时文本自动滚动，可以放入一个hscroll_label为子控件，并命名为"value"。如：
 * 
 * ```xml
 *   <combo_box left_margin="6" readonly="true" x="10" y="50" w="80" h="30" options="leftttttttttt;centerrrrrrrrrrrrrrrr;rightttttttttt;"
 *   selected_index="1">
 *   <hscroll_label x="0" y="0" w="-30" h="100%"
 *     name="value"
 *     lull="1000"
 *     loop="true"
 *     yoyo="true"
 *     ellipses="true"
 *     only_parent_focus="true"/> 
 *   <button style="combobox_down" x="right:5" y="middle" w="20" h="20"/>
 * </combo_box>
 * ```
 * 
 * 在c代码中使用函数combo\_box\_create创建下拉列表控件。如：
 *
 * ```c
 *  widget_t* combo_box = combo_box_create(win, 10, 10, 128, 30);
 *
 *  combo_box_set_options(combo_box, "left;center;right;");
 *  combo_box_set_selected_index(combo_box, 1);
 *
 * ```
 *
 * 创建之后：
 *
 * * 用combo\_box\_set\_options设置可选项目。
 * * 用combo\_box\_set\_selected\_index设置缺省项。
 *
 *> 完整示例请参考：[combo_box
 *demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/combo_box.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <combo_box>
 * <style name="default" border_color="#a0a0a0"  text_color="black" text_align_h="left">
 *   <normal     bg_color="#f0f0f0" />
 *   <focused    bg_color="#f0f0f0" border_color="black"/>
 *   <empty      bg_color="#f0f0f0" text_color="#a0a0a0" />
 * </style>
 * </combo_box>
 * ```
 *
 * * 1.combobox的下拉按钮的style名称为combobox_down，可以在主题文件中设置。
 * 
 * ```xml
 * <button>
 *  <style name="combobox_down" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" icon="arrow_down_n"/>
 *   <pressed    bg_color="#c0c0c0" icon="arrow_down_p"/>
 *   <over       bg_color="#e0e0e0" icon="arrow_down_o"/>
 * </style>
 * </button>
 * ```
 *
 *  * 2.combobox的弹出popup窗口的style名称为combobox_popup，可以在主题文件中设置。
 * 
 * ```xml
 * <popup>
 * <style name="combobox_popup" border_color="red">
 *   <normal bg_color="#808080"/>
 * </style>
 * </popup>
 * ```
 * 
 * > 更多用法请参考：[theme
 *default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L422)
 *
 */
typedef struct _combo_box_t {
  edit_t edit;

  /**
   * @property {char*} open_window
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 为点击按钮时，要打开窗口的名称。
   */
  char* open_window;

  /**
   * @property {int32_t} selected_index
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前选中的选项。
   */
  int32_t selected_index;

  /**
   * @property {int32_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  int32_t value;

  /**
   * @property {bool_t} localize_options
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否本地化(翻译)选项(缺省为TRUE)。
   */
  bool_t localize_options;

  /**
   * @property {char*} options
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置可选项(冒号分隔值和文本，分号分隔选项，如:1:red;2:green;3:blue)。
   */
  char* options;

  /**
   * @property {int32_t} item_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 下拉选项的高度。如果open_window为空，则使用缺省高度。
   */
  int32_t item_height;

  /*private*/
  str_t text;
  bool_t pressed;
  combo_box_option_t* option_items;
  combo_box_custom_open_popup_t open_popup;
} combo_box_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值改变事件。
 */

/**
 * @method combo_box_create
 * 创建combo_box对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* combo_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method combo_box_cast
 * 转换combo_box对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {widget_t*} combo_box对象。
 */
widget_t* combo_box_cast(widget_t* widget);

/**
 * @method combo_box_set_open_window
 * 点击按钮时可以打开popup窗口，本函数可设置窗口的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {const char*} open_window 弹出窗口的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_open_window(widget_t* widget, const char* open_window);

/**
 * @method combo_box_reset_options
 * 重置所有选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_reset_options(widget_t* widget);

/**
 * @method combo_box_count_options
 * 获取选项个数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {int32_t} 返回选项个数。
 */
int32_t combo_box_count_options(widget_t* widget);

/**
 * @method combo_box_set_selected_index
 * 设置第index个选项为当前选中的选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {uint32_t} index 选项的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_selected_index(widget_t* widget, uint32_t index);

/**
 * @method combo_box_set_localize_options
 * 设置是否本地化(翻译)选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {bool_t} localize_options 是否本地化(翻译)选项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_localize_options(widget_t* widget, bool_t localize_options);

/**
 * @method combo_box_set_value
 * 设置值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {int32_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_value(widget_t* widget, int32_t value);

/**
 * @method combo_box_set_item_height
 * 设置item高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {uint32_t} item_height item的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_item_height(widget_t* widget, uint32_t item_height);

/**
 * @method combo_box_append_option
 * 追加一个选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {int32_t} value 值。
 * @param {const char*} text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_append_option(widget_t* widget, int32_t value, const char* text);

/**
 * @method combo_box_set_options
 * 设置选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 * @param {const char*} options 选项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_options(widget_t* widget, const char* options);

/**
 * @method combo_box_set_custom_open_popup
 * 设置自定义的打开弹出窗口的函数。
 * @param {widget_t*} widget combo_box对象。
 * @param {combo_box_custom_open_popup_t} open_popup 回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_set_custom_open_popup(widget_t* widget, combo_box_custom_open_popup_t open_popup);

/**
 * @method combo_box_get_option
 * 获取第index个选项。
 * @param {widget_t*} widget combo_box对象。
 * @param {uint32_t} index 选项的索引。
 *
 * @return {combo_box_option_t*} 返回index个选项。
 */
combo_box_option_t* combo_box_get_option(widget_t* widget, uint32_t index);

/**
 * @method combo_box_get_value
 * 获取combo_box的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {int32_t} 返回值。
 */
int32_t combo_box_get_value(widget_t* widget);

/**
 * @method combo_box_get_text
 * 获取combo_box的文本。
 * @annotation ["scriptable"]
 * @alias combo_box_get_text_value
 * @param {widget_t*} widget combo_box对象。
 *
 * @return {const char*} 返回文本。
 */
const char* combo_box_get_text(widget_t* widget);

#define COMBO_BOX(widget) ((combo_box_t*)(combo_box_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(combo_box);

/*public for test*/
ret_t combo_box_parse_options(widget_t* widget, const char* str);

END_C_DECLS

#endif /*TK_COMBO_BOX_H*/
