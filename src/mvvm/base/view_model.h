/**
 * File:   view_model.h
 * Author: AWTK Develop Team
 * Brief:  view_model
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

#ifndef TK_VIEW_MODEL_H
#define TK_VIEW_MODEL_H

#include "tkc/str.h"
#include "mvvm/base/model.h"

BEGIN_C_DECLS

struct _view_model_t;
typedef struct _view_model_t view_model_t;

typedef enum _view_model_type_t {
  VIEW_MODEL_NORMAL = 0,
  VIEW_MODEL_ARRAY,
  VIEW_MODEL_ITEM,
  VIEW_MODEL_TREE
} view_model_type_t;

/**
 * @class view_model_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * ViewModel。
 *
 */
struct _view_model_t {
  object_t object;

  model_t* model;

  view_model_type_t type;

  str_t last_error;
};

view_model_t* view_model_init(view_model_t* vm, view_model_type_t type, model_t* model);

bool_t view_model_has_prop(view_model_t* vm, const char* name);
ret_t view_model_eval(view_model_t* vm, const char* expr, value_t* value);
ret_t view_model_get_prop(view_model_t* vm, const char* name, value_t* value);
ret_t view_model_set_prop(view_model_t* vm, const char* name, const value_t* value);

bool_t view_model_can_exec(view_model_t* vm, const char* name, const char* args);
ret_t view_model_exec(view_model_t* vm, const char* name, const char* args);

ret_t view_model_deinit(view_model_t* vm);

#define VIEW_MODEL(view_model) ((view_model_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_H*/
