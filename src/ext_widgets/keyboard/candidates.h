/**
 * File:   candidates.h
 * Author: AWTK Develop Team
 * Brief:  input method text candidates
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CANDIDATES_H
#define TK_CANDIDATES_H

#include "tkc/str.h"
#include "base/widget.h"
#include "base/hscrollable.h"

BEGIN_C_DECLS

/**
 * @class candidates_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 输入法候选字词控件。
 * 
 * 如果希望启用用数字选择对应的候选字，请设置属性grab_keys="true"。如：
 * 
 * ```xml
 * <candidates x="0" y="0" w="100%" h="30" grab_keys="true"/>
 * ```
 * 
 *>相关文件： assets/default/raw/ui/kb_default.xml
 * 
 * 如果希望通过左右键切换不同的候选字，除了设置属性grab_keys="true"，还需要设置按钮的focused状态的style。
 *
 * ```xml
 * <style name="candidates" text_color="black">
 *   <normal  />
 *   <pressed    bg_color="#c0c0c0" border_color="#a0a0a0"/>
 *   <over       bg_color="#e0e0e0" border_color="#a0a0a0"/>
 *   <focused    border_color="#a0a0a0"/>
 * </style>
 * ``` 
 * 
 *>相关文件：assets/default/raw/styles/keyboard.xml
 * 
 */
typedef struct _candidates_t {
  widget_t widget;

  /**
   * @property {bool_t} pre
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为预候选字。
   * 
   * > 预候选字: 在有的输入法中，比如T9硬键盘输入时，按下12两个键时，预候选字会显示可用的拼音列表。
   * > 从预候选字列表中选择拼音，再查询拼音对应的候选字列表。
   */
  bool_t pre;

  /**
   * @property {bool_t} select_by_num
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用用数字选择候选字。比如按下1选择第1个候选字，按下2选择第2个候选字。
   * 
   */
  bool_t select_by_num;

  /**
   * @property {bool_t} auto_hide
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 没有候选字时，是否自动隐藏控件。
   * 
   */
  bool_t auto_hide;

  /**
   * @property {char*} button_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 按钮的style名称。
   */
  char* button_style;

  /*private*/
  bool_t expanded;
  wh_t normal_h;
  uint32_t event_id;
  hscrollable_t* hscrollable;
  uint32_t candidates_nr;

  bool_t is_suggest;
} candidates_t;

/**
 * @method candidates_create
 * 创建candidates对象
 * @annotation ["constructor"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* candidates_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method candidates_cast
 * 转换为candidates对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget candidates对象。
 *
 * @return {widget_t*} candidates对象。
 */
widget_t* candidates_cast(widget_t* widget);

/**
 * @method candidates_set_pre
 * 设置是否为预候选字列表。
 * 
 * > 为预候选字列表则注册EVT\_IM\_SHOW\_PRE\_CANDIDATES，否则注册EVT\_IM\_SHOW\_CANDIDATES事件。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  pre 是否为预候选字列表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t candidates_set_pre(widget_t* widget, bool_t pre);

/**
 * @method candidates_set_select_by_num
 * 设置是否启用用数字选择候选字。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  select_by_num 是否启用用数字选择候选字。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t candidates_set_select_by_num(widget_t* widget, bool_t select_by_num);

/**
 * @method candidates_set_auto_hide
 * 设置是否自动隐藏。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  auto_hide 是否自动隐藏。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t candidates_set_auto_hide(widget_t* widget, bool_t auto_hide);

/**
 * @method candidates_set_button_style
 * 设置按钮的style名称。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  button_style 按钮的style名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t candidates_set_button_style(widget_t* widget, const char* button_style);

#define CANDIDATES_PROP_PRE "pre"
#define CANDIDATES_PROP_AUTO_HIDE "auto_hide"
#define CANDIDATES_PROP_BUTTON_STYLE "button_style"
#define CANDIDATES_PROP_SELECT_BY_NUM "select_by_num"

#define CANDIDATES(widget) ((candidates_t*)(candidates_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(candidates);

END_C_DECLS

#endif /*TK_CANDIDATES_H*/
