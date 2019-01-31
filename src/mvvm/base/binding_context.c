/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
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
 * 2019-01-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "mvvm/base/binding_context.h"

ret_t binding_context_init(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  darray_init(&(ctx->command_bindings), 10, (tk_destroy_t)object_unref,
              (tk_compare_t)object_compare);
  darray_init(&(ctx->data_bindings), 10, (tk_destroy_t)object_unref, (tk_compare_t)object_compare);

  return RET_OK;
}

ret_t binding_context_bind(binding_context_t* ctx, view_model_t* vm, void* widget) {
  return_value_if_fail(vm != NULL && widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->bind != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->bound == FALSE, RET_BAD_PARAMS);

  ctx->vm = vm;
  ctx->widget = widget;

  return ctx->vt->bind(ctx, vm, widget);
}

ret_t binding_context_update_to_view(binding_context_t* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->update_to_view != NULL,
                       RET_BAD_PARAMS);

  if(ctx->updating_view) {
    return RET_BUSY;
  } 

  ctx->updating_view = TRUE;
  ret = ctx->vt->update_to_view(ctx);
  ctx->updating_view = FALSE;

  return ret;
}

ret_t binding_context_update_to_model(binding_context_t* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->update_to_model != NULL,
                       RET_BAD_PARAMS);

  if(ctx->updating_model) {
    return RET_BUSY;
  } 

  ctx->updating_model = TRUE;
  ret = ctx->vt->update_to_model(ctx);
  ctx->updating_model = FALSE;

  return ret;
}

ret_t binding_context_destroy(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  darray_deinit(&(ctx->command_bindings));
  darray_deinit(&(ctx->data_bindings));

  if (ctx->vt->destroy != NULL) {
    ctx->vt->destroy(ctx);
  }

  return RET_OK;
}
