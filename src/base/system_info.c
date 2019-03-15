/**
 * File:   system_info.h
 * Author: AWTK Develop Team
 * Brief:  system info
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "base/system_info.h"
#include "base/locale_info.h"

static system_info_t* s_system_info = NULL;

system_info_t* system_info() {
  return s_system_info;
}

ret_t system_info_set_app_info(system_info_t* info, app_type_t app_type, const char* app_name,
                               const char* app_root) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->app_type = app_type;
  info->app_root = app_root ? app_root : "./";
  info->app_name = app_name ? app_name : "AWTK Simulator";

  return RET_OK;
}

ret_t system_info_init(app_type_t app_type, const char* app_name, const char* app_root) {
  if (s_system_info == NULL) {
    s_system_info = system_info_create(app_type, app_name, app_root);
    return_value_if_fail(s_system_info != NULL, RET_BAD_PARAMS);
  } else {
    system_info_set_app_info(s_system_info, app_type, app_name, app_root);
  }

  return RET_OK;
}

ret_t system_info_deinit(void) {
  return_value_if_fail(s_system_info != NULL, RET_BAD_PARAMS);

  object_unref(OBJECT(s_system_info));
  s_system_info = NULL;

  return RET_OK;
}

static ret_t system_info_get_prop(object_t* obj, const char* name, value_t* v) {
  system_info_t* info = SYSTEM_INFO(obj);

  if (tk_str_eq(name, SYSTEM_INFO_PROP_LCD_W)) {
    value_set_int(v, info->lcd_w);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_LCD_H)) {
    value_set_int(v, info->lcd_h);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_LCD_TYPE)) {
    value_set_int(v, info->lcd_type);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_LCD_ORIENTATION)) {
    value_set_int(v, info->lcd_orientation);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_DEVICE_ORIENTATION)) {
    int32_t w = info->lcd_w;
    int32_t h = info->lcd_h;
    lcd_orientation_t orientation = info->lcd_orientation;
    if (orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_270) {
      w = info->lcd_h;
      h = info->lcd_w;
    }
    value_set_str(v, w < h ? "portrait" : "landscape");
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_DEVICE_PIXEL_RATIO)) {
    value_set_int(v, info->device_pixel_ratio);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_APP_TYPE)) {
    value_set_int(v, info->app_type);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_APP_NAME)) {
    value_set_str(v, info->app_name);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_APP_ROOT)) {
    value_set_str(v, info->app_root);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_LANGUAGE)) {
    value_set_str(v, locale_info()->language);
  } else if (tk_str_eq(name, SYSTEM_INFO_PROP_COUNTRY)) {
    value_set_str(v, locale_info()->country);
  } else {
    value_set_int(v, 0);
    return RET_FOUND;
  }

  return RET_OK;
}

static const object_vtable_t s_system_info_vtable = {.type = "system_info",
                                                     .desc = "system_info",
                                                     .size = sizeof(system_info_t),
                                                     .get_prop = system_info_get_prop};

system_info_t* system_info_create(app_type_t app_type, const char* app_name, const char* app_root) {
  object_t* obj = object_create(&s_system_info_vtable);
  system_info_t* info = SYSTEM_INFO(obj);
  return_value_if_fail(info != NULL, NULL);

  info->font_scale = 1;
  info->device_pixel_ratio = 1;
  system_info_set_app_info(info, app_type, app_name, app_root);

  return info;
}

ret_t system_info_set_font_scale(system_info_t* info, float_t font_scale) {
  return_value_if_fail(info != NULL && font_scale >= 0.5 && font_scale <= 2, RET_BAD_PARAMS);

  info->font_scale = font_scale;

  return RET_OK;
}

ret_t system_info_set_lcd_w(system_info_t* info, uint32_t lcd_w) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->lcd_w = lcd_w;

  return RET_OK;
}

ret_t system_info_set_lcd_h(system_info_t* info, uint32_t lcd_h) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->lcd_h = lcd_h;

  return RET_OK;
}

ret_t system_info_set_lcd_type(system_info_t* info, lcd_type_t lcd_type) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->lcd_type = lcd_type;

  return RET_OK;
}

ret_t system_info_set_lcd_orientation(system_info_t* info, lcd_orientation_t lcd_orientation) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->lcd_orientation = lcd_orientation;

  return RET_OK;
}

ret_t system_info_set_device_pixel_ratio(system_info_t* info, float_t device_pixel_ratio) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->device_pixel_ratio = device_pixel_ratio;

  return RET_OK;
}

static ret_t system_info_eval_one(system_info_t* info, str_t* str, const char* expr,
                                  tk_visit_t on_expr_result, void* ctx) {
  if (strchr(expr, '$') != NULL) {
    str_set(str, "");
    ENSURE(str_expand_vars(str, expr, OBJECT(info)) == RET_OK);
  } else {
    ENSURE(str_set(str, expr) == RET_OK);
  }

  return on_expr_result(ctx, str->str);
}

ret_t system_info_eval_exprs(system_info_t* info, const char* exprs, tk_visit_t on_expr_result,
                             void* ctx) {
  str_t str;
  char* p = NULL;
  char* expr = NULL;
  ret_t ret = RET_FAIL;
  char* dup_exprs = NULL;
  return_value_if_fail(exprs != NULL && on_expr_result != NULL, RET_BAD_PARAMS);

  dup_exprs = tk_strdup(exprs);
  return_value_if_fail(dup_exprs != NULL, RET_BAD_PARAMS);

  expr = dup_exprs;
  p = strchr(expr, ',');
  str_init(&str, 0);

  while (p != NULL) {
    *p = '\0';

    if ((ret = system_info_eval_one(info, &str, expr, on_expr_result, ctx)) == RET_OK) {
      goto done;
    }

    expr = p + 1;
    p = strchr(expr, ',');
  }

  ret = system_info_eval_one(info, &str, expr, on_expr_result, ctx);

done:
  str_reset(&str);
  TKMEM_FREE(dup_exprs);

  return ret;
}
