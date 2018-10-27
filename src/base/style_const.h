/**
 * File:   style_const.h
 * Author: AWTK Develop Team
 * Brief:  const style(can not be changed)
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_STYLE_CONST_H
#define TK_STYLE_CONST_H

#include "base/style.h"

BEGIN_C_DECLS

style_t* style_const_create(widget_t* widget);

END_C_DECLS

#endif /*TK_STYLE_CONST_H*/
