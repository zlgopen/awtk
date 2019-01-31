/**
 * File:   binding_context_awtk.c
 * Author: AWTK Develop Team
 * Brief:  binding context awtk
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
 * 2019-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/int_str.h"
#include "tkc/darray.h"
#include "base/widget.h"
#include "widgets/window.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/awtk/binding_context_awtk.h"

static ret_t on_widget_prop_change(void* ctx, event_t* e) {
  data_binding_t* rule = DATA_BINDING(ctx);
  prop_change_event_t* evt = prop_change_event_cast(e);

  data_binding_set_prop(rule, evt->value);

  /*TODO: check error*/

  return RET_OK;
}

static ret_t on_widget_value_change(void* ctx, event_t* e) {
  value_t v;
  widget_t* widget = WIDGET(e->target);
  data_binding_t* rule = DATA_BINDING(ctx);
  return_value_if_fail(widget_get_prop(widget, WIDGET_PROP_VALUE, &v) == RET_OK, RET_OK);

  data_binding_set_prop(rule, &v);
  /*TODO: check error*/

  return RET_OK;
}

static ret_t binding_context_bind_data(binding_context_t* ctx, const char* name,
                                       const char* value) {
  widget_t* widget = WIDGET(ctx->current_widget);
  data_binding_t* rule = (data_binding_t*)binding_rule_parse(name, value);
  return_value_if_fail(rule != NULL, RET_FAIL);

  BINDING_RULE(rule)->widget = widget;
  BINDING_RULE(rule)->binding_context = ctx;
  goto_error_if_fail(darray_push(&(ctx->data_bindings), rule) == RET_OK);

  if (rule->trigger != UPDATE_WHEN_EXPLICIT) {
    if (rule->mode == BINDING_TWO_WAY || rule->mode == BINDING_ONE_WAY_TO_MODEL) {
      if (tk_str_eq(rule->prop, WIDGET_PROP_VALUE)) {
        if (rule->trigger == UPDATE_WHEN_CHANGING) {
          widget_on(widget, EVT_VALUE_CHANGING, on_widget_value_change, rule);
        }
        widget_on(widget, EVT_VALUE_CHANGED, on_widget_value_change, rule);
      } else {
        widget_on(widget, EVT_PROP_CHANGED, on_widget_prop_change, rule);
      }
    }
  }

  return RET_OK;
error:
  object_unref(OBJECT(rule));

  return RET_FAIL;
}

/*TODO: add more event*/
static int_str_t s_event_map[] = {
  {EVT_CLICK, "on_click"}, 
  {EVT_POINTER_DOWN, "on_pointer_down"}, 
  {EVT_POINTER_UP, "on_pointer_up"}, 
  {EVT_KEY_DOWN, "on_key_down"}, 
  {EVT_KEY_UP, "on_key_up"}, 
  {EVT_NONE, NULL}};

static ret_t on_widget_event(void* ctx, event_t* e) {
  command_binding_t* rule = COMMAND_BINDING(ctx);

  if (command_binding_can_exec(rule)) {
    if (rule->update_model) {
      binding_context_update_to_model(BINDING_RULE(rule)->binding_context);
    }

    command_binding_exec(rule);

    if (rule->close_window) {
      widget_t* win = widget_get_window(BINDING_RULE(rule)->widget);
      window_close(win);
    }
  }

  return RET_OK;
}

static ret_t binding_context_bind_command(binding_context_t* ctx, const char* name,
                                          const char* value) {
  int32_t event = 0;
  widget_t* widget = WIDGET(ctx->current_widget);
  command_binding_t* rule = (command_binding_t*)binding_rule_parse(name, value);
  return_value_if_fail(rule != NULL, RET_FAIL);

  BINDING_RULE(rule)->widget = widget;
  BINDING_RULE(rule)->binding_context = ctx;
  goto_error_if_fail(darray_push(&(ctx->command_bindings), rule) == RET_OK);

  event = int_str_name(s_event_map, rule->event, EVT_NONE);
  if (event != EVT_NONE) {
    widget_on(widget, event, on_widget_event, rule);
  }

  return RET_OK;
error:
  object_unref(OBJECT(rule));

  return RET_FAIL;
}

static ret_t visit_bind_one_prop(void* ctx, const void* data) {
  binding_context_t* bctx = (binding_context_t*)(ctx);
  named_value_t* nv = (named_value_t*)data;

  if (tk_str_start_with(nv->name, BINDING_RULE_DATA_PREFIX)) {
    binding_context_bind_data(bctx, nv->name, value_str(&(nv->value)));
  } else if (tk_str_start_with(nv->name, BINDING_RULE_COMMAND_PREFIX)) {
    binding_context_bind_command(bctx, nv->name, value_str(&(nv->value)));
  }

  return RET_OK;
}

