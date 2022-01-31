/**
 * File:   asset_loader_default.c
 * Author: AWTK Develop Team
 * Brief:  asset_loader_default
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/fs.h"
#include "base/asset_loader_default.h"

#define MMAP_ASSET_MIN_SIZE 4 * 1024

static asset_info_t* load_asset_with_mmap(uint16_t type, uint16_t subtype, const char* path,
                                          const char* name) {
#ifdef LOAD_ASSET_WITH_MMAP
  if (file_get_size(path) >= MMAP_ASSET_MIN_SIZE) {
    asset_info_t* info = asset_info_create(type, subtype, name, 0);
    return_value_if_fail(info != NULL, NULL);

    info->map = mmap_create(path, FALSE, FALSE);
    if (info->map != NULL) {
      info->data = info->map->data;
      info->size = info->map->size;
      return info;
    } else {
      asset_info_destroy(info);
    }
  }
#endif /*LOAD_ASSET_WITH_MMAP*/
  return NULL;
}

#ifdef WITH_SDL
#include <SDL.h>
static asset_info_t* load_asset(uint16_t type, uint16_t subtype, const char* path,
                                const char* name) {
  asset_info_t* info = load_asset_with_mmap(type, subtype, path, name);
  if (info == NULL) {
    SDL_RWops* rwops = SDL_RWFromFile(path, "r");
    if (rwops != NULL) {
      int32_t size = rwops->size(rwops);
      info = asset_info_create(type, subtype, name, size);
      if (info != NULL) {
        rwops->read(rwops, info->data, size, 1);
      }
      rwops->close(rwops);
    }
  }

  return info;
}

static bool_t asset_exist(const char* path) {
  SDL_RWops* rwops = SDL_RWFromFile(path, "r");

  if (rwops != NULL) {
    rwops->close(rwops);
    return TRUE;
  } else {
    return FALSE;
  }
}

#else
static bool_t asset_exist(const char* path) {
  return file_exist(path);
}

static asset_info_t* load_asset(uint16_t type, uint16_t subtype, const char* path,
                                const char* name) {
  asset_info_t* info = NULL;
  if (file_exist(path)) {
    info = load_asset_with_mmap(type, subtype, path, name);
    if (info == NULL) {
      int32_t size = file_get_size(path);
      info = asset_info_create(type, subtype, name, size);
      return_value_if_fail(info != NULL, NULL);

      ENSURE(file_read_part(path, info->data, size, 0) == size);
    }
  }

  return info;
}
#endif /*WITH_SDL*/

static asset_info_t* asset_loader_default_load(asset_loader_t* loader, uint16_t type,
                                               uint16_t subtype, const char* path,
                                               const char* name) {
  return load_asset(type, subtype, path, name);
}

static bool_t asset_loader_default_exist(asset_loader_t* loader, const char* path) {
  return asset_exist(path);
}

static ret_t asset_loader_default_destroy(asset_loader_t* loader) {
  TKMEM_FREE(loader);

  return RET_OK;
}

static const asset_loader_vtable_t s_asset_loader_default_vtable = {
    .load = asset_loader_default_load,
    .exist = asset_loader_default_exist,
    .destroy = asset_loader_default_destroy};

asset_loader_t* asset_loader_default_create(void) {
  asset_loader_t* loader = TKMEM_ZALLOC(asset_loader_t);
  return_value_if_fail(loader != NULL, NULL);

  loader->vt = &s_asset_loader_default_vtable;

  return loader;
}

asset_loader_t* asset_loader_create(void) {
  return asset_loader_default_create();
}
