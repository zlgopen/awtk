/**
 * File:   children_layouter_factory.h
 * Author: AWTK Develop Team
 * Brief:  children layouter factory
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHILDREN_LAYOUTER_FACTORY_H
#define TK_CHILDREN_LAYOUTER_FACTORY_H

#include "base/children_layouter.h"

BEGIN_C_DECLS

children_layouter_t* children_layouter_create(const char* params);

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_FACTORY_H*/
