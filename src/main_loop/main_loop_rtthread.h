/**
 * file:   main_loop_rtthread.h
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  rtthread implemented main_loop interface
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
 * 2018-02-22 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_MAIN_LOOP_RTTHREAD_H
#define LFTK_MAIN_LOOP_RTTHREAD_H

#include "base/main_loop.h"

BEGIN_C_DECLS

main_loop_t* main_loop_rtthread_init(int w, int h);

END_C_DECLS

#endif/*LFTK_MAIN_LOOP_RTTHREAD_H*/

