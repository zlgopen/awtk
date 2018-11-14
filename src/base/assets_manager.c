/**
 * File:   assets_manager.h
 * Author: AWTK Develop Team
 * Brief:  asset manager
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/mem.h"
#include "base/path.h"
#include "base/utils.h"
#include "base/system_info.h"
#include "base/assets_manager.h"

static assets_manager_t* s_assets_manager = NULL;

#ifdef WITH_FS_RES
#include "base/fs.h"

static const char* assets_manager_get_res_root(assets_manager_t* rm) {
  if (rm->res_root != NULL) {
    return rm->res_root;
  } else {
    return system_info()->app_root;
  }
}

static asset_info_t* load_asset(uint16_t type, uint16_t subtype, uint32_t size, const char* path,
                                const char* name) {
  asset_info_t* info = TKMEM_ALLOC(sizeof(asset_info_t) + size);
  return_value_if_fail(info != NULL, NULL);

  memset(info, 0x00, sizeof(asset_info_t));
  info->size = size;
  info->type = type;
  info->subtype = subtype;
  info->refcount = 1;
  info->is_in_rom = FALSE;
  strncpy(info->name, name, NAME_LEN);

  ENSURE(file_read_part(path, info->data, size, 0) == size);

  return info;
}

static ret_t build_path(const char* res_root, char* path, uint32_t size, bool_t is_image,
                        const char* subpath, const char* name, const char* extname) {
  system_info_t* sysinfo = system_info();
  float_t dpr = sysinfo->device_pixel_ratio;

  if (is_image) {
    const char* ratio = "x1";
    if (dpr >= 3) {
      ratio = "x3";
    } else if (dpr >= 2) {
      ratio = "x2";
    }

    return_value_if_fail(path_build(path, size, res_root, subpath, ratio, name, NULL) == RET_OK,
                         RET_FAIL);
  } else {
    return_value_if_fail(path_build(path, size, res_root, subpath, name, NULL) == RET_OK, RET_FAIL);
  }

  return tk_str_append(path, size, extname);
}

asset_info_t* assets_manager_load(assets_manager_t* rm, asset_type_t type, const char* name) {
  int32_t size = 0;
  char path[MAX_PATH + 1];
  asset_info_t* info = NULL;
  const char* res_root = assets_manager_get_res_root(rm);

  memset(path, 0x00, sizeof(path));
  switch (type) {
    case ASSET_TYPE_FONT: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/fonts", name, ".ttf") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_FONT_TTF, size, path, name);
        break;
      }

      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/fonts", name, ".bin") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_FONT_BMP, size, path, name);
        break;
      }

      break;
    }
    case ASSET_TYPE_STYLE: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/styles", name, ".bin") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_STYLE, size, path, name);
        break;
      }
      break;
    }
    case ASSET_TYPE_STRINGS: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/strings", name, ".bin") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_STRINGS, size, path, name);
      }
      break;
    }
    case ASSET_TYPE_IMAGE: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, TRUE, "assets/raw/images", name, ".png") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_IMAGE_PNG, size, path, name);
        /*not cache png file raw data*/
        return info;
      }

      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, TRUE, "assets/raw/images", name, ".jpg") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_IMAGE_JPG, size, path, name);
        /*not cache png file jpg data*/
        return info;
      }

      break;
    }
    case ASSET_TYPE_UI: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/ui", name, ".bin") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_UI_BIN, size, path, name);
        /*not cache ui*/
        return info;
      }
      break;
    }
    case ASSET_TYPE_XML: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/xml", name, ".xml") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_XML, size, path, name);
        /*not cache xml*/
        return info;
      }
      break;
    }
    case ASSET_TYPE_DATA: {
      return_value_if_fail(
          build_path(res_root, path, MAX_PATH, FALSE, "assets/raw/data", name, ".bin") == RET_OK,
          NULL);
      if (file_exist(path)) {
        size = file_get_size(path);
        info = load_asset(type, ASSET_TYPE_DATA, size, path, name);
        break;
      }
      break;
    }
    default:
      break;
  }

  if (info != NULL) {
    assets_manager_add(rm, info);
  }

  return info;
}
#else
asset_info_t* assets_manager_load(assets_manager_t* rm, asset_type_t type, const char* name) {
  (void)type;
  (void)name;
  return NULL;
}
#endif /*WITH_FS_RES*/

assets_manager_t* assets_manager(void) {
  return s_assets_manager;
}

