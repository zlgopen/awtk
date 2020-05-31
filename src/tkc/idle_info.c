/**
 * File:   idle_info.h
 * Author: AWTK Develop Team
 * Brief:  idle info
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/idle_info.h"
#include "tkc/idle_manager.h"

#ifdef AWTK_WEB_JS
#include <emscripten.h>
#endif /*AWTK_WEB_JS*/

static ret_t idle_info_on_destroy(idle_info_t* info) {
  if (info->on_destroy != NULL) {
    info->on_destroy(info);
  }

#ifdef AWTK_WEB_JS
  EM_ASM_INT({ return TBrowser.releaseFunction($0); }, info->on_idle);
#endif /*AWTK_WEB_JS*/

  return RET_OK;
}

static const object_vtable_t s_idle_info_vtable = {
    .type = "idle_info",
    .desc = "idle_info",
    .size = sizeof(idle_info_t),
    .is_collection = FALSE,
    .on_destroy = (object_on_destroy_t)idle_info_on_destroy};

idle_info_t* idle_info_create(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx) {
  object_t* obj = object_create(&s_idle_info_vtable);
  idle_info_t* idle = IDLE_INFO(obj);
  return_value_if_fail(idle != NULL, NULL);

  idle->ctx = ctx;
  idle->on_idle = on_idle;

  if (idle_manager != NULL) {
    idle->id = idle_manager->next_idle_id++;
    if (idle->id == TK_INVALID_ID) {
      idle->id = idle_manager->next_idle_id++;
    }
    idle_manager_append(idle_manager, idle);
  }

  return idle;
}

int idle_info_compare(const void* a, const void* b) {
  return ((const idle_info_t*)a)->id - ((const idle_info_t*)b)->id;
}

idle_info_t* idle_info_init_dummy(idle_info_t* idle, uint32_t id) {
  return_value_if_fail(idle != NULL, NULL);
  memset(idle, 0x00, sizeof(idle_info_t));

  idle->id = id;

  return idle;
}

idle_info_t* idle_info_cast(idle_info_t* idle) {
  return_value_if_fail(idle != NULL, NULL);

  return idle;
}

bool_t idle_info_is_available(idle_info_t* idle, uint32_t dispatch_id) {
  return idle != NULL && !(idle->busy) && idle->dispatch_id != dispatch_id;
}

ret_t idle_info_on_idle(idle_info_t* idle, uint32_t dispatch_id) {
  ret_t ret = RET_OK;
  return_value_if_fail(idle != NULL && idle->on_idle != NULL, RET_BAD_PARAMS);

  if (idle->busy) {
    return RET_BUSY;
  }

  idle->busy = TRUE;
  idle->dispatch_id = dispatch_id;
  ret = idle->on_idle(idle);
  idle->busy = FALSE;

  return ret;
}
