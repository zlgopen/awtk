/**
 * File:   self_layouter_builts.h
 * Author: AWTK Develop Team
 * Brief:  builts self_layouter
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

#include "base/self_layouter_factory.h"
#include "layouters/self_layouter_builtins.h"
#include "layouters/self_layouter_menu.h"
#include "layouters/self_layouter_default.h"

ret_t self_layouter_register_builtins(void) {
  self_layouter_factory_t* f = self_layouter_factory();

  self_layouter_factory_register(f, SELF_LAYOUTER_DEFAULT, self_layouter_default_create);
  self_layouter_factory_register(f, SELF_LAYOUTER_MENU, self_layouter_menu_create);

  return RET_OK;
}
