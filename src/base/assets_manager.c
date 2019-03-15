/**
 * File:   assets_manager.h
 * Author: AWTK Develop Team
 * Brief:  asset manager
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "base/locale_info.h"
#include "base/system_info.h"
#include "base/assets_manager.h"

static ret_t asset_info_unref(asset_info_t* info);

static int asset_cache_cmp_type(const void* a, const void* b) {
  const asset_info_t* aa = (const asset_info_t*)a;
  const asset_info_t* bb = (const asset_info_t*)b;

  if (aa->is_in_rom) {
    return -1;
  }

  return aa->type - bb->type;
}

static assets_manager_t* s_assets_manager = NULL;

#ifdef WITH_FS_RES
#include "tkc/fs.h"

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
  strncpy(info->name, name, TK_NAME_LEN);

  ENSURE(file_read_part(path, info->data, size, 0) == size);

  return info;
}

static ret_t build_path(const char* res_root, char* path, uint32_t size, bool_t ratio_sensitive,
                        const char* subpath, const char* name, const char* extname) {
  system_info_t* sysinfo = system_info();
  float_t dpr = sysinfo->device_pixel_ratio;

  if (ratio_sensitive) {
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

static asset_info_t* try_load_image(const char* res_root, const char* name,
                                    asset_image_type_t subtype, bool_t ratio) {
  char path[MAX_PATH + 1];
  const char* extname = NULL;
  const char* subpath = ratio ? "assets/raw/images" : "assets/raw/images/xx";

  switch (subtype) {
    case ASSET_TYPE_IMAGE_JPG: {
      extname = ".jpg";
      break;
    }
    case ASSET_TYPE_IMAGE_PNG: {
      extname = ".png";
      break;
    }
    case ASSET_TYPE_IMAGE_GIF: {
      extname = ".gif";
      break;
    }
    case ASSET_TYPE_IMAGE_BSVG: {
      extname = ".bsvg";
      subpath = "assets/raw/images/svg";
      break;
    }
    default: { return NULL; }
  }

  return_value_if_fail(
      build_path(res_root, path, MAX_PATH, ratio, subpath, name, extname) == RET_OK, NULL);

  if (file_exist(path)) {
    size_t size = file_get_size(path);
    asset_info_t* info = load_asset(ASSET_TYPE_IMAGE, subtype, size, path, name);

    return info;
  }

  return NULL;
}

static asset_info_t* try_load_assets(const char* res_root, const char* name, const char* extname,
                                     asset_type_t type, uint16_t subtype) {
  char path[MAX_PATH + 1];
  const char* subpath = NULL;
  switch (type) {
    case ASSET_TYPE_FONT: {
      subpath = "assets/raw/fonts";
      break;
    }
    case ASSET_TYPE_SCRIPT: {
      subpath = "assets/raw/scripts";
      break;
    }
    case ASSET_TYPE_STYLE: {
      subpath = "assets/raw/styles";
      break;
    }
    case ASSET_TYPE_STRINGS: {
      subpath = "assets/raw/strings";
      break;
    }
    case ASSET_TYPE_UI: {
      subpath = "assets/raw/ui";
      break;
    }
    case ASSET_TYPE_XML: {
      subpath = "assets/raw/xml";
      break;
    }
    case ASSET_TYPE_DATA: {
      subpath = "assets/raw/data";
      break;
    }
    default: { return NULL; }
  }

  return_value_if_fail(
      build_path(res_root, path, MAX_PATH, FALSE, subpath, name, extname) == RET_OK, NULL);
  if (file_exist(path)) {
    int32_t size = file_get_size(path);
    return load_asset(type, subtype, size, path, name);
  }

  return NULL;
}

asset_info_t* assets_manager_load(assets_manager_t* rm, asset_type_t type, const char* name) {
  asset_info_t* info = NULL;
  const char* res_root = assets_manager_get_res_root(rm);

  switch (type) {
    case ASSET_TYPE_FONT: {
      if ((info = try_load_assets(res_root, name, ".ttf", type, ASSET_TYPE_FONT_TTF)) != NULL) {
        break;
      }

      if ((info = try_load_assets(res_root, name, ".bin", type, ASSET_TYPE_FONT_BMP)) != NULL) {
        break;
      }
      break;
    }
    case ASSET_TYPE_SCRIPT: {
      if ((info = try_load_assets(res_root, name, ".js", type, ASSET_TYPE_SCRIPT_JS)) != NULL) {
        break;
      }

      if ((info = try_load_assets(res_root, name, ".lua", type, ASSET_TYPE_SCRIPT_LUA)) != NULL) {
        break;
      }
      break;
    }
    case ASSET_TYPE_STYLE: {
      if ((info = try_load_assets(res_root, name, ".bin", type, ASSET_TYPE_STYLE)) != NULL) {
        break;
      }
      break;
    }
    case ASSET_TYPE_STRINGS: {
      if ((info = try_load_assets(res_root, name, ".bin", type, ASSET_TYPE_STRINGS)) != NULL) {
        break;
      }
      break;
    }
    case ASSET_TYPE_IMAGE: {
      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_PNG, TRUE)) != NULL) {
        return info;
      }

      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_JPG, TRUE)) != NULL) {
        return info;
      }

      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_GIF, TRUE)) != NULL) {
        return info;
      }

      /*try ratio-insensitive image.*/
      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_PNG, FALSE)) != NULL) {
        return info;
      }

      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_JPG, FALSE)) != NULL) {
        return info;
      }

      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_GIF, FALSE)) != NULL) {
        return info;
      }

      if ((info = try_load_image(res_root, name, ASSET_TYPE_IMAGE_BSVG, FALSE)) != NULL) {
        return info;
      }

      break;
    }
    case ASSET_TYPE_UI: {
      if ((info = try_load_assets(res_root, name, ".bin", type, ASSET_TYPE_UI)) != NULL) {
        break;
      }
      break;
    }
    case ASSET_TYPE_XML: {
      if ((info = try_load_assets(res_root, name, ".xml", type, ASSET_TYPE_XML)) != NULL) {
        break;
      }
      break;
    }
    case ASSET_TYPE_DATA: {
      if ((info = try_load_assets(res_root, name, ".bin", type, ASSET_TYPE_DATA)) != NULL) {
        break;
      }

      if ((info = try_load_assets(res_root, name, ".dat", type, ASSET_TYPE_DATA)) != NULL) {
        break;
      }
      break;
    }
    default:
      break;
  }

  if (info != NULL) {
    assets_manager_add(rm, info);
    asset_info_unref(info);
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

  darray_init(&(rm->assets), init_nr, (tk_destroy_t)asset_info_unref,
              (tk_compare_t)asset_cache_cmp_type);

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
  return darray_push(&(rm->assets), (void*)r);
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

static const asset_info_t* assets_manager_ref_impl(assets_manager_t* rm, asset_type_t type,
                                                   const char* name) {
  const asset_info_t* info = assets_manager_find_in_cache(rm, type, name);

  if (info == NULL) {
    info = assets_manager_load(rm, type, name);
    /*加载时初始计数为1，缓存时自动增加引用计数，此处不需要引用*/
  } else {
    asset_info_ref((asset_info_t*)info);
  }

  return info;
}

const asset_info_t* assets_manager_ref(assets_manager_t* rm, asset_type_t type, const char* name) {
  const asset_info_t* info = NULL;
  return_value_if_fail(rm != NULL && name != NULL, NULL);
  if (strstr(name, TK_LOCALE_MAGIC) != NULL) {
    char locale[TK_NAME_LEN + 1];
    char real_name[TK_NAME_LEN + 1];
    const char* language = locale_info()->language;
    const char* country = locale_info()->country;

    tk_snprintf(locale, sizeof(locale) - 1, "%s_%s", language, country);
    tk_replace_locale(name, real_name, locale);
    info = assets_manager_ref_impl(rm, type, real_name);
    if (info != NULL) {
      return info;
    }

    tk_replace_locale(name, real_name, language);
    info = assets_manager_ref_impl(rm, type, real_name);
    if (info != NULL) {
      return info;
    }

    tk_replace_locale(name, real_name, "");
    info = assets_manager_ref_impl(rm, type, real_name);
    if (info != NULL) {
      return info;
    }

    return NULL;
  } else {
    return assets_manager_ref_impl(rm, type, name);
  }
}

ret_t assets_manager_unref(assets_manager_t* rm, const asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (rm == NULL) {
    /*asset manager was destroied*/
    return RET_OK;
  }

  if (!(info->is_in_rom)) {
    bool_t remove = info->refcount <= 1;

    if (remove) {
      tk_compare_t cmp = rm->assets.compare;
      rm->assets.compare = pointer_compare;
      if (darray_remove(&(rm->assets), (void*)info) == RET_NOT_FOUND) {
        asset_info_unref((asset_info_t*)info);
      }
      rm->assets.compare = cmp;
    }
  }

  return RET_OK;
}

ret_t assets_manager_clear_cache(assets_manager_t* rm, asset_type_t type) {
  asset_info_t info;

  memset(&info, 0x00, sizeof(info));
  info.type = type;
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  return darray_remove_all(&(rm->assets), &info);
}

ret_t assets_manager_deinit(assets_manager_t* rm) {
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rm->res_root);
  darray_deinit(&(rm->assets));

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
