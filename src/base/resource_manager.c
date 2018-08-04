/**
 * File:   resource_manager.h
 * Author: AWTK Develop Team
 * Brief:  resource manager
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
#include "base/utils.h"
#include "base/system_info.h"
#include "base/resource_manager.h"

static resource_manager_t* s_resource_manager = NULL;

#ifdef WITH_FS_RES
#include "base/fs.h"

#ifndef RES_ROOT
#define RES_ROOT TK_ROOT "/demos/res/raw"
#endif

static resource_info_t* load_resource(uint16_t type, uint16_t subtype, uint32_t size,
                                      const char* path, const char* name) {
  resource_info_t* info = TKMEM_ALLOC(sizeof(resource_info_t) + size);
  return_value_if_fail(info != NULL, NULL);

  memset(info, 0x00, sizeof(resource_info_t));
  info->size = size;
  info->type = type;
  info->subtype = subtype;
  info->refcount = 1;
  info->is_in_rom = FALSE;
  strncpy(info->name, name, NAME_LEN);

  ENSURE(fs_read_file_part(path, info->data, size, 0) == size);

  return info;
}

resource_info_t* resource_manager_load(resource_manager_t* rm, resource_type_t type,
                                       const char* name) {
  int32_t size = 0;
  char path[MAX_PATH + 1];
  resource_info_t* info = NULL;
  system_info_t* sysinfo = system_info();
  float_t dpr = sysinfo->device_pixel_ratio;

  switch (type) {
    case RESOURCE_TYPE_FONT: {
      tk_snprintf(path, MAX_PATH, "%s/fonts/%s.ttf", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_FONT_TTF, size, path, name);
        break;
      }

      tk_snprintf(path, MAX_PATH, "%s/fonts/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_FONT_BMP, size, path, name);
        break;
      }

      break;
    }
    case RESOURCE_TYPE_THEME: {
      tk_snprintf(path, MAX_PATH, "%s/theme/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_THEME, size, path, name);
        break;
      }
      break;
    }
    case RESOURCE_TYPE_STRINGS: {
      tk_snprintf(path, MAX_PATH, "%s/strings/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_STRINGS, size, path, name);
      }
      break;
    }
    case RESOURCE_TYPE_IMAGE: {
      const char* ratio = "x1";
      if (dpr >= 3) {
        ratio = "x3";
      } else if (dpr >= 2) {
        ratio = "x2";
      }

      tk_snprintf(path, MAX_PATH, "%s/images/%s/%s.png", RES_ROOT, ratio, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_IMAGE_PNG, size, path, name);
        /*not cache png file raw data*/
        return info;
      }

      tk_snprintf(path, MAX_PATH, "%s/images/%s/%s.jpg", RES_ROOT, ratio, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_IMAGE_JPG, size, path, name);
        /*not cache png file jpg data*/
        return info;
      }

      break;
    }
    case RESOURCE_TYPE_UI: {
      tk_snprintf(path, MAX_PATH, "%s/ui/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_UI_BIN, size, path, name);
        /*not cache ui*/
        return info;
      }
      break;
    }
    case RESOURCE_TYPE_XML: {
      tk_snprintf(path, MAX_PATH, "%s/xml/%s.xml", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_XML, size, path, name);
        /*not cache xml*/
        return info;
      }
      break;
    }
    case RESOURCE_TYPE_DATA: {
      tk_snprintf(path, MAX_PATH, "%s/data/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_DATA, size, path, name);
        break;
      }
      break;
    }
    default:
      break;
  }

  if (info != NULL) {
    resource_manager_add(rm, info);
  }

  return info;
}
#else
resource_info_t* resource_manager_load(resource_manager_t* rm, resource_type_t type,
                                       const char* name) {
  (void)type;
  (void)name;
  return NULL;
}
#endif /*WITH_FS_RES*/

resource_manager_t* resource_manager(void) {
  return s_resource_manager;
}

static ret_t resource_info_destroy(resource_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    memset(info, 0x00, sizeof(resource_info_t));

    TKMEM_FREE(info);
  }

  return RET_OK;
}

