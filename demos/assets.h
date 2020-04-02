/**
 * File:   asset.h
 * Author: AWTK Develop Team
 * Brief:  asset init
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef ASSET_H
#define ASSET_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

ret_t assets_init(void);

bool_t assets_has_theme(const char* name);
ret_t assets_set_global_theme(const char* name);

END_C_DECLS

#endif /*ASSET_H*/
