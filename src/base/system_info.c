/**
 * File:   system_info.h
 * Author: AWTK Develop Team
 * Brief:  system info
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
 * 2018-04-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "base/system_info.h"
#include "base/locale_info.h"

#ifdef AWTK_WEB
#include <emscripten.h>
#endif /*AWTK_WEB*/

static system_info_t* s_system_info = NULL;

system_info_t* system_info() {
  return s_system_info;
}

#ifdef WITH_FS_RES
static bool_t app_root_is_valid(const char* app_root) {
  char res_root[MAX_PATH + 1];
  return_value_if_fail(app_root != NULL, FALSE);

  path_build(res_root, MAX_PATH, app_root, "assets", NULL);

  return path_exist(res_root);
}

static ret_t system_info_set_app_root(system_info_t* info, const char* app_root) {
  if (app_root != NULL) {
    info->app_root = tk_strdup(app_root);
    log_debug("app_root=%s\n", app_root);
  }

  return RET_OK;
}

static ret_t system_info_normalize_app_root_try_default(system_info_t* info,
                                                        const char* app_root_default) {
  if (app_root_is_valid(app_root_default)) {
    return system_info_set_app_root(info, app_root_default);
  } else if (!path_is_abs(app_root_default)) {
    char path[MAX_PATH + 1] = {0};

    if (path_exe(path) == RET_OK) {
      char app_root[MAX_PATH + 1] = {0};
      char* last = NULL;

      last = strrchr(path, TK_PATH_SEP);
      if (last != NULL) {
        *last = '\0';
      }

      path_build(app_root, MAX_PATH, path, app_root_default, NULL);
      if (app_root_is_valid(app_root)) {
        return system_info_set_app_root(info, app_root);
      }
    }

    return RET_FAIL;
  }

  return RET_FAIL;
}

static ret_t system_info_normalize_app_root_try_path(system_info_t* info, char* path) {
  char* last = NULL;
  char app_root[MAX_PATH + 1] = {0};

  log_debug("try %s\n", path);
  last = strrchr(path, TK_PATH_SEP);
  if (last != NULL) {
    if (tk_str_eq(last + 1, "bin")) {
      *last = '\0';
    }
  }

  if (!app_root_is_valid(path)) {
    path_build(app_root, MAX_PATH, path, "res", NULL);
    log_debug("try set app_root:%s\n", app_root);
    if (app_root_is_valid(app_root)) {
      return system_info_set_app_root(info, app_root);
    } else {
      path_build(app_root, MAX_PATH, path, "demos", NULL);
     log_debug("try set app_root:%s\n", app_root);
      if (app_root_is_valid(app_root)) {
        return system_info_set_app_root(info, app_root);
      } else {
        return RET_FAIL;
      }
    }
  } else {
    return system_info_set_app_root(info, path);
  }
}

static ret_t system_info_normalize_app_root_try_cwd(system_info_t* info) {
  char path[MAX_PATH + 1] = {0};
  return_value_if_fail(path_cwd(path) == RET_OK, RET_FAIL);

  return system_info_normalize_app_root_try_path(info, path);
}

static ret_t system_info_normalize_app_root_try_exe(system_info_t* info) {
  char* last = NULL;
  char path[MAX_PATH + 1] = {0};
  return_value_if_fail(path_exe(path) == RET_OK, RET_FAIL);

  last = strrchr(path, TK_PATH_SEP);
  if (last != NULL) {
    *last = '\0';
  }

  return system_info_normalize_app_root_try_path(info, path);
}

static ret_t system_info_normalize_app_root(system_info_t* info, const char* app_root_default) {
  if (system_info_normalize_app_root_try_default(info, app_root_default) == RET_OK) {
    return RET_OK;
  } else if (system_info_normalize_app_root_try_cwd(info) == RET_OK) {
    return RET_OK;
  } else if (system_info_normalize_app_root_try_exe(info) == RET_OK) {
    return RET_OK;
  } else {
    system_info_set_app_root(info, "");
  }

  log_debug("Not found valid assets folder!\n");

  return RET_FAIL;
}
#else
static ret_t system_info_normalize_app_root(system_info_t* info, const char* app_root_default) {
  info->app_root = tk_strdup(app_root_default);

  return RET_OK;
}
#endif /*WITH_FS_RES*/

ret_t system_info_set_app_info(system_info_t* info, app_type_t app_type, const char* app_name,
                               const char* app_root) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  info->app_type = app_type;
  system_info_normalize_app_root(info, app_root);
  info->app_name = app_name ? app_name : "AWTK Simulator";

  return RET_OK;
}

ret_t system_info_init(app_type_t app_type, const char* app_name, const char* app_root) {
  if (s_system_info == NULL) {
    s_system_info = system_info_create(app_type, app_name, app_root);
    system_info_set_default_font(s_system_info, "default");
    return_value_if_fail(s_system_info != NULL, RET_BAD_PARAMS);
  } else {
    system_info_set_app_info(s_system_info, app_type, app_name, app_root);
  }

#ifdef AWTK_WEB
  {
    float_t font_scale = EM_ASM_DOUBLE({ return (TBrowser.config.fontScale || 1); }, 0);
    system_info_set_font_scale(s_system_info, font_scale);
    log_debug("system_info_init: font_scale=%lf\n", font_scale);
  }
#endif /*AWTK_WEB*/

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

static ret_t system_info_on_destroy(object_t* obj) {
  system_info_t* info = SYSTEM_INFO(obj);
  return_value_if_fail(info != NULL, RET_FAIL);

  TKMEM_FREE(info->app_root);

  return RET_OK;
}

static const object_vtable_t s_system_info_vtable = {.type = "system_info",
                                                     .desc = "system_info",
                                                     .size = sizeof(system_info_t),
                                                     .get_prop = system_info_get_prop,
                                                     .on_destroy = system_info_on_destroy};

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
  return_value_if_fail(info != NULL && font_scale >= 0.5f && font_scale <= 2, RET_BAD_PARAMS);

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
  bool_t not_schema = strstr(expr, STR_SCHEMA_FILE) == NULL &&
                      strstr(expr, STR_SCHEMA_HTTP) == NULL &&
                      strstr(expr, STR_SCHEMA_HTTPS) == NULL;

  if (not_schema && strchr(expr, '$') != NULL) {
    str_set(str, "");

    if (str_expand_vars(str, expr, OBJECT(info)) == RET_OK) {
      return on_expr_result(ctx, str->str);
    }
  }

  ENSURE(str_set(str, expr) == RET_OK);
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

ret_t system_info_set_default_font(system_info_t* info, const char* default_font) {
  return_value_if_fail(info != NULL && default_font != NULL, RET_BAD_PARAMS);

  info->default_font = default_font;

  return RET_OK;
}

const char* system_info_fix_font_name(const char* name) {
  return_value_if_fail(system_info() != NULL, name);

  return (name && *name) ? name : system_info()->default_font;
}
