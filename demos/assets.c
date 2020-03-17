#include "awtk.h"
#include "base/assets_manager.h"

extern ret_t assets_init_default(void);
extern ret_t assets_init_dark(void);

ret_t assets_init(const char* theme) {
  assets_manager_t* am = assets_manager();
  return_value_if_fail(theme != NULL && am != NULL, RET_BAD_PARAMS);

  assets_manager_set_theme(am, theme);

  if (tk_str_eq(theme, "default")) {
    return assets_init_default();
  } else if (tk_str_eq(theme, "dark")) {
    return assets_init_dark();
  } else { 
    log_debug("%s not support.\n", theme);
    return RET_NOT_IMPL;
  }
}