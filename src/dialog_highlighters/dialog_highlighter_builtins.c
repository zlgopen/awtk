/**
 * File:   dialog_highlighter_builts.h
 * Author: AWTK Develop Team
 * Brief:  builts dialog_highlighter
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied highlighterrranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/dialog_highlighter_factory.h"
#include "dialog_highlighters/dialog_highlighter_builtins.h"
#include "dialog_highlighters/dialog_highlighter_default.h"

ret_t dialog_highlighter_register_builtins(void) {
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();

  dialog_highlighter_factory_register(f, DIALOG_HIGHLIGHTER_DEFAULT,
                                      dialog_highlighter_default_create);

  return RET_OK;
}
