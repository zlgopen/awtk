#include "awtk.h"
#include "base/assets_manager.h"

ret_t assets_init(void) {
  assets_manager_t* am = assets_manager();

  tk_init_assets();
  return RET_OK;
}
