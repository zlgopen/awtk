/**
 * File:   font_loader.h
 * Author: AWTK Develop Team
 * Brief:  font_loader interface
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
 * 2018-10-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/font_loader.h"

font_t* font_loader_load(font_loader_t* loader, const char* name, const uint8_t* buff,
                         uint32_t size) {
  return_value_if_fail(loader != NULL && loader->load != NULL && name != NULL && buff != NULL,
                       NULL);

  return loader->load(loader, name, buff, size);
}
