#include "tkc/platform.h"
#include "conf_io/conf_json.h"
#include "conf_io/conf_ubjson.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_factory.h"

static ret_t json_to_ubjson(const char* json_filename, const char* ubjson_filename) {
  tk_object_t* obj = conf_json_load(json_filename, FALSE);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  conf_ubjson_save_as(obj, ubjson_filename);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t application_init(void) {
  data_reader_factory_t* reader_factory = data_reader_factory_create();
  data_writer_factory_t* writer_factory = data_writer_factory_create();

  data_reader_factory_set(reader_factory);
  data_reader_factory_register(reader_factory, "file", data_reader_file_create);

  data_writer_factory_set(writer_factory);
  data_writer_factory_register(writer_factory, "file", data_writer_file_create);

  return RET_OK;
}

static ret_t application_deinit(void) {
  data_reader_factory_destroy(data_reader_factory());
  data_reader_factory_set(NULL);
  data_writer_factory_destroy(data_writer_factory());
  data_writer_factory_set(NULL);

  return RET_OK;
}

int main(int argc, char* argv[]) {
  platform_prepare();

  if (argc != 3) {
    log_info("Usage: %s ubjson_filename json_filename\n", argv[0]);
    return -1;
  }
  application_init();
  json_to_ubjson(argv[1], argv[2]);
  application_deinit();

  return 0;
}
