/**
 * File:   bitmap.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap interface
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

#include "base/bitmap.h"

bool_t image_loader_match(image_loader_t* loader, uint16_t type) {
  return_value_if_fail(loader != NULL && loader->match != NULL, FALSE); 

  return loader->match(loader, type);
}

ret_t image_loader_load(image_loader_t* loader, const uint8_t* buff, uint32_t size, bitmap_t* bitmap) {
  return_value_if_fail(loader != NULL && loader->load != NULL && buff != NULL && bitmap != NULL,
                       RET_BAD_PARAMS);

  return loader->load(loader, buff, size, bitmap);
}

ret_t bitmap_destroy(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL && bitmap->destroy != NULL, RET_BAD_PARAMS);

  return bitmap->destroy(bitmap);
}