static ret_t asset_info_destroy(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    memset(info, 0x00, sizeof(asset_info_t));

    TKMEM_FREE(info);
  }

  return RET_OK;
}

static ret_t asset_info_unref(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    if (info->refcount > 0) {
      info->refcount--;
      if (info->refcount == 0) {
        asset_info_destroy(info);
      }
    }
  }

  return RET_OK;
}

static ret_t asset_info_ref(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    info->refcount++;
  }

  return RET_OK;
}

ret_t assets_manager_set(assets_manager_t* rm) {
  s_assets_manager = rm;

  return RET_OK;
}

assets_manager_t* assets_manager_create(uint32_t init_nr) {
  assets_manager_t* rm = TKMEM_ZALLOC(assets_manager_t);

  return assets_manager_init(rm, init_nr);
}

assets_manager_t* assets_manager_init(assets_manager_t* rm, uint32_t init_nr) {
  return_value_if_fail(rm != NULL, NULL);

  array_init(&(rm->assets), init_nr);

  return rm;
}

ret_t assets_manager_set_res_root(assets_manager_t* rm, const char* res_root) {
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(rm->res_root);
  if (res_root != NULL) {
    rm->res_root = tk_strdup(res_root);
  }

  return RET_OK;
}

ret_t assets_manager_add(assets_manager_t* rm, const void* info) {
  const asset_info_t* r = (const asset_info_t*)info;
  return_value_if_fail(rm != NULL && info != NULL, RET_BAD_PARAMS);

  asset_info_ref((asset_info_t*)r);

  return array_push(&(rm->assets), (void*)r);
}

const asset_info_t* assets_manager_find_in_cache(assets_manager_t* rm, asset_type_t type,
                                                 const char* name) {
  uint32_t i = 0;
  const asset_info_t* iter = NULL;
  const asset_info_t** all = NULL;
  return_value_if_fail(rm != NULL && name != NULL, NULL);

  all = (const asset_info_t**)(rm->assets.elms);

  for (i = 0; i < rm->assets.size; i++) {
    iter = all[i];
    if (type == iter->type && strcmp(name, iter->name) == 0) {
      return iter;
    }
  }

  return NULL;
}

const asset_info_t* assets_manager_ref(assets_manager_t* rm, asset_type_t type, const char* name) {
  const asset_info_t* info = assets_manager_find_in_cache(rm, type, name);

  if (info == NULL) {
    info = assets_manager_load(rm, type, name);
    /*加载时初始计数为1，缓存时自动增加引用计数，此处不需要引用*/
  } else {
    asset_info_ref((asset_info_t*)info);
  }

  return info;
}

ret_t assets_manager_unref(assets_manager_t* rm, const asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (rm == NULL) {
    /*asset manager was destroied*/
    return RET_OK;
  }

  if (!(info->is_in_rom)) {
    bool_t remove = info->refcount <= 1;

    asset_info_unref((asset_info_t*)info);
    if (remove) {
      array_remove(&(rm->assets), NULL, (void*)info, NULL);
    }
  }

  return RET_OK;
}

static int asset_cache_cmp_type(const void* a, const void* b) {
  const asset_info_t* aa = (const asset_info_t*)a;
  const asset_info_t* bb = (const asset_info_t*)b;

  if (aa->is_in_rom) {
    return -1;
  }

  return aa->type - bb->type;
}

ret_t assets_manager_clear_cache(assets_manager_t* rm, asset_type_t type) {
  asset_info_t info;

  memset(&info, 0x00, sizeof(info));
  info.type = type;
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  return array_remove_all(&(rm->assets), asset_cache_cmp_type, &info,
                          (tk_destroy_t)asset_info_unref);
}

ret_t assets_manager_deinit(assets_manager_t* rm) {
  uint32_t i = 0;
  asset_info_t* iter = NULL;
  asset_info_t** all = NULL;
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  all = (asset_info_t**)(rm->assets.elms);

  for (i = 0; i < rm->assets.size; i++) {
    iter = all[i];
    asset_info_destroy(iter);
  }

  TKMEM_FREE(rm->res_root);
  array_deinit(&(rm->assets));

  return RET_OK;
}

ret_t assets_manager_destroy(assets_manager_t* rm) {
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);
  assets_manager_deinit(rm);

  if (rm == assets_manager()) {
    assets_manager_set(NULL);
  }
  TKMEM_FREE(rm);

  return RET_OK;
}
