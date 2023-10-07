#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/socket_helper.h"
#include "streams/process/iostream_process.h"

int main(int argc, char* argv[]) {
  str_t file_path;
  ret_t ret = RET_OK;
  char exe[MAX_PATH + 1];
  const char* args[] = { "AAAAAA", "BBBBBB", "CCCCCC" };
  tk_iostream_t* process = NULL;

  path_exe(exe);
  TK_ENABLE_CONSOLE();

  tk_socket_init();
  platform_prepare();


  str_init(&file_path, 1024);
  str_append(&file_path, exe);
#ifdef WIN32
  str_trim_right(&file_path, "process_parent.exe");
  str_append(&file_path, "process_child.exe");
#else
  str_trim_right(&file_path, "process_parent");
  str_append(&file_path, "process_child");
#endif

  process = tk_iostream_process_create(file_path.str, args, ARRAY_SIZE(args));

  if (tk_iostream_process_start(process) == RET_OK) {
    char buff[1024 * 10];
    tk_istream_t* istream = tk_iostream_get_istream(process);

    memset(buff, 0x0, sizeof(buff));
    ret = tk_istream_wait_for_data(istream, 1000);
    if (ret == RET_OK) {
      const char* data = "Y";
      tk_istream_read(istream, buff, sizeof(buff));
      printf(buff);
      printf("Y\r\n");
      tk_iostream_write_len(process, data, tk_strlen(data), 1000);
      sleep_ms(500);
      memset(buff, 0x0, sizeof(buff));
      ret = tk_istream_wait_for_data(istream, 1000);
      if (ret == RET_OK) {
        tk_istream_read(istream, buff, sizeof(buff));
        printf(buff);
      }
    }

  }

  str_reset(&file_path);
  tk_object_unref(TK_OBJECT(process));
  tk_socket_deinit();


  return 0;
}
