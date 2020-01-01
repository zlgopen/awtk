/**
 * File:   asset_info.c
 * Author: AWTK Develop Team
 * Brief:  asset info
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/asset_info.h"

asset_info_t* asset_info_create(uint16_t type, uint16_t subtype, const char* name, int32_t size) {
  asset_info_t* info = TKMEM_ALLOC(sizeof(asset_info_t) + size);
  return_value_if_fail(info != NULL, NULL);

  memset(info, 0x00, sizeof(asset_info_t));
  info->size = size;
  info->type = type;
  info->subtype = subtype;
  info->refcount = 1;
  info->is_in_rom = FALSE;
  strncpy(info->name, name, TK_NAME_LEN);

  return info;
}

ret_t asset_info_destroy(asset_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  if (!(info->is_in_rom)) {
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
