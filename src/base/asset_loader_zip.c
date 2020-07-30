/**
 * File:   asset_loader_zip.c
 * Author: AWTK Develop Team
 * Brief:  asset_loader_zip
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

#include "tkc/mem.h"

#ifdef WITH_ASSET_LOADER_ZIP
#include "miniz/miniz.h"
#include "miniz/miniz_zip.h"
#include "base/asset_loader_zip.h"

typedef struct _asset_loader_zip_t {
  asset_loader_t asset_loader;
  void* data;
  uint32_t size;
  mz_zip_archive archive;

  data_reader_t* reader;
  bool_t own_the_reader;
} asset_loader_zip_t;

#ifdef WITH_SDL
#include <SDL.h>
static void* load_asset(const char* filename, uint32_t* data_size) {
  SDL_RWops* rwops = SDL_RWFromFile(filename, "r");
  if (rwops != NULL) {
    uint32_t size = rwops->size(rwops);
    void* data = TKMEM_ALLOC(size + 1);

    if (data != NULL) {
      memset(data, 0x00, size + 1);
      rwops->read(rwops, data, size, 1);
    }
    rwops->close(rwops);
    *data_size = size;

    return data;
  } else {
    return NULL;
  }
}
#else
static void* load_asset(const char* filename, uint32_t* data_size) {
  return file_read(filename, data_size);
}
#endif /*WITH_SDL*/

static void* miniz_awtk_alloc_func(void* opaque, size_t items, size_t size) {
  (void)opaque, (void)items, (void)size;
  return TKMEM_ALLOC(items * size);
}

static void miniz_awtk_free_func(void* opaque, void* address) {
  (void)opaque, (void)address;
  TKMEM_FREE(address);
}

static void* miniz_awtk_realloc_func(void* opaque, void* address, size_t items, size_t size) {
  (void)opaque, (void)address, (void)items, (void)size;
  return TKMEM_REALLOC(address, items * size);
}

static bool_t asset_loader_zip_exist(asset_loader_t* loader, const char* path) {
  asset_loader_zip_t* zip = (asset_loader_zip_t*)loader;
  int file_index = mz_zip_reader_locate_file(&(zip->archive), path, NULL, 0);

  return file_index >= 0;
}

static asset_info_t* asset_loader_zip_load(asset_loader_t* loader, uint16_t type, uint16_t subtype,
                                           const char* path, const char* name) {
  size_t size = 0;
  void* data = NULL;
  asset_info_t* info = NULL;
  asset_loader_zip_t* zip = (asset_loader_zip_t*)loader;
  int file_index = mz_zip_reader_locate_file(&(zip->archive), path, NULL, 0);

  if (file_index < 0) {
    return NULL;
  }

  data = mz_zip_reader_extract_to_heap(&(zip->archive), file_index, &size, 0);
  return_value_if_fail(data != NULL, NULL);

  info = asset_info_create(type, subtype, name, size);
  if (info != NULL) {
    memcpy(info->data, data, size);
  }
  TKMEM_FREE(data);

  return info;
}

static ret_t asset_loader_zip_destroy(asset_loader_t* loader) {
  asset_loader_zip_t* zip = (asset_loader_zip_t*)loader;
  mz_zip_end(&(zip->archive));

  if (zip->data != NULL) {
    TKMEM_FREE(zip->data);
  }

  if (zip->own_the_reader && zip->reader != NULL) {
    data_reader_destroy(zip->reader);
  }

  TKMEM_FREE(loader);

  return RET_OK;
}

static const asset_loader_vtable_t s_asset_loader_zip_vtable = {
    .load = asset_loader_zip_load,
    .exist = asset_loader_zip_exist,
    .destroy = asset_loader_zip_destroy};

static asset_loader_zip_t* asset_loader_zip_create_default(void) {
  asset_loader_zip_t* zip = TKMEM_ZALLOC(asset_loader_zip_t);
  return_value_if_fail(zip != NULL, NULL);

  zip->archive.m_pFree = miniz_awtk_free_func;
  zip->archive.m_pAlloc = miniz_awtk_alloc_func;
  zip->archive.m_pRealloc = miniz_awtk_realloc_func;
  zip->asset_loader.vt = &s_asset_loader_zip_vtable;

  return zip;
}

asset_loader_t* asset_loader_zip_create(const char* zipfile) {
  asset_loader_zip_t* zip = asset_loader_zip_create_default();
  return_value_if_fail(zip != NULL, NULL);

  zip->data = load_asset(zipfile, &(zip->size));
  if (zip->data == NULL) {
    TKMEM_FREE(zip);
  }

  if (!mz_zip_reader_init_mem(&(zip->archive), zip->data, zip->size, 0)) {
    TKMEM_FREE(zip);
  }

  return (asset_loader_t*)zip;
}

static size_t asset_loader_zip_read(void* pOpaque, mz_uint64 offset, void* buff, size_t n) {
  asset_loader_zip_t* zip = (asset_loader_zip_t*)pOpaque;

  return data_reader_read(zip->reader, offset, buff, n);
}

asset_loader_t* asset_loader_zip_create_with_reader(data_reader_t* reader, bool_t own_the_reader) {
  asset_loader_zip_t* zip = NULL;
  uint32_t size = data_reader_get_size(reader);
  return_value_if_fail(reader != NULL, NULL);

  zip = asset_loader_zip_create_default();
  return_value_if_fail(zip != NULL, NULL);

  zip->reader = reader;
  zip->own_the_reader = own_the_reader;
  zip->archive.m_pIO_opaque = zip;
  zip->archive.m_pRead = asset_loader_zip_read;

  if (!mz_zip_reader_init(&(zip->archive), size, 0)) {
    TKMEM_FREE(zip);
  }

  return (asset_loader_t*)zip;
}

#endif /*WITH_ASSET_LOADER_ZIP*/
