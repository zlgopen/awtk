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
  tk_iostream_t* process = NULL;

  path_exe(exe);
  TK_ENABLE_CONSOLE();

  tk_socket_init();
  platform_prepare();

  str_init(&work_dir, 1024);
  str_init(&file_path, 1024);
  str_append(&work_dir, exe);
#ifdef WIN32
  str_trim_right(&work_dir, "process_demoui.exe");
  str_append(&file_path, work_dir.str);
  str_append(&file_path, "demoui.exe");
#else
  str_trim_right(&work_dir, "process_demoui");
  str_append(&file_path, work_dir.str);
  str_append(&file_path, "demoui");
#endif

  process = tk_iostream_process_create(file_path.str, NULL, 0);
  tk_iostream_process_set_work_dir(process, work_dir.str);
  tk_iostream_process_set_show_window(process, TRUE);
  if (tk_iostream_process_start(process) == RET_OK) {
    char buff[1024 * 10];
    tk_istream_t* istream = tk_iostream_get_istream(process);

    memset(buff, 0x0, sizeof(buff));
    ret = tk_istream_wait_for_data(istream, 1000);
    if (ret == RET_OK) {
      tk_istream_read(istream, buff, sizeof(buff));
      printf("%s", buff);
    }
    printf("key down kill process \r\n");
    getchar();
    tk_iostream_process_kill(process);
  }

  str_reset(&work_dir);
  str_reset(&file_path);
  tk_object_unref(TK_OBJECT(process));
  tk_socket_deinit();

  return 0;
}
