#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "fscript_ext/fscript_ext.h"
#include "conf_io/app_conf_init_json.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"
#include "base/data_reader_asset.h"

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

  fscript_global_init();
  fscript_ext_init();
  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "asset", data_reader_asset_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);

  app_conf_init_json("runFScript");
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
  app_conf_set_int("foobar", 1);
  app_conf_save();
  data_writer_factory_destroy(data_writer_factory());
  data_reader_factory_destroy(data_reader_factory());
  data_writer_factory_set(NULL);
  data_reader_factory_set(NULL);
  fscript_global_deinit();

  return 0;
}
