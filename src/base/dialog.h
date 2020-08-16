/**
 * File:   dialog.h
 * Author: AWTK Develop Team
 * Brief:  dialog
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIALOG_H
#define TK_DIALOG_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @enum dialog_quit_code_t
 * @annotation ["scriptable"]
 * @prefix DIALOG_QUIT_
 * 对话框退出码。 
 * 
 * > 一般用作dialog_quit函数的参数。
 */
typedef enum _dialog_quit_code_t {
  /**
   * @const DIALOG_QUIT_NONE
   * 对话框被强行关闭或不关心关闭原因。
   */
  DIALOG_QUIT_NONE = 0,

  /**
   * @const DIALOG_QUIT_OK
   * 点击“OK”按钮关闭。
   */
  DIALOG_QUIT_OK = 1,

  /**
   * @const DIALOG_QUIT_YES
   * 点击“YES”按钮关闭。
   */
  DIALOG_QUIT_YES = 1,

  /**
   * @const DIALOG_QUIT_CANCEL
   * 点击“CANCEL”按钮关闭。
   */
  DIALOG_QUIT_CANCEL = 2,

  /**
   * @const DIALOG_QUIT_NO
   * 点击“NO”按钮关闭。
   */
  DIALOG_QUIT_NO = 2,

  /**
   * @const DIALOG_QUIT_OTHER
   * 点击其它按钮关闭。
   */
  DIALOG_QUIT_OTHER
} dialog_quit_code_t;

/**
 * @class dialog_t
 * @parent window_base_t
 * @annotation ["scriptable","design","widget","window"]
 *
 * 对话框。 对话框是一种特殊的窗口，大小和位置可以自由设置。
 *
 * AWTK中的对话框可以是模态的，也可以是非模态的。
 *
 * 如果dialog有透明或半透效果则不支持窗口动画。
 *
 *> 由于浏览器中无法实现主循环嵌套，因此无法实现模态对话框。
 * 如果希望自己写的AWTK应用程序可以在浏览器(包括各种小程序)中运行或演示，
 * 请避免使用模态对话框。
 *
 * 对话框通常由对话框标题和对话框客户区两部分组成：
 *
 * ```graphviz
 *   [default_style]
 *
 *   dialog_title_t -> dialog_t[arrowhead = "ediamond"]
 *   dialog_client_t -> dialog_t[arrowhead = "ediamond"]
 * ```
 *
 * dialog\_t是[window\_base\_t](window_base_t.md)的子类控件，window\_base\_t的函数均适用于dialog\_t控件。
 *
 * 在xml中使用"dialog"标签创建对话框。如：
 *
 * ```xml
 * <dialog anim_hint="center_scale(duration=300)" x="c" y="m" w="80%" h="160" text="Dialog">
 * <dialog_title x="0" y="0" w="100%" h="30" text="Hello AWTK" />
 * <dialog_client x="0" y="bottom" w="100%" h="-30">
 *   <label name="" x="center" y="middle:-20" w="200" h="30" text="Are you ready?"/>
 *   <button name="quit" x="10" y="bottom:10" w="40%" h="30" text="确定"/>
 *   <button name="quit" x="right:10" y="bottom:10" w="40%" h="30" text="取消"/>
 * </dialog_client>
 * </dialog>
 * ```
 *
 * 如果你不需要对话框的标题，可以这样写：
 *
 * ```xml
 * <dialog anim_hint="center_scale(duration=300)" x="c" y="m" w="80%" h="160" text="Dialog">
 *   <label name="" x="center" y="middle:-20" w="200" h="30" text="Are you ready?"/>
 *   <button name="quit" x="10" y="bottom:10" w="40%" h="30" text="确定"/>
 *   <button name="quit" x="right:10" y="bottom:10" w="40%" h="30" text="取消"/>
 * </dialog>
 * ```
 *
 * 打开非模态对话框时，其用法与普通窗口一样。打开非模态对话框时，还需要调用dialog\_modal。
 *
 * ```c
 *  widget_t* dlg = dialog_open(name);
 *
 *  ret = dialog_modal(dlg);
 * ```
 *
 * 关闭模态对话框用dialog\_quit
 *
 * ```c
 * static ret_t on_dialog_btn_click(void* ctx, event_t* evt) {
 *   widget_t* win = widget_get_window(WIDGET(evt->target));
 *   int code = (char*)ctx - (char*)NULL;
 *
 *   dialog_quit(win, code);
 *
 *    return RET_OK;
 * }
 * ```
 *
 * 关闭非模态对话框用window\_close。
 *
 * ```c
 * static ret_t on_dialog_btn_click(void* ctx, event_t* evt) {
 *   widget_t* win = widget_get_window(WIDGET(evt->target));
 *   int code = (char*)ctx - (char*)NULL;
 *
 *   window_close(win);
 *
 *   return RET_OK;
 * }
 * ```
 *
 * > 更多用法请参考：
 * [dialog.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * > 完整C代码示例请参考：
 *
 * * [非模态对话框](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/dialog.c)
 *
 * * [模态对话框](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/dialog_modal.c)
 *
 *可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <style name="default">
 *   <normal border_color="#606060" />
 * </style>
 * ```
 *
 * > 更多用法请参考：
 * [theme default]
 * (https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L324)
 *
 */
