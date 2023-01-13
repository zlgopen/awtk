/**
 * File:   asset_info.c
 * Author: AWTK Develop Team
 * Brief:  asset info
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/utils.h"
#include "tkc/asset_info.h"

static ret_t asset_info_set_full_name_flag(asset_info_t* info);
static bool_t asset_info_is_full_name(const asset_info_t* info);

const char* asset_info_get_formatted_name(const char* name) {
  uint32_t len = 0;
  return_value_if_fail(name != NULL, NULL);
  len = tk_strlen(name);
  if (len < TK_NAME_LEN) {
    return name;
  } else {
    uint32_t p = len - TK_NAME_LEN;
    return name + p;
  }
}

asset_info_t* asset_info_create(uint16_t type, uint16_t subtype, const char* name, int32_t size) {
  asset_info_t* info = NULL;
  uint32_t total = sizeof(asset_info_t) + size + 1;
  return_value_if_fail(name != NULL, NULL);

  info = TKMEM_ALLOC(total);
  return_value_if_fail(info != NULL, NULL);

  memset(info, 0x00, total);

  info->size = size;
  info->type = type;
  info->subtype = subtype;
  info->refcount = 1;
  asset_info_set_is_in_rom(info, FALSE);
  asset_info_set_name(info, name, TRUE);

#ifdef LOAD_ASSET_WITH_MMAP
  if (size > 0) {
    info->data = TKMEM_ALLOC(size + 1);
    if (info->data != NULL) {
      memset(info->data, 0x00, size + 1);
    } else {
      TKMEM_FREE(info);
      return NULL;
    }
  }
#endif /*LOAD_ASSET_WITH_MMAP*/

  return info;
}

ret_t asset_info_destroy(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!asset_info_is_in_rom(info)) {
#ifdef LOAD_ASSET_WITH_MMAP
    if (info->map != NULL) {
      mmap_destroy(info->map);
    } else if (info->data != NULL) {
      TKMEM_FREE(info->data);
    }
#endif /*LOAD_ASSET_WITH_MMAP*/

    if (asset_info_is_full_name(info)) {
      TKMEM_FREE(info->name.full_name);
    }

    memset(info, 0x00, sizeof(asset_info_t));
    TKMEM_FREE(info);
  }

  return RET_OK;
}

ret_t asset_info_unref(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!asset_info_is_in_rom(info)) {
    if (info->refcount > 0) {
      info->refcount--;
      if (info->refcount == 0) {
        asset_info_destroy(info);
      }
    }
  }

  return RET_OK;
}

ret_t asset_info_ref(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!asset_info_is_in_rom(info)) {
    info->refcount++;
  }

  return RET_OK;
}

uint16_t asset_info_get_type(asset_info_t* info) {
  return_value_if_fail(info != NULL, ASSET_TYPE_NONE);

  return info->type;
}

const char* asset_info_get_name(const asset_info_t* info) {
  return_value_if_fail(info != NULL, NULL);
  if (asset_info_is_full_name(info)) {
    return info->name.full_name;
  } else {
    return info->name.small_name;
  }
}

bool_t asset_info_is_in_rom(const asset_info_t* info) {
  return_value_if_fail(info != NULL, FALSE);
  return info->flags & ASSET_INFO_FLAG_IN_ROM;
}

ret_t asset_info_set_is_in_rom(asset_info_t* info, bool_t is_in_rom) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (is_in_rom) {
    info->flags |= ASSET_INFO_FLAG_IN_ROM;
  } else {
    info->flags &= ~ASSET_INFO_FLAG_IN_ROM;
  }
  return RET_OK;
}

static ret_t asset_info_set_full_name_flag(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  info->flags |= ASSET_INFO_FLAG_FULL_NAME;
  return RET_OK;
}

static bool_t asset_info_is_full_name(const asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  return info->flags & ASSET_INFO_FLAG_FULL_NAME;
}

ret_t asset_info_set_name(asset_info_t* info, const char* name, bool_t is_alloc) {
  uint32_t len = 0;
  return_value_if_fail(info != NULL && name != NULL, RET_BAD_PARAMS);
  len = tk_strlen(name);
  if (len <= TK_NAME_LEN) {
    memset(info->name.small_name, 0x0, sizeof(info->name.small_name));
    tk_memcpy(info->name.small_name, name, len);
  } else {
    if (is_alloc) {
      info->name.full_name = tk_strdup(name);
    } else {
      info->name.full_name = (char*)name;
    }
    asset_info_set_full_name_flag(info);
  }
  return RET_OK;
}
