/**
 * File:   widget_animator_factory.h
 * Author: AWTK Develop Team
 * Brief:  widget animator factory
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
 * 2018-10-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget_animator_factory.h"

#ifdef WITHOUT_WIDGET_ANIMATOR
widget_animator_t* widget_animator_create(widget_t* widget, const char* params) {
  return NULL;
}
#endif /*WITHOUT_WIDGET_ANIMATOR*/
