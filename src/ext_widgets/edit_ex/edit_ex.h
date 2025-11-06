/**
 * File:   edit_ex.h
 * Author: AWTK Develop Team
 * Brief:  扩展edit控件
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-02-13 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_EDIT_EX_H
#define TK_EDIT_EX_H

#include "widgets/edit.h"

BEGIN_C_DECLS

/**
 * @class edit_ex_t
 * @parent edit_t
 * @annotation ["scriptable","design","widget"]
 * 扩展edit控件。支持以下功能：
 * * 支持搜索建议功能。
 */
typedef struct _edit_ex_t {
  edit_t edit;

  /**
   * @property {tk_object_t*} suggest_words
   * @annotation ["set_prop","get_prop","readable","persitent","scriptable"]
   * 输入建议词。
   */
  tk_object_t* suggest_words;

  /**
   * @property {char*} suggest_words_item_formats
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 输入建议词的项格式。
   */
  char* suggest_words_item_formats;

  /**
   * @property {char*} suggest_words_input_name
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最终输入到edit控件的文本的属性名。
   * > 设置了 suggest_words_item_formats 才会被用到。
   */
  char* suggest_words_input_name;

  /* private */
  tk_object_t* suggest_words_ui_props;
  widget_t* suggest_words_popup;
  darray_t* suggest_words_model_items;
} edit_ex_t;

/**
 * @method edit_ex_create
 * 创建edit_ex对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* edit_ex_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method edit_ex_set_suggest_words
 * 设置输入建议词源。
 * > EVT_VALUE_CHANGED 事件请求词源更新，new_value 为 edit 输入内容。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget edit_ex对象。
 * @param {tk_object_t*} suggest_words 输入建议词源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_ex_set_suggest_words(widget_t* widget, tk_object_t* suggest_words);

/**
 * @method edit_ex_set_suggest_words_item_formats
 * 设置输入建议词的项格式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget edit_ex对象。
 * @param {const char*} formats 输入建议词的项格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_ex_set_suggest_words_item_formats(widget_t* widget, const char* formats);

/**
 * @method edit_ex_set_suggest_words_input_name
 * 最终输入到edit控件的文本的属性名。
 * > 设置了 suggest_words_item_formats 才会被用到。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget edit_ex对象。
 * @param {const char*} name 最终输入到edit控件的文本的属性名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_ex_set_suggest_words_input_name(widget_t* widget, const char* name);

/**
 * @method edit_ex_update_suggest_words_popup
 * 请求刷新显示建议词窗口。
 * > suggest_words 为空时关闭窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget edit_ex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t edit_ex_update_suggest_words_popup(widget_t* widget);

/**
 * @method edit_ex_cast
 * 转换为edit对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget edit_ex对象。
 *
 * @return {widget_t*} edit对象。
 */
widget_t* edit_ex_cast(widget_t* widget);

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS
 * 输入建议词。
 */
#define EDIT_EX_PROP_SUGGEST_WORDS "suggest_words"

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS
 * 输入建议词相关ui属性。
 * eg:
 * ```xml
 * <edit_ex suggest_words_ui_props.popup.theme="number"/>
 * ```
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS "suggest_words_ui_props"
#define EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS_PREFIX EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS "."

#define EDIT_EX_SUGGEST_WORDS_UI_NAME_POPUP "popup"
#define EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW "list_view"
#define EDIT_EX_SUGGEST_WORDS_UI_NAME_VBAR "vbar"
#define EDIT_EX_SUGGEST_WORDS_UI_NAME_HBAR "hbar"
#define EDIT_EX_SUGGEST_WORDS_UI_NAME_SCROLL_VIEW "scroll_view"

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS_ITEM_ODD_STYLE
 * 奇数项的样式。
 * eg:
 * ```xml
 * <edit_ex suggest_words_ui_props.list_view.item_odd_style="odd"/>
 * ```
 * 
 * style:
 * ```xml
 * <combo_box_item>
 *   <style name="odd">
 *   </style>
 * </combo_box_item>
 * ```
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_ITEM_ODD_STYLE "item_odd_style"

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS_ITEM_EVEN_STYLE
 * 偶数项的样式。
 * eg:
 * ```xml
 * <edit_ex suggest_words_ui_props.list_view.item_even_style="even"/>
 * ```
 * 
 * style:
 * ```xml
 * <combo_box_item>
 *   <style name="even">
 *   </style>
 * </combo_box_item>
 * ```
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_ITEM_EVEN_STYLE "item_even_style"

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS_ITEM_SEPARATE_STYLE
 * 分隔线的样式。
 * eg:
 * ```xml
 * <edit_ex suggest_words_ui_props.list_view.item_separate_style="separate"/>
 * ```
 * 
 * style:
 * ```xml
 * <view>
 *   <style name="separate">
 *   </style>
 * </view>
 * ```
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_ITEM_SEPARATE_STYLE "item_separate_style"

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS_ITEM_FORMATS
 * 项格式。
 * > 格式说明：
 *   * 1. {}里包含一个格式的内容，格式与格式间用;相隔，格式为：格式名可忽略{内容}
 *   * 2. 格式内容由控件组成，控件格式为：控件类型默认为label(控件属性)[子控件]
 *   * 3. 控件可用分隔为,或|，如果使用|则自动生成分隔线。
 *   * 4. 控件可变属性前有$符号，属性会替换为输入建议词里的属性，如{(text=$title)}，label控件的属性text会替换为输入关键词里的title属性。
 * * 完整格式参考：
 * *   格式名{控件1类型(控件属性)[子控件1类型(子控件1属性),(类型为label的子控件2属性)]|(类型为label的控件2属性)};格式名2{...}
 * eg:
 * ```xml
 * <edit_ex suggest_words_item_formats="{view(w=20%)[image(w=20,image=$img),(text=$INPUT,w=-20,m=5)]|(text=$desc,w=80%)}"/>
 * ```
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_ITEM_FORMATS "suggest_words_item_formats"

/**
 * @const EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME
 * 建议词源属性：使用的格式名。
 * eg:
 * ```xml
 * <edit_ex suggest_words_item_formats="{view(w=20%)[image(w=20,image=$img),(text=$INPUT,w=-20,m=5)]|(text=$desc,w=80%)};A{(text=$INPUT,w=20%,m=5)|(text=$desc,w=80%)}"/>
 * ```
 * ```c
 * obj_t* obj = object_default_create();// 某个item的数据源。
 * tk_object_set_prop_str(obj, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, "[0]"); // 使用第一项格式。
 * // ...为obj添加其他属性
 * obj_t* obj2 = object_default_create();// 某个item的数据源。
 * tk_object_set_prop_str(obj2, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, "A"); // 使用A格式，即第二项格式。
 * // ...为obj2添加其他属性
 * ```
 */
#define EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME "@format_name"

/**
 * @const EDIT_EX_PROP_SUGGEST_WORDS_INPUT_NAME
 * 最终输入到edit控件的文本的属性名。
 * > 设置了 suggest_words_item_formats 才会被用到。
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_INPUT_NAME "suggest_words_input_name"
#define EDIT_EX_DEFAULT_SUGGEST_WORDS_INPUT_NAME "INPUT"

#define WIDGET_TYPE_EDIT_EX "edit_ex"

#define EDIT_EX(widget) ((edit_ex_t*)edit_ex_cast(WIDGET(widget)))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(edit_ex);

END_C_DECLS

#endif /*TK_EDIT_EX_H*/