static ret_t visit_bind_one_widget(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);
  binding_context_t* bctx = (binding_context_t*)(ctx);

  if (widget->custom_props != NULL) {
    bctx->current_widget = widget;
    object_foreach_prop(widget->custom_props, visit_bind_one_prop, ctx);
  }

  return RET_OK;
}

static ret_t on_view_model_prop_change(void* ctx, event_t* e) {
  binding_context_update_to_view((binding_context_t*)ctx);

  return RET_OK;
}

static ret_t binding_context_awtk_bind(binding_context_t* ctx, view_model_t* vm, void* widget) {
  return_value_if_fail(widget_foreach(WIDGET(widget), visit_bind_one_widget, ctx) == RET_OK,
                       RET_FAIL);

  return_value_if_fail(binding_context_update_to_view(ctx) == RET_OK, RET_FAIL);
  emitter_on(EMITTER(vm), EVT_PROP_CHANGED, on_view_model_prop_change, ctx);
  ctx->bound = TRUE;

  return RET_OK;
}

static ret_t visit_data_binding_update_to_view(void* ctx, const void* data) {
  value_t v;
  data_binding_t* rule = DATA_BINDING(data);
  widget_t* widget = WIDGET(BINDING_RULE(rule)->widget);
  binding_context_t* bctx = BINDING_RULE(rule)->binding_context;

  if ((rule->mode == BINDING_ONCE && !(bctx->bound)) || rule->mode == BINDING_ONE_WAY ||
      rule->mode == BINDING_TWO_WAY) {
    return_value_if_fail(data_binding_get_prop(rule, &v) == RET_OK, RET_OK);
    return_value_if_fail(widget_set_prop(widget, rule->prop, &v) == RET_OK, RET_OK);
  }

  return RET_OK;
}

static ret_t binding_context_awtk_update_to_view(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  darray_foreach(&(ctx->data_bindings), visit_data_binding_update_to_view, ctx);

  return RET_OK;
}

static ret_t visit_data_binding_update_to_model(void* ctx, const void* data) {
  value_t v;
  data_binding_t* rule = DATA_BINDING(data);
  widget_t* widget = WIDGET(BINDING_RULE(rule)->widget);

  if (rule->mode == BINDING_TWO_WAY || rule->mode == BINDING_ONE_WAY_TO_MODEL) {
    return_value_if_fail(widget_get_prop(widget, rule->prop, &v) == RET_OK, RET_OK);
    return_value_if_fail(data_binding_set_prop(rule, &v) == RET_OK, RET_OK);
  }

  return RET_OK;
}

static ret_t binding_context_awtk_update_to_model(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  darray_foreach(&(ctx->data_bindings), visit_data_binding_update_to_model, ctx);

  return RET_OK;
}

static ret_t binding_context_awtk_destroy(binding_context_t* ctx) {
  TKMEM_FREE(ctx);

  return RET_OK;
}

static const binding_context_vtable_t s_binding_context_vtable = {
    .bind = binding_context_awtk_bind,
    .update_to_view = binding_context_awtk_update_to_view,
    .update_to_model = binding_context_awtk_update_to_model,
    .destroy = binding_context_awtk_destroy};

binding_context_t* binding_context_create(void) {
  binding_context_t* ctx = TKMEM_ZALLOC(binding_context_t);
  return_value_if_fail(ctx != NULL, NULL);

  ctx->vt = &s_binding_context_vtable;
  if (binding_context_init(ctx) != RET_OK) {
    binding_context_destroy(ctx);
    ctx = NULL;
  }

  return ctx;
}

static ret_t on_widget_destroy(void* ctx, event_t* e) {
  binding_context_t* bctx = (binding_context_t*)ctx;

  emitter_off_by_ctx(EMITTER(bctx->vm), bctx);
  binding_context_destroy((binding_context_t*)(ctx));

  return RET_REMOVE;
}

ret_t binding_context_bind_simple(view_model_t* vm, widget_t* widget) {
  binding_context_t* ctx = NULL;
  return_value_if_fail(vm != NULL && widget != NULL, RET_BAD_PARAMS);

  ctx = binding_context_create();
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(binding_context_bind(ctx, vm, widget) == RET_OK);
  widget_on(widget, EVT_DESTROY, on_widget_destroy, ctx);

  return RET_OK;
error:
  binding_context_destroy(ctx);

  return RET_FAIL;
}
