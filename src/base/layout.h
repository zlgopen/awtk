/**
 * File:   layout.h
 * Author: AWTK Develop Team
 * Brief:  widget layout
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LAYOUT_H
#define TK_LAYOUT_H

#include "tkc/darray.h"
#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @method widget_layout_self
 * 调用widget的selflayout。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout_self(widget_t* widget);
ret_t widget_layout_children(widget_t* widget);

/**
 * @method widget_layout_children_default
 * 调用widget默认的childrenlayout。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout_children_default(widget_t* widget);

/**
 * @method widget_layout_floating_children
 * 对floating属性的子控件进行layout。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout_floating_children(widget_t* widget);

/**
 * @method widget_get_children_for_layout
 * 获取受childrenlayout控件的子控件队列。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件。
 * @param {darray_t*} result 返回队列。
 * @param {bool_t} keep_disable 是否保留非enable状态的控件。
 * @param {bool_t} keep_invisible 是否保留非visible状态的控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_children_for_layout(widget_t* widget, darray_t* result, bool_t keep_disable,
                                     bool_t keep_invisible);

END_C_DECLS

#endif /*TK_LAYOUT_H*/
