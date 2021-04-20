#include "awtk.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"
#include "base/data_reader_asset.h"
#include "conf_io/app_conf_init_json.h"

#define NR 10000

static void* work_thread(void* args) {
  uint32_t i = 0;
  uint32_t id = (uint32_t)tk_pointer_to_int(args);

  log_debug("%u start\n", id);
  for (i = 0; i < NR; i++) {
    app_conf_set_int("test.int", i);
    app_conf_set_double("test.int", i);
    app_conf_set_str("test.str", "foo");
    log_debug("%u run %u\n", id, i);
  }

  return NULL;
}

int main(int argc, char* argv[]) {
  uint32_t i = 0;
  tk_thread_t* threads[100];

  platform_prepare();
  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "asset", data_reader_asset_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);

  app_conf_init_json("app_conf_multi_thread");
  app_conf_set_int("test.int", 100);
  app_conf_save();

  for (i = 0; i < ARRAY_SIZE(threads); i++) {
    threads[i] = tk_thread_create(work_thread, tk_pointer_from_int(i));
    tk_thread_start(threads[i]);
  }

  for (i = 0; i < ARRAY_SIZE(threads); i++) {
    tk_thread_join(threads[i]);
    tk_thread_destroy(threads[i]);
    log_debug("%u stop\n", i);
  }

  return 0;
}
