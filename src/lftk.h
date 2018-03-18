/**
 * File:   lftk.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lftk
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_MAIN_H
#define LFTK_MAIN_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @method lftk_init
 * 初始化LFTK。
 * @global
 * @scriptable no
 * @param {wh_t} w LCD宽度。
 * @param {wh_t} h LCD高度。
 * @param {uint32_t*} heap 动态内存空间。 
 * @param {uint32_t} size 动态内存空间的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lftk_init(wh_t w, wh_t h, uint32_t* heap, uint32_t size);

/**
 * @method lftk_run
 * 进入LFTK事件主循环。
 * @global
 * @scriptable no
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lftk_run(void);

/**
 * @method lftk_quit
 * 退出LFTK事件主循环。
 * @global
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lftk_quit(void);

ret_t lftk_init_resources();

END_C_DECLS

#endif/*LFTK_MAIN_H*/

