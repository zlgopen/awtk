/**
 * File:   resource_manager.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  resource manager
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_RESOURCE_MANAGER_H
#define LFTK_RESOURCE_MANAGER_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef enum _resource_type_t {
  RESOURCE_TYPE_NONE,
  RESOURCE_TYPE_FONT_TTF,
  RESOURCE_TYPE_FONT_BMP,
  RESOURCE_TYPE_IMAGE_BMP,
  RESOURCE_TYPE_IMAGE_PNG,
  RESOURCE_TYPE_IMAGE_JPG,
  RESOURCE_TYPE_THEME,
  RESOURCE_TYPE_UI,
  RESOURCE_TYPE_XML
}resource_type_t;

typedef struct _resource_info_t {
  uint32_t type;
  uint32_t size;
  char name[NAME_LEN+1];
  uint8_t data[4];
} resource_info_t;

ret_t resource_manager_init(uint32_t init_res_nr);
ret_t resource_manager_add(const resource_info_t* info);

const resource_info_t* resource_manager_ref(resource_type_t type, const char* name);
ret_t resource_manager_unref(const resource_info_t* info);


END_C_DECLS

#endif/*LFTK_RESOURCE_MANAGER_H*/

