#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/url.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/func_call_parser.h"
#include "network_shell.h"

static ret_t show_help(int argc, char* argv[]) {
  log_debug("Usage: %s protocl args\n", argv[0]);
  log_debug("Format: %s interface_name interface_type\n", argv[0]);
  log_debug("ex: %s eth0 1\n", argv[0]);

  return RET_OK;
}

static network_interface_t* network_interface_create_with_args(int argc, char* argv[]) {
  int interface_type = 0;
  network_interface_t* network_interface = NULL;

  if (argc < 3) {
    show_help(argc, argv);
    exit(0);
    return NULL;
  }

  interface_type = tk_atoi(argv[2]);
  network_interface = network_interface_create(argv[1], (network_interface_type_t)(interface_type));
  return_value_if_fail(network_interface != NULL && network_interface->vt != NULL, NULL);
  return network_interface;
}

#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"

static ret_t network_shell_init(void) {
  TK_ENABLE_CONSOLE();
  platform_prepare();
  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);

  return RET_OK;
}

int main(int argc, char* argv[]) {
  network_interface_t* network_interface = NULL;
  network_shell_init();
  network_interface = network_interface_create_with_args(argc, argv);

  return_value_if_fail(network_interface != NULL, 0);
  network_shell_run(network_interface);

  network_interface_destroy(network_interface);

  return 0;
}
