/**
 * File:   self_layouter_factory.h
 * Author: AWTK Develop Team
 * Brief:  self layouter factory
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

#ifndef TK_SELF_LAYOUTER_FACTORY_H
#define TK_SELF_LAYOUTER_FACTORY_H

#include "base/self_layouter.h"

BEGIN_C_DECLS

/**
 * @method self_layouter_create
 * @annotation ["global"]
 * 根据参数创建layouter对象。
 *
 * @return {self_layouter_t*} 返回layouter对象。
 */
self_layouter_t* self_layouter_create(const char* params);

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_FACTORY_H*/
