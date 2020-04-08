#include "awtk.h"
#include "base/assets_manager.h"

ret_t assets_init(void) {
  assets_manager_t* am = assets_manager();

  tk_init_assets();
  return RET_OK;
}

bool_t assets_has_theme(const char* name) {
  return_value_if_fail(name != NULL, FALSE);

  if (tk_str_eq(name, "default")) {
    return TRUE;
  } else {
    return FALSE;
  }
}

ret_t assets_set_global_theme(const char* name) {
  log_debug("not support to change theme.\n");
  return RET_NOT_IMPL;
}
