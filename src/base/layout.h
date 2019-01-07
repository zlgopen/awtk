/**
 * File:   layout.h
 * Author: AWTK Develop Team
 * Brief:  widget layout
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LAYOUT_H
#define TK_LAYOUT_H

#include "base/types_def.h"

BEGIN_C_DECLS

ret_t widget_layout_self(widget_t* widget);
ret_t widget_layout_children(widget_t* widget);
ret_t widget_layout_children_default(widget_t* widget);

END_C_DECLS

#endif /*TK_LAYOUT_H*/