typedef struct _dialog_t {
  window_base_t window;

  /**
   * @property {const char*} highlight
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 对话框高亮策略。
   * 
   * > 请参考 [对话框高亮策略](https://github.com/zlgopen/awtk/blob/master/docs/dialog_highlight.md)
   */
  char* highlight;

  /*private*/
  widget_t* title;
  widget_t* client;
  dialog_quit_code_t quit_code;
  bool_t quited;
  bool_t is_model;
} dialog_t;

/**
 * @method dialog_create
 * 创建dialog对象。
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} dialog对象。
 */
widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method dialog_create_simple
 * 创建dialog对象，同时创建title/client。
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} dialog对象。
 */
widget_t* dialog_create_simple(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method dialog_cast
 * 转换dialog对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget dialog对象。
 *
 * @return {widget_t*} dialog对象。
 */
widget_t* dialog_cast(widget_t* widget);

/**
 * @method dialog_get_title
 * 获取title控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 *
 * @return {widget_t*} title对象。
 */
widget_t* dialog_get_title(widget_t* widget);

/**
 * @method dialog_get_client
 * 获取client控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 *
 * @return {widget_t*} client对象。
 */
widget_t* dialog_get_client(widget_t* widget);

/**
 * @method dialog_open
 * @annotation ["constructor", "scriptable"]
 * 从资源文件中加载并创建Dialog对象。
 *
 * 本函数在ui\_loader/ui\_builder_default里实现。
 * @param {const char*} name dialog的名称。
 *
 * @return {widget_t*} 对象。
 */
widget_t* dialog_open(const char* name);

/**
 * @method dialog_set_title
 * 设置对话框的标题文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 * @param {char*}  title 标题。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_set_title(widget_t* widget, const char* title);

/**
 * @method dialog_modal
 * 模态显示对话框。
 * dialog_modal返回后，dialog对象将在下一个idle函数中回收。
 * 也就是在dialog_modal调用完成后仍然可以访问dialog中控件，直到本次事件结束。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 *
 * @return {dialog_quit_code_t} 返回退出吗。
 */
dialog_quit_code_t dialog_modal(widget_t* widget);

/**
 * @method dialog_quit
 * 退出模态显示，关闭对话框。
 *
 *> 比如，在对话框中关闭按钮的事件处理函数中，调用本函数关闭对话框。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 * @param {uint32_t} code 退出码，作为dialog_modal的返回值(参考：[dialog_quit_code_t](dialog_quit_code_t.md))。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_quit(widget_t* widget, uint32_t code);

/**
 * @method dialog_is_quited
 * 检查对话框是否已经退出模态。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 *
 * @return {bool_t} 返回TRUE表示已经退出，否则表示没有。
 */
bool_t dialog_is_quited(widget_t* widget);

/**
 * @method dialog_is_modal
 * 检查对话框是否为模态对话框。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dialog对象。
 *
 * @return {bool_t} 返回TRUE表示是模态对话框，否则表示不是。
 */
bool_t dialog_is_modal(widget_t* widget);

/**
 * @method dialog_toast
 * 显示『短暂提示信息』对话框。
 *
 * 主题由dialog_toast.xml文件决定。
 *
 * @annotation ["static", "scriptable"]
 * @param {const char*} text 文本内容。
 * @param {uint32_t} duration 显示时间(单位为毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_toast(const char* text, uint32_t duration);

/**
 * @method dialog_info
 * 显示『提示信息』对话框。
 *
 * 主题由dialog_info.xml文件决定。
 *
 * @annotation ["static", "scriptable"]
 * @param {const char*} title 标题。
 * @param {const char*} text 文本内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_info(const char* title, const char* text);

/**
 * @method dialog_warn
 * 显示『警告』对话框。
 *
 * 主题由dialog_warn.xml文件决定。
 *
 * @annotation ["static", "scriptable"]
 * @param {const char*} title 标题。
 * @param {const char*} text 文本内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_warn(const char* title, const char* text);

/**
 * @method dialog_confirm
 * 显示『确认』对话框。
 *
 * 主题由dialog_confirm.xml文件决定。
 *
 * @annotation ["static", "scriptable"]
 * @param {const char*} title 标题。
 * @param {const char*} text 文本内容。
 *
 * @return {ret_t} 返回RET_OK表示确认，否则表示取消。
 */
ret_t dialog_confirm(const char* title, const char* text);

#define DIALOG(widget) ((dialog_t*)(dialog_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(dialog);

END_C_DECLS

#endif /*TK_DIALOG_H*/
