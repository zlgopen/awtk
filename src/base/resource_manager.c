/**
 * File:   resource_manager.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  resource manager
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
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
      snprintf(path, MAX_PATH, "%s/fonts/%s.ttf", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_FONT_TTF, size, path, name);
        break;
      }

      snprintf(path, MAX_PATH, "%s/fonts/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_FONT_BMP, size, path, name);
        break;
      }

      break;
    }
    case RESOURCE_TYPE_THEME: {
      snprintf(path, MAX_PATH, "%s/theme/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_THEME, size, path, name);
        break;
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

      snprintf(path, MAX_PATH, "%s/images/%s/%s.png", RES_ROOT, ratio, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_IMAGE_PNG, size, path, name);
        break;
      }
      break;
    }
    case RESOURCE_TYPE_UI: {
      snprintf(path, MAX_PATH, "%s/ui/%s.bin", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_UI_BIN, size, path, name);
        break;
      }
      break;
    }
    case RESOURCE_TYPE_XML: {
      snprintf(path, MAX_PATH, "%s/xml/%s.xml", RES_ROOT, name);
      size = fs_file_size(path);
      if (size > 0) {
        info = load_resource(type, RESOURCE_TYPE_XML, size, path, name);
        break;
      }
      break;
    }
    case RESOURCE_TYPE_DATA: {
      snprintf(path, MAX_PATH, "%s/data/%s.bin", RES_ROOT, name);
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

resource_manager_t* resource_manager(void) { return s_resource_manager; }

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
  return_value_if_fail(rm != NULL && info != NULL, RET_BAD_PARAMS);

  return array_push(&(rm->resources), (void*)info) ? RET_OK : RET_FAIL;
}

const resource_info_t* resource_manager_ref(resource_manager_t* rm, resource_type_t type,
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

  return resource_manager_load(rm, type, name);
}

ret_t resource_manager_unref(resource_manager_t* rm, const resource_info_t* info) {
  return_value_if_fail(rm != NULL && info != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t resource_manager_deinit(resource_manager_t* rm) {
  uint32_t i = 0;
  resource_info_t* iter = NULL;
  resource_info_t** all = NULL;
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);

  all = (resource_info_t**)(rm->resources.elms);

  for (i = 0; i < rm->resources.size; i++) {
    iter = all[i];
    if (!iter->is_in_rom) {
      TKMEM_FREE(iter);
    }
  }

  array_deinit(&(rm->resources));

  return RET_OK;
}

ret_t resource_manager_destroy(resource_manager_t* rm) {
  return_value_if_fail(rm != NULL, RET_BAD_PARAMS);
  resource_manager_deinit(rm);

  TKMEM_FREE(rm);

  return RET_OK;
}
