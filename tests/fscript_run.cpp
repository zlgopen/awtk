#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/fscript.h"
#include "tkc/object_default.h"

static ret_t run_fscript(const char* code, uint32_t times) {
  value_t v;
  char buff[64];
  uint64_t start = time_now_us();
  object_t* obj = object_default_create();
  tk_mem_dump();
  if (times > 1) {
    /*stress test*/
    uint32_t i = 0;
    fscript_t* fscript = fscript_create(obj, code);
    for (i = 0; i < times; i++) {
      fscript_exec(fscript, &v);
      value_reset(&v);
    }
    fscript_destroy(fscript);
  } else {
    fscript_eval(obj, code, &v);
    log_debug("result:%s\n", value_str_ex(&v, buff, sizeof(buff) - 1));
    value_reset(&v);
  }
  OBJECT_UNREF(obj);
  log_debug("cost: %d us\n", (int)(time_now_us() - start));

  return RET_OK;
}

static ret_t run_fscript_file(const char* filename, uint32_t times) {
  uint32_t size = 0;
  char* code = (char*)file_read(filename, &size);
  return_value_if_fail(code != NULL, RET_BAD_PARAMS);
  run_fscript(code, times);
  TKMEM_FREE(code);

  return RET_OK;
}

int main(int argc, char* argv[]) {
  platform_prepare();

  tk_mem_dump();
  if (argc < 2) {
    printf("Usage: %s script\n", argv[0]);
    return 0;
  } else {
    const char* code = argv[1];
    uint32_t times = argc > 2 ? tk_atoi(argv[2]) : 1;
    if (*code == '@') {
      run_fscript_file(code + 1, times);
    } else {
      run_fscript(code, times);
    }
  }
  tk_mem_dump();

  return 0;
}
