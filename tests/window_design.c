#include "base/window_base.h"
#include "base/window.h"

widget_t* window_design_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

image_manager_t* t_image_manager = (image_manager_t*)(((char*)NULL) + 2);
locale_info_t* t_locale_info = (locale_info_t*)(((char*)NULL) + 3);
assets_manager_t* t_assets_manager = (assets_manager_t*)(((char*)NULL) + 4);
font_manager_t* t_font_manager = (font_manager_t*)(((char*)NULL) + 5);

ret_t window_design_get_prop(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_DEFAULT_THEME_OBJ)) {
    value_set_pointer(v, theme());
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE_MANAGER)) {
    value_set_pointer(v, (void*)(t_image_manager));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALE_INFO)) {
    value_set_pointer(v, (void*)(t_locale_info));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FONT_MANAGER)) {
    value_set_pointer(v, (void*)(t_font_manager));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ASSETS_MANAGER)) {
    value_set_pointer(v, (void*)(t_assets_manager));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

TK_DECL_VTABLE(window_design) = {.type = WIDGET_TYPE_NORMAL_WINDOW,
                                 .size = sizeof(window_t),
                                 .is_window = TRUE,
                                 .parent = TK_PARENT_VTABLE(window_base),
                                 .create = window_design_create,
                                 .on_event = window_base_on_event,
                                 .on_paint_self = window_base_on_paint_self,
                                 .on_paint_begin = window_base_on_paint_begin,
                                 .on_paint_end = window_base_on_paint_end,
                                 .get_prop = window_design_get_prop,
                                 .on_destroy = window_base_on_destroy};

widget_t* window_design_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(window_design), x, y, w, h);
}
