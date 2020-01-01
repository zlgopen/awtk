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

#ifndef TK_WIDGET_ANIMATOR_FACTORY_H
#define TK_WIDGET_ANIMATOR_FACTORY_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @method widget_animator_create
 * @annotation ["global"]
 * 创建animator对象。
 * @param {widget_t*} widget 动画应用的控件。
 * @param {const char*} params 参数。
 *
 * @return {widget_animator_t*} 动画对象。
 */
widget_animator_t* widget_animator_create(widget_t* widget, const char* params);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_FACTORY_H*/
