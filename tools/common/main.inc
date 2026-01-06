#include "tkc.h"
#include "charset/encoding.h"

#ifdef WIN32
#include <windows.h>
#endif

wchar_t** argvw_create(int argc, char* argv[]) {
  int i = 0;
  wstr_t str;
  wchar_t** argvw = (wchar_t**)TKMEM_ALLOC((argc + 1) * sizeof(wchar_t*));
  return_value_if_fail(argvw != NULL, NULL);
  wstr_init(&str, 100);

  for (i = 0; i < argc; i++) {
#ifdef WIN32
    char utf8[MAX_PATH] = {0};
    /* windows 系统下，gdb调试时参数是utf-8编码（可能gdb内部做了处理），而正常运行时是gbk编码，需要先转码为utf-8 */
    if(IsDebuggerPresent()) {
      wstr_set_utf8(&str, argv[i]);
    } else {
      encoding_gbk_to_utf8(argv[i], MAX_PATH, utf8, MAX_PATH);
      wstr_set_utf8(&str, utf8);
    }
#else
    /* linux/mac 系统下，参数一般是utf-8编码，直接使用 */
    wstr_set_utf8(&str, argv[i]);
#endif
    argvw[i] = wcs_dup(str.str);
  }
  argvw[i] = NULL;

  wstr_reset(&str);
  return argvw;
}

ret_t argvw_destroy(wchar_t** argvw) {
  uint32_t i = 0;
  return_value_if_fail(argvw != NULL, RET_BAD_PARAMS);

  for (i = 0; argvw[i] != NULL; i++) {
    TKMEM_FREE(argvw[i]);
  }
  TKMEM_FREE(argvw);

  return RET_OK;
}

#if !defined(WIN32) || defined(MINGW)
int main(int argc, char* argv[]) {
  int ret = 0;
  wchar_t** argvw = argvw_create(argc, argv);

  ret = wmain(argc, argvw);
  argvw_destroy(argvw);

  return ret;
}
#endif /*WIN32*/
