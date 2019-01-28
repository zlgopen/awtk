/**
 * File:   view_model_normal.h
 * Author: AWTK Develop Team
 * Brief:  view_model_normal
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_NORMAL_H
#define TK_VIEW_MODEL_NORMAL_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _view_model_normal_t;
typedef struct _view_model_normal_t view_model_normal_t;

/**
 * @class view_model_normal_t
 * @parent view_model_t
 * @annotation ["scriptable"]
 *
 * normal view model。
 *
 */
struct _view_model_normal_t {
  view_model_t view_model;
};

view_model_t* view_model_normal_create(model_t* model);

#define VIEW_MODEL_NORMAL(view_model) ((view_model_normal_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_NORMAL_H*/
