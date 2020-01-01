/**
 * File:   ui_feedback.h
 * Author: AWTK Develop Team
 * Brief:  ui_feedback
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_FEEDBACK_H
#define TK_UI_FEEDBACK_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef ret_t (*ui_on_feedback_t)(void* ctx, widget_t* widget, event_t* evt);

/**
 * @class ui_feedback_t
 * @annotation ["fake"]
 *
 * 提供按键音、触屏音和震动等反馈。
 */

/**
 * @method ui_feedback_request
 * 请求反馈。
 *
 * @annotation ["static"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} evt event对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ui_feedback_request(widget_t* widget, event_t* evt);

/**
 * @method ui_feedback_init
 * 初始化。
 * 设置实际的处理函数。
 *
 * @annotation ["static"]
 * @param {ui_on_feedback_t} on_feedback 实际的处理函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ui_feedback_init(ui_on_feedback_t on_feedback, void* ctx);

END_C_DECLS

#endif /*TK_UI_FEEDBACK_H*/
