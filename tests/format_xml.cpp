#include "conf_io/conf_xml.h"
#include "tkc/fs.h"
#include "tkc/utils.h"
#include "tkc/platform.h"

#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"
#include "base/data_reader_asset.h"

int main(int argc, char* argv[]) {
  char in_filename[MAX_PATH + 1] = {0};
  char out_filename[MAX_PATH + 1] = {0};

  platform_prepare();
  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "asset", data_reader_asset_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);

  if (argc >= 3) {
    tk_object_t* obj = NULL;
    tk_snprintf(in_filename, sizeof(in_filename) - 1, "file://%s", argv[1]);
    tk_snprintf(out_filename, sizeof(out_filename) - 1, "file://%s", argv[2]);
    obj = conf_xml_load(in_filename, FALSE);
    if (obj != NULL) {
      conf_xml_save_as(obj, out_filename);
      TK_OBJECT_UNREF(obj);
    }
  }
}
