/**
 * File:   dl.c
 * Author: AWTK Develop Team
 * Brief:  dynamic libaray api
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/dl.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

#if defined(ANDROID) || defined(IOS) || defined(MACOS) || defined(LINUX)
#include <dlfcn.h>
struct _tk_dl_t {
  void* h;
};

tk_dl_t* tk_dl_open(const char* filename) {
  void* h = NULL;
  tk_dl_t* dl = NULL;
  return_value_if_fail(filename != NULL, NULL);

  h = dlopen(filename, RTLD_NOW | RTLD_LOCAL);
  return_value_if_fail(h != NULL, NULL);

  dl = TKMEM_ZALLOC(tk_dl_t);
  if (dl == NULL) {
    dlclose(h);
  } else {
    dl->h = h;
  }

  return dl;
}

void* tk_dl_sym(tk_dl_t* dl, const char* name) {
  return_value_if_fail(dl != NULL && name != NULL, NULL);

  return dlsym(dl->h, name);
}

ret_t tk_dl_close(tk_dl_t* dl) {
  return_value_if_fail(dl != NULL && dl->h != NULL, RET_BAD_PARAMS);
  dlclose(dl->h);
  dl->h = NULL;
  TKMEM_FREE(dl);

  return RET_OK;
}

#elif defined(WIN32)
struct _tk_dl_t {
  HMODULE h;
};

tk_dl_t* tk_dl_open(const char* filename) {
  HMODULE h = NULL;
  tk_dl_t* dl = NULL;
  return_value_if_fail(filename != NULL, NULL);
  wchar_t* w_file_name = tk_wstr_dup_utf8(filename);
  h = LoadLibraryW(w_file_name);
  TKMEM_FREE(w_file_name);
  return_value_if_fail(h != NULL, NULL);

  dl = TKMEM_ZALLOC(tk_dl_t);
  if (dl == NULL) {
    FreeLibrary(h);
  } else {
    dl->h = h;
  }

  return dl;
}

void* tk_dl_sym(tk_dl_t* dl, const char* name) {
  return_value_if_fail(dl != NULL && name != NULL, NULL);

  return GetProcAddress(dl->h, name);
}

ret_t tk_dl_close(tk_dl_t* dl) {
  return_value_if_fail(dl != NULL && dl->h != NULL, RET_BAD_PARAMS);
  FreeLibrary(dl->h);
  dl->h = NULL;
  TKMEM_FREE(dl);

  return RET_OK;
}
#else
#error "not support platform"
#endif
