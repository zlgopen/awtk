/**
 * File:   data_reader.c
 * Author: AWTK Develop Team
 * Brief:  data_reader asset
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License asset for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-06-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/asset_info.h"
#include "base/assets_manager.h"
#include "base/data_reader_asset.h"

typedef struct _data_reader_asset_t {
  data_reader_t data_reader;

  const asset_info_t* info;
} data_reader_asset_t;

static int32_t data_reader_asset_read(data_reader_t* reader, uint64_t offset, void* data,
                                      uint32_t size) {
  const void* src = NULL;
  uint32_t max_size = 0;
  data_reader_asset_t* asset = (data_reader_asset_t*)reader;
  return_value_if_fail(asset != NULL && asset->info != NULL, 0);
  return_value_if_fail(offset < asset->info->size, 0);

  src = asset->info->data + offset;
  max_size = asset->info->size - offset;
  size = tk_min(size, max_size);

  memcpy(data, src, size);

  return size;
}

static uint64_t data_reader_asset_get_size(data_reader_t* reader) {
  data_reader_asset_t* asset = (data_reader_asset_t*)reader;
  return_value_if_fail(asset != NULL && asset->info != NULL, 0);

  return asset->info->size;
}

static ret_t data_reader_asset_destroy(data_reader_t* reader) {
  data_reader_asset_t* asset = (data_reader_asset_t*)reader;

  assets_manager_unref(assets_manager(), (asset_info_t*)(asset->info));
  TKMEM_FREE(asset);

  return RET_OK;
}

static data_reader_vtable_t s_data_reader_asset_vtable = {
    .read = data_reader_asset_read,
    .get_size = data_reader_asset_get_size,
    .destroy = data_reader_asset_destroy,
};

data_reader_t* data_reader_asset_create(const char* assetname) {
  data_reader_asset_t* asset = NULL;
  return_value_if_fail(assetname != NULL, NULL);
  asset = TKMEM_ZALLOC(data_reader_asset_t);
  return_value_if_fail(asset != NULL, NULL);

  asset->data_reader.vt = &s_data_reader_asset_vtable;
  asset->info = assets_manager_ref(assets_manager(), ASSET_TYPE_DATA, assetname);

  if (asset->info == NULL) {
    TKMEM_FREE(asset);
  }

  return (data_reader_t*)asset;
}
