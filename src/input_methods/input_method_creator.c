/**
 * File:   input_method_creator.c
 * Author: AWTK Develop Team
 * Brief:  input method creator
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
 * 2018-08-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/system_info.h"
#include "input_method_null.inc"

#if !defined(WITH_NULL_IM) && !defined(MOBILE_APP)
#include "input_method_default.inc"
#endif /**/

#if defined(WITH_SDL)
#include "input_method_sdl.inc"
#endif /*WITH_SDL*/

#if defined(AWTK_WEB)
extern input_method_t* input_method_web_create(void);
#endif /*AWTK_WEB*/

input_method_t* input_method_create(void) {
  input_method_t* im = NULL;

#if defined(WITH_NULL_IM)
  im = input_method_null_create();
#elif defined(AWTK_WEB)
  im = input_method_web_create();
#elif defined(WITH_SDL)
#if defined(MOBILE_APP)
  im = input_method_sdl_create();
#else
  if (system_info()->app_type == APP_SIMULATOR) {
    im = input_method_default_create();
  } else {
    im = input_method_sdl_create();
  }
#endif /*MOBILE_APP*/
#else
  im = input_method_default_create();
#endif /*WITH_SDL*/

  return im;
}
