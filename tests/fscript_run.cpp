#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/fscript.h"
#include "tkc/object_default.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  if (argc != 2) {
    printf("Usage: %s script\n", argv[0]);
    return 0;
  } else {
    value_t v;
    uint64_t start = time_now_us();
    object_t* obj = object_default_create();
    fscript_eval(obj, argv[1], &v);
    value_reset(&v);
    OBJECT_UNREF(obj);
    log_debug("cost: %d us\n", (int)(time_now_us() - start));
  }

  return 0;
}
