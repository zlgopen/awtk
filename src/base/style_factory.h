/**
 * File:   style_factory.h
 * Author: AWTK Develop Team
 * Brief:  style factory
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

#ifndef TK_STYLE_FACTORY_H
#define TK_STYLE_FACTORY_H

#include "base/style.h"

BEGIN_C_DECLS

style_t* style_factory_create_style(widget_t* widget);

END_C_DECLS

#endif /*TK_STYLE_FACTORY_H*/
