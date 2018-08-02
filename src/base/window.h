/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_H
#define TK_WINDOW_H

#include "base/widget.h"
#include "base/window_animator.h"

BEGIN_C_DECLS

/**
 * @class window_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 窗口。
 */
typedef struct _window_t {
  widget_t widget;
  /**
   * @property {char*} theme
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 主题资源的名称。
   */
  char* theme;
  /**
   * @property {char*} anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 动画的名称。
   */
  char* anim_hint;
} window_t;

/**
 * @method window_create
 * 创建window对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method window_open
 * @annotation ["constructor", "scriptable"]
 * 从资源文件中加载并创建window对象。本函数在ui_loader/ui_builder_default里实现。
 * @param {char*} name window的名称。
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_open(const char* name);

/**
 * @method window_close
 * 关闭窗口。
 * @annotation ["deconstructor", "scriptable"]
 * @param {widget_t*} widget window对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_close(widget_t* widget);

#define WINDOW(widget) ((window_t*)(widget))

END_C_DECLS

#endif /*TK_WINDOW_H*/
