#include "common/utils.h"

#if !defined(WIN32) || defined(MINGW)
int main(int argc, char* argv[]) {
  int ret = 0;
  wchar_t** argvw = argvw_create(argc, argv);

  ret = wmain(argc, argvw);
  argvw_destroy(argvw);

  return ret;
}
#endif /*WIN32*/
