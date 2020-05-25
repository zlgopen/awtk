/**
 * File:   awtk.h
 * Author: AWTK Develop Team
 * Brief:  awtk
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
 * 2018-03-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GLOBAL_H
#define TK_GLOBAL_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class global_t
 * @annotation ["scriptable", "fake"]
 * TK全局对象。
 */

/**
 * @method tk_init
 * 初始化TK。
 * @alias init
 * @annotation ["static", "scriptable"]
 * @param {wh_t} w LCD宽度。
 * @param {wh_t} h LCD高度。
 * @param {app_type_t} app_type 应用程序的类型。
 * @param {const char*} app_name 应用程序的名称(必须为常量字符串)。
 * @param {const char*} app_root 应用程序的根目录，用于定位资源文件(必须为常量字符串)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_init(wh_t w, wh_t h, app_type_t app_type, const char* app_name, const char* app_root);

/**
 * @method tk_run
 * 进入TK事件主循环。
 * @alias run
 * @annotation ["static", "scriptable"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_run(void);

/**
 * @method tk_quit
 * 退出TK事件主循环。
 * @alias global_quit
 * @annotation ["static", "scriptable"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_quit(void);

/**
 * @method tk_get_pointer_x
 * 获取全局指针的X坐标。
 * @alias global_get_pointer_x
 * @annotation ["static", "scriptable"]
 *
 * @return {int32_t} 返回全局指针的X坐标。
 */
int32_t tk_get_pointer_x(void);

/**
 * @method tk_get_pointer_y
 * 获取全局指针的Y坐标。
 * @alias global_get_pointer_y
 * @annotation ["static", "scriptable"]
 *
 * @return {int32_t} 返回全局指针的X坐标。
 */
int32_t tk_get_pointer_y(void);

/**
 * @method tk_is_pointer_pressed
 * 获取全局指针是否按下。
 * @alias global_is_pointer_pressed
 * @annotation ["static", "scriptable"]
 *
 * @return {bool_t} 返回全局指针是否按下。
 */
bool_t tk_is_pointer_pressed(void);

/**
 * @method tk_set_lcd_orientation
 * 设置屏幕的旋转方向(XXX:目前仅支持0度和90度)。
 * @param {int} orientation 旋转方向。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_set_lcd_orientation(lcd_orientation_t orientation);

/**
 * @method tk_init_assets
 * 初始化资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_init_assets(void);

/**
 * @method tk_init_internal
 * init。
 *> public for test program
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_init_internal(void);

/**
 * @method tk_deinit_internal
 * deinit。
 *> public for test program
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_deinit_internal(void);

/**
 * @method tk_exit
 * public for web
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_exit(void);

END_C_DECLS

#endif /*TK_GLOBAL_H*/
