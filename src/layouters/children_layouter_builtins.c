/**
 * File:   children_layouter_builts.h
 * Author: AWTK Develop Team
 * Brief:  builts children_layouter
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
 * 2019-05-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/children_layouter_factory.h"
#include "layouters/children_layouter_builtins.h"
#include "layouters/children_layouter_default.h"

ret_t children_layouter_register_builtins(void) {
  children_layouter_factory_t* f = children_layouter_factory();

  children_layouter_factory_register(f, CHILDREN_LAYOUTER_DEFAULT,
                                     children_layouter_default_create);

  return RET_OK;
}