static ret_t resource_info_unref(resource_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    if (info->refcount > 0) {
      info->refcount--;
      if (info->refcount == 0) {
        resource_info_destroy(info);
      }
    }
  }

  return RET_OK;
}

static ret_t resource_info_ref(resource_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    info->refcount++;
  }

  return RET_OK;
}

ret_t resource_manager_set(resource_manager_t* rm) {
  s_resource_manager = rm;

  return RET_OK;
}

resource_manager_t* resource_manager_create(uint32_t init_res_nr) {
  resource_manager_t* rm = TKMEM_ZALLOC(resource_manager_t);

  return resource_manager_init(rm, init_res_nr);
}

resource_manager_t* resource_manager_init(resource_manager_t* rm, uint32_t init_res_nr) {
  return_value_if_fail(rm != NULL, NULL);

  array_init(&(rm->resources), init_res_nr);

  return rm;
}

ret_t resource_manager_add(resource_manager_t* rm, const void* info) {
  const resource_info_t* r = (const resource_info_t*)info;
  return_value_if_fail(rm != NULL && info != NULL, RET_BAD_PARAMS);

  resource_info_ref((resource_info_t*)r);

  return array_push(&(rm->resources), (void*)r);
}

const resource_info_t* resource_manager_find_in_cache(resource_manager_t* rm, resource_type_t type,
                                                      const char* name) {
  uint32_t i = 0;
  const resource_info_t* iter = NULL;
  const resource_info_t** all = NULL;
  return_value_if_fail(rm != NULL && name != NULL, NULL);

  all = (const resource_info_t**)(rm->resources.elms);

  for (i = 0; i < rm->resources.size; i++) {
    iter = all[i];
    if (type == iter->type && strcmp(name, iter->name) == 0) {
      return iter;
    }
  }

  return NULL;
}

const resource_info_t* resource_manager_ref(resource_manager_t* rm, resource_type_t type,
                                            const char* name) {
  const resource_info_t* info = resource_manager_find_in_cache(rm, type, name);

  if (info == NULL) {
    info = resource_manager_load(rm, type, name);
    /*加载时初始计数为1，缓存时自动增加引用计数，此处不需要引用*/
  } else {
    resource_info_ref((resource_info_t*)info);
  }

  return info;
}

ret_t resource_manager_unref(resource_manager_t* rm, const resource_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (rm == NULL) {
    /*resource manager was destroied*/
    return RET_OK;
  }

  if (!(info->is_in_rom)) {
    bool_t remove = info->refcount <= 1;

    resource_info_unref(info);
    if (remove) {
      array_remove(&(rm->resources), NULL, (void*)info, NULL);
    }
  }

  return RET_OK;
}

static int res_cache_cmp_type(const void* a, const void* b) {
  const resource_info_t* aa = (const resource_info_t*)a;
  const resource_info_t* bb = (const resource_info_t*)b;

  if (aa->is_in_rom) {
    return -1;
  }

  return aa->type - bb->type;
}

ret_t resource_manager_clear_cache(resource_manager_t* rm, resource_type_t type) {
  resource_type_t res = {type};
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  return array_remove_all(&(rm->resources), res_cache_cmp_type, &res,
                          (tk_destroy_t)resource_info_destroy);
}

ret_t resource_manager_deinit(resource_manager_t* rm) {
  uint32_t i = 0;
  resource_info_t* iter = NULL;
  resource_info_t** all = NULL;
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  all = (resource_info_t**)(rm->resources.elms);

  for (i = 0; i < rm->resources.size; i++) {
    iter = all[i];
    resource_info_destroy(iter);
  }

  array_deinit(&(rm->resources));

  return RET_OK;
}

ret_t resource_manager_destroy(resource_manager_t* rm) {
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);
  resource_manager_deinit(rm);

  if (rm == resource_manager()) {
    resource_manager_set(NULL);
  }
  TKMEM_FREE(rm);

  return RET_OK;
}
