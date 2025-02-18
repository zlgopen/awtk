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

  /* private */
  tk_object_t* suggest_words_ui_props;
  widget_t* suggest_words_popup;
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
 * style
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
 * style
 * ```xml
 * <combo_box_item>
 *   <style name="even">
 *   </style>
 * </combo_box_item>
 * ```
 */
#define EDIT_EX_PROP_SUGGEST_WORDS_ITEM_EVEN_STYLE "item_even_style"

#define WIDGET_TYPE_EDIT_EX "edit_ex"

#define EDIT_EX(widget) ((edit_ex_t*)(edit_ex_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(edit_ex);

END_C_DECLS

#endif /*TK_EDIT_EX_H*/
