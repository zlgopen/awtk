/**
 * File:   asset_info.c
 * Author: AWTK Develop Team
 * Brief:  asset info
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  const char* asset_name = asset_info_get_formatted_name(name);
  return_value_if_fail(asset_name != NULL, NULL);

  info = TKMEM_ALLOC(total);
  return_value_if_fail(info != NULL, NULL);

  memset(info, 0x00, total);

  info->size = size;
  info->type = type;
  info->subtype = subtype;
  info->refcount = 1;
  info->is_in_rom = FALSE;
  strncpy(info->name, asset_name, TK_NAME_LEN);

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

  if (asset_name != name) {
    log_warn(
        "[warn] asset name max length is %d , so old name : \"%s\" format to new name : \"%s\" ! "
        "\r\n",
        TK_NAME_LEN + 1, name, asset_name);
  }

  return info;
}

ret_t asset_info_destroy(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
#ifdef LOAD_ASSET_WITH_MMAP
    if (info->map != NULL) {
      mmap_destroy(info->map);
    } else if (info->data != NULL) {
      TKMEM_FREE(info->data);
    }
#endif /*LOAD_ASSET_WITH_MMAP*/

    memset(info, 0x00, sizeof(asset_info_t));
    TKMEM_FREE(info);
  }

  return RET_OK;
}

ret_t asset_info_unref(asset_info_t* info) {
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

ret_t asset_info_ref(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
    info->refcount++;
  }

  return RET_OK;
}

uint16_t asset_info_get_type(asset_info_t* info) {
  return_value_if_fail(info != NULL, ASSET_TYPE_NONE);

  return info->type;
}

const char* asset_info_get_name(asset_info_t* info) {
  return_value_if_fail(info != NULL, NULL);

  return info->name;
}
