/**
 * File:   lang_indicator.h
 * Author: AWTK Develop Team
 * Brief:  lang_indicator
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
 * 2020-04-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/input_method.h"
#include "keyboard/lang_indicator.h"

static ret_t lang_indicator_on_lang_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_invalidate_force(widget, NULL);

  return RET_OK;
}

static ret_t lang_indicator_get_prop(widget_t* widget, const char* name, value_t* v) {
  lang_indicator_t* lang_indicator = LANG_INDICATOR(widget);
  return_value_if_fail(lang_indicator != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, lang_indicator->image);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t lang_indicator_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return lang_indicator_set_image(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t lang_indicator_on_destroy(widget_t* widget) {
  lang_indicator_t* lang_indicator = LANG_INDICATOR(widget);
  input_method_off(input_method(), lang_indicator->event_id);

  lang_indicator->event_id = TK_INVALID_ID;
  TKMEM_FREE(lang_indicator->image);

  return RET_OK;
}

static ret_t lang_indicator_on_paint_self(widget_t* widget, canvas_t* c) {
  wstr_t* str = &(widget->text);
  lang_indicator_t* lang_indicator = LANG_INDICATOR(widget);
  const char* lang = input_method_get_lang(input_method());

  if (lang_indicator->image != NULL) {
    char icon[128];
    tk_snprintf(icon, sizeof(icon), "%s%s", lang_indicator->image, lang != NULL ? lang : "none");
    widget_paint_helper(widget, c, icon, NULL);
  } else {
    wstr_set_utf8(str, lang != NULL ? lang : "");
    widget_paint_helper(widget, c, NULL, NULL);
  }

  return RET_OK;
}

TK_DECL_VTABLE(lang_indicator) = {.size = sizeof(lang_indicator_t),
                                  .type = WIDGET_TYPE_LANG_INDICATOR,
                                  .parent = TK_PARENT_VTABLE(widget),
                                  .set_prop = lang_indicator_set_prop,
                                  .get_prop = lang_indicator_get_prop,
                                  .on_destroy = lang_indicator_on_destroy,
                                  .on_paint_self = lang_indicator_on_paint_self,
                                  .create = lang_indicator_create};

widget_t* lang_indicator_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(lang_indicator), x, y, w, h);
  lang_indicator_t* lang_indicator = LANG_INDICATOR(widget);

  lang_indicator->event_id =
      input_method_on(input_method(), EVT_IM_LANG_CHANGED, lang_indicator_on_lang_changed, widget);

  return widget;
}

widget_t* lang_indicator_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, lang_indicator), NULL);

  return widget;
}

ret_t lang_indicator_set_image(widget_t* widget, const char* image) {
  lang_indicator_t* lang_indicator = LANG_INDICATOR(widget);
  return_value_if_fail(lang_indicator != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  lang_indicator->image = tk_str_copy(lang_indicator->image, image);

  return RET_OK;
}
