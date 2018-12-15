/**
 * File:   system_bar.h
 * Author: AWTK Develop Team
 * Brief:  system_bar
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
 * 2018-12-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SYSTEM_BAR_H
#define TK_SYSTEM_BAR_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class system_bar_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * system_bar窗口。独占LCD顶部区域，用来显示系统状态和当前窗口的标题等信息。
 * system_bar对两个子控件会做特殊处理：
 * * 1.名为"title"的label控件，自动显示当前主窗口的name或text。
 * * 2.名为"close"的button控件，点击之后向当前主窗口发送EVT_REQUEST_CLOSE_WINDOW消息。
 */
typedef struct _system_bar_t {
  window_base_t window;
} system_bar_t;

/**
 * @method system_bar_create
 * @annotation ["constructor", "scriptable"]
 * 创建system_bar对象。
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} system_bar对象。
 */
widget_t* system_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method system_bar_cast
 * 转换为system_bar对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget system_bar对象。
 *
 * @return {widget_t*} system_bar对象。
 */
widget_t* system_bar_cast(widget_t* widget);

#define SYSTEM_BAR(widget) ((system_bar_t*)(widget))

END_C_DECLS

#endif /*TK_SYSTEM_BAR_H*/
