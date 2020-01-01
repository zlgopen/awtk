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

#include "base/ui_feedback.h"

static void* s_on_feedback_ctx;
static ui_on_feedback_t s_on_feedback;

ret_t ui_feedback_request(widget_t* widget, event_t* evt) {
  return_value_if_fail(widget != NULL && widget->feedback && evt != NULL, RET_BAD_PARAMS);

  if (s_on_feedback != NULL) {
    s_on_feedback(s_on_feedback_ctx, widget, evt);
  }

  return RET_OK;
}

ret_t ui_feedback_init(ui_on_feedback_t on_feedback, void* ctx) {
  s_on_feedback = on_feedback;
  s_on_feedback_ctx = ctx;

  return RET_OK;
}
