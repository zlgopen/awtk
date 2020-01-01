/**
 * File:   window_animator_factory.h
 * Author: AWTK Develop Team
 * Brief:  window animator factory
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
 * 2018-03-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_ANIMATOR_FACTORY_H
#define TK_WINDOW_ANIMATOR_FACTORY_H

#include "tkc/darray.h"
#include "base/window_animator.h"

BEGIN_C_DECLS

/**
 * @class window_animator_factory_t
 * 窗口动画工厂对象。
 *
 * 提供通过类型创建窗口动画的能力。
 * 让用户可以自定义窗口动画，并获得内置窗口动画同等待遇。
 *
 */
typedef struct _window_animator_factory_t {
  /*private*/
  darray_t creators;
} window_animator_factory_t;

/**
 * @method window_animator_factory
 * 获取缺省的窗口动画工厂对象。
 * @alias window_animator_factory_instance
 * @annotation ["constructor"]
 * @return {window_animator_factory_t*} 返回窗口动画工厂对象。
 */
window_animator_factory_t* window_animator_factory(void);

/**
 * @method window_animator_factory_set
 * 设置缺省的窗口动画工厂对象。
 * @param {window_animator_factory_t*} factory 窗口动画工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_factory_set(window_animator_factory_t* factory);

/**
 * @method window_animator_factory_create
 * 创建窗口动画工厂对象。
 * @annotation ["constructor"]
 *
 * @return {window_animator_factory_t*} 返回窗口动画工厂对象。
 */
window_animator_factory_t* window_animator_factory_create(void);

/**
 * @method window_animator_factory_register
 * 注册窗口动画创建函数。
 * @param {window_animator_factory_t*} factory 窗口动画工厂对象。
 * @param {const char*} type 窗口动画类型。
 * @param {window_animator_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_factory_register(window_animator_factory_t* factory, const char* type,
                                       window_animator_create_t create);

/**
 * @method window_animator_factory_create_animator
 * 创建指定类型的窗口动画。
 * @annotation ["constructor"]
 * @param {window_animator_factory_t*} factory 窗口动画工厂对象。
 * @param {bool_t} open TRUE表示打开窗口时的动画，FALSE表示关闭窗口时的动画。
 * @param {object_t*} args 参数。
 *
 * @return {window_animator_t*} 返回窗口动画对象。
 */
window_animator_t* window_animator_factory_create_animator(window_animator_factory_t* factory,
                                                           bool_t open, object_t* args);

/**
 * @method window_animator_factory_destroy
 * 析构并释放窗口动画工厂对象。
 * @param {window_animator_factory_t*} factory 窗口动画工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_factory_destroy(window_animator_factory_t* factory);

/*wrapper*/

/**
 * @method window_animator_create_for_open
 * @annotation ["constructor"]
 * 为打开窗口创建动画。
 * @param {char*} type 动画类型。
 * @param {canvas_t*} c canvas。
 * @param {widget_t*} prev_win 前一窗口。
 * @param {widget_t*} curr_win 当前窗口。
 *
 * @return {window_animator_t*} 窗口动画对象。
 */
window_animator_t* window_animator_create_for_open(const char* type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win);

/**
 * @method window_animator_create_for_close
 * @annotation ["constructor"]
 * 为关闭窗口创建动画。
 * @param {char*} type 动画类型。
 * @param {canvas_t*} c canvas。
 * @param {widget_t*} prev_win 前一窗口。
 * @param {widget_t*} curr_win 当前窗口。
 *
 * @return {window_animator_t*} 窗口动画对象。
 */
window_animator_t* window_animator_create_for_close(const char* type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win);
END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_FACTORY_H*/
