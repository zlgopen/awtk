/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  image_loader interface
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
 * 2018-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/image_loader.h"

ret_t image_loader_load(image_loader_t* loader, const asset_info_t* asset, bitmap_t* bitmap) {
  return_value_if_fail(loader != NULL && loader->load != NULL && asset != NULL && bitmap != NULL,
                       RET_BAD_PARAMS);

  return loader->load(loader, asset, bitmap);
}

#define MAX_IMAGE_LOADERS 3
static image_loader_t* s_image_loaders[MAX_IMAGE_LOADERS];

ret_t image_loader_register(image_loader_t* loader) {
  uint32_t i = 0;
  return_value_if_fail(loader != NULL, RET_BAD_PARAMS);
  for (i = 0; i < ARRAY_SIZE(s_image_loaders); i++) {
    if (s_image_loaders[i] == NULL) {
      s_image_loaders[i] = loader;

      return RET_OK;
    }
  }

  return RET_FAIL;
}

ret_t image_loader_unregister(image_loader_t* loader) {
  uint32_t i = 0;
  return_value_if_fail(loader != NULL, RET_BAD_PARAMS);
  for (i = 0; i < ARRAY_SIZE(s_image_loaders); i++) {
    if (s_image_loaders[i] == loader) {
      s_image_loaders[i] = NULL;

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

ret_t image_loader_load_image(const asset_info_t* asset, bitmap_t* bitmap) {
  uint32_t i = 0;
  return_value_if_fail(asset != NULL && bitmap != NULL, RET_BAD_PARAMS);

  for (i = 0; i < ARRAY_SIZE(s_image_loaders); i++) {
    image_loader_t* loader = s_image_loaders[i];

    if (loader != NULL && image_loader_load(loader, asset, bitmap) == RET_OK) {
      return RET_OK;
    }
  }

  return RET_FAIL;
}
