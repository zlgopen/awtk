/**
 * File:   rich_text.h
 * Author: AWTK Develop Team
 * Brief:  rich_text
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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RICH_TEXT_H
#define TK_RICH_TEXT_H

#include "base/widget.h"
#include "base/velocity.h"
#include "base/widget_animator.h"
#include "rich_text/rich_text_render_node.h"

BEGIN_C_DECLS

/**
 * @class rich_text_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 图文混排控件，实现简单的图文混排。
 *
 * rich\_text\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于rich\_text\_t控件。
 *
 * 在xml中使用"rich\_text"标签创建图文混排控件。如：
 *
 * ```xml
 * <rich_text x="0" y="0" w="100%" h="60"
 * text="<image name=&quota;bricks&quota;/><font align_v=&quota;middle&quota;>hello awtk!</font>" />
 * ```
 * >
 *
 * > 更多用法请参考：
 * [rich_text.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/rich_text.xml)
 *
 * 在c代码中使用函数rich\_text\_create创建图文混排控件。如：
 *
 * ```c
 *  widget_t* rich_text = rich_text_create(win, 0, 0, 0, 0);
 *  widget_set_text_utf8(rich_text,
 *                    "<image name=\"bricks\"/><font color=\"gold\" align_v=\"bottom\" "
 *                    "size=\"24\">hello awtk!</font><font color=\"green\" size=\"20\">ProTip! The "
 *                    "feed shows you events from people you follow and repositories you watch. "
 *                    "nhello world. </font><font color=\"red\" "
 *                    "size=\"20\">确定取消中文字符测试。确定。取消。中文字符测试。</font>");
 *
 * widget_set_self_layout_params(rich_text, "center", "middle", "100%", "100%");
 * ```
 *
 * > 完整示例请参考：
 * [rich_text demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/rich_text.c)
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等等。而字体的大小和颜色则由文本内部的属性决定。
 *
 * 目前支持image和font两个tag：
 *
 * * image 支持的属性有：
 *   * name 图片的名称。
 *   * w 图片的宽度。
 *   * h 图片的高度。
 *
 * * font 支持的属性有：
 *   * color 颜色。
 *   * size 大小。
 *   * align_v 垂直方向对齐的方式(top|middle|bottom)。
 *   * bold 粗体(暂不支持)
 *   * italic 斜体(暂不支持)
 *   * underline 下划线(暂不支持)
 *
 */
typedef struct _rich_text_t {
  widget_t widget;

  /**
   * @property {uint32_t} line_gap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 行间距。
   */
  uint32_t line_gap;

  /**
   * @property {uint32_t} margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 边距。
   */
  uint32_t margin;

  /**
   * @property {int32_t} yoffset
   * @annotation ["set_prop","get_prop","readable"]
   * y偏移。
   */
  int32_t yoffset;

  /**
   * @property {bool_t} need_reset
   * @annotation ["readable"]
   * 标识控件是否需要重新绘图。
   */
  bool_t need_reset;

  /*private*/
  bool_t pressed;
  int32_t ydown;
  int32_t yoffset_end;
  uint32_t content_h;
  rich_text_node_t* node;
  widget_animator_t* wa;
  velocity_t velocity;
  int32_t yoffset_save;
  rich_text_render_node_t* render_node;
} rich_text_t;

/**
 * @method rich_text_create
 * 创建rich_text对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* rich_text_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method rich_text_set_text
 * 设置文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rich_text_set_text(widget_t* widget, const char* text);

/**
 * @method rich_text_cast
 * 转换为rich_text对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget rich_text对象。
 *
 * @return {widget_t*} rich_text对象。
 */
widget_t* rich_text_cast(widget_t* widget);

#define RICH_TEXT(widget) ((rich_text_t*)(rich_text_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(rich_text);

END_C_DECLS

#endif /*TK_RICH_TEXT_H*/
