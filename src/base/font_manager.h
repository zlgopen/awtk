/**
 * File:   font_manager.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  font manager
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_FONT_MANAGER_H
#define LFTK_FONT_MANAGER_H

#include "base/font.h"
#include "base/array.h"

BEGIN_C_DECLS

typedef struct _font_manager_t {
  array_t fonts;
}font_manager_t;

font_manager_t* default_fm(void);

font_manager_t* font_manager_create(void);
font_manager_t* font_manager_init(font_manager_t* fm);

ret_t font_manager_add(font_manager_t* fm, font_t* font);

font_t* font_manager_find(font_manager_t* fm, const char* name, uint16_t size);

ret_t font_manager_deinit(font_manager_t* fm);
ret_t font_manager_destroy(font_manager_t* fm);

END_C_DECLS

#endif/*LFTK_FONT_MANAGER_H*/

