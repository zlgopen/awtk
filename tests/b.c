#if defined(_WIN32)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT /**< API export macro */
#endif
#include "tkc/utils.h"

DLL_EXPORT ret_t b_register(void) {
  log_debug("%s\n", __FUNCTION__);
  return RET_OK;
}

DLL_EXPORT ret_t b_unregister(void) {
  log_debug("%s\n", __FUNCTION__);
  return RET_OK;
}
