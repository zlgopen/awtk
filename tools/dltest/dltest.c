#include "tkc.h"

ret_t dltest(const char* filename, const char* func) {
  tk_dl_t* dl = tk_dl_open(filename);
  if(dl != NULL) {
    void* p = tk_dl_sym(dl, func);
    if(p != NULL) {
      log_debug("load %s %s ok\n", filename, func);
    } else {
      log_debug("get func failed:%s\n", tk_dl_error());
    } 
  } else{
    log_debug("load dll failed:%s\n", tk_dl_error());
  }

  return RET_OK;
}

int main(int argc, char* argv[]) {
  platform_prepare();
  if(argc != 3) {
    log_debug("Usage: %s dll func\n", argv[0]);
    return 0;
  }

  dltest(argv[1], argv[2]);

  return 0;
}
