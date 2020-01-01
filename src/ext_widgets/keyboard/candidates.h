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

  /*private*/
  bool_t expanded;
  wh_t normal_h;
  uint32_t event_id;
  canvas_t* canvas;
  hscrollable_t* hscrollable;
  uint32_t candidates_nr;
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

#define CANDIDATES(widget) ((candidates_t*)(candidates_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(candidates);

END_C_DECLS

#endif /*TK_CANDIDATES_H*/
