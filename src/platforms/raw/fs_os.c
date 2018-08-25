#include "base/fs.h"

fs_t* os_fs(void) {
  return (fs_t*)&s_os_fs;
}
