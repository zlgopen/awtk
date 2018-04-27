/**
 * file:   main_loop_stm32_raw.h
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  stm32_raw implemented main_loop interface
 *
 * copyright (c) 2018 - 2018  li xianjing <xianjimli@hotmail.com>
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-02-17 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MAIN_LOOP_STM32_RAW_H
#define TK_MAIN_LOOP_STM32_RAW_H

#include "base/main_loop.h"

BEGIN_C_DECLS

main_loop_t* main_loop_init(int w, int h);

END_C_DECLS

#endif /*TK_MAIN_LOOP_STM32_RAW_H*/
