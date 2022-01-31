/**
 * File:   dl.c
 * Author: AWTK Develop Team
 * Brief:  dynamic libaray api
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

struct _tk_dl_t {
  void* h;
};

tk_dl_t* tk_dl_open(const char* filename) {
  return NULL;
}

void* tk_dl_sym(tk_dl_t* dl, const char* name) {
  return NULL;
}

ret_t tk_dl_close(tk_dl_t* dl) {
  return RET_NOT_IMPL;
}
