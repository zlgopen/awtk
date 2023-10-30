#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/socket_helper.h"
#include "streams/process/iostream_process.h"

int main(int argc, char* argv[]) {
  str_t work_dir;
  str_t file_path;
  ret_t ret = RET_OK;
  char exe[MAX_PATH + 1];
  const char* args[] = {"AAAAAA", "BBBBBB", "CCCCCC"};
  tk_iostream_t* process = NULL;

  path_exe(exe);
  TK_ENABLE_CONSOLE();

  tk_socket_init();
  platform_prepare();

  str_init(&work_dir, 1024);
  str_init(&file_path, 1024);
  str_append(&work_dir, exe);
#ifdef WIN32
  str_trim_right(&work_dir, "process_parent.exe");
  str_append(&file_path, work_dir.str);
  str_append(&file_path, "process_child.exe");
#else
  str_trim_right(&work_dir, "process_parent");
  str_append(&file_path, work_dir.str);
  str_append(&file_path, "process_child");
#endif
  str_append(&work_dir, "../src");

  process = tk_iostream_process_create(file_path.str, args, ARRAY_SIZE(args));
  tk_iostream_process_set_work_dir(process, work_dir.str);
  if (tk_iostream_process_start(process) == RET_OK) {
    char buff[1024 * 10];
    tk_istream_t* istream = tk_iostream_get_istream(process);

    memset(buff, 0x0, sizeof(buff));
    ret = tk_istream_wait_for_data(istream, 1000);
    if (ret == RET_OK) {
      const char* data = "Y";
      tk_istream_read(istream, buff, sizeof(buff));
      printf("%s", buff);
      printf("Y\r\n");
      tk_iostream_write_len(process, data, tk_strlen(data), 1000);
      sleep_ms(500);
      memset(buff, 0x0, sizeof(buff));
      ret = tk_istream_wait_for_data(istream, 1000);
      if (ret == RET_OK) {
        tk_istream_read(istream, buff, sizeof(buff));
        printf("%s", buff);
      }
      tk_iostream_process_kill(process);
      if (tk_istream_wait_for_data(istream, 0xFFFFFFFF) == RET_OK) {
        memset(buff, 0x0, sizeof(buff));
        tk_istream_read(istream, buff, sizeof(buff));
      }
      if (tk_iostream_write_len(process, data, tk_strlen(data), 1000) == 0) {
        if (tk_istream_wait_for_data(istream, 0xFFFFFFFF) != RET_OK) {
          printf("close process !!!");
        }
      }
    }
  }

  str_reset(&work_dir);
  str_reset(&file_path);
  tk_object_unref(TK_OBJECT(process));
  tk_socket_deinit();

  return 0;
}
