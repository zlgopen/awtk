/**
 * File:   asset_loader.c
 * Author: AWTK Develop Team
 * Brief:  asset_loader
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/asset_loader.h"

asset_info_t* asset_loader_load(asset_loader_t* loader, uint16_t type, uint16_t subtype,
                                const char* path, const char* name) {
  return_value_if_fail(path != NULL && name != NULL, NULL);
  return_value_if_fail(loader != NULL && loader->vt != NULL && loader->vt->load != NULL, NULL);

  return loader->vt->load(loader, type, subtype, path, name);
}

bool_t asset_loader_exist(asset_loader_t* loader, const char* path) {
  return_value_if_fail(path != NULL, FALSE);
  return_value_if_fail(loader != NULL && loader->vt != NULL && loader->vt->exist != NULL, FALSE);

  return loader->vt->exist(loader, path);
}

ret_t asset_loader_destroy(asset_loader_t* loader) {
  return_value_if_fail(loader != NULL && loader->vt != NULL && loader->vt->destroy != NULL,
                       RET_BAD_PARAMS);

  return loader->vt->destroy(loader);
}
