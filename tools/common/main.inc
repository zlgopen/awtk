#include "tkc.h"

wchar_t** argvw_create(int argc, char* argv[]) {
  int i = 0;
  wstr_t str;
  wchar_t** argvw = (wchar_t**)TKMEM_ALLOC((argc + 1) * sizeof(wchar_t*));
  return_value_if_fail(argvw != NULL, NULL);
  wstr_init(&str, 100);

  for (i = 0; i < argc; i++) {
    wstr_set_utf8(&str, argv[i]);
    argvw[i] = wcs_dup(str.str);
  }
  argvw[i] = NULL;

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
