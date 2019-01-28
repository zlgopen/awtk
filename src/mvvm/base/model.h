/**
 * File:   model.h
 * Author: AWTK Develop Team
 * Brief:  model
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
 * 2019-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MODEL_H
#define TK_MODEL_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _model_t;
typedef struct _model_t model_t;

/**
 * @class model_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 模型的基类。
 *
 */
struct _model_t {
  object_t object;
};

#define MODEL(model) ((model_t*)(model))

END_C_DECLS

#endif /*TK_MODEL_H*/
