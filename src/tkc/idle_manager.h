/**
 * File:   idle.h
 * Author: AWTK Develop Team
 * Brief:  idle manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IDLE_MANAGER_H
#define TK_IDLE_MANAGER_H

#include "tkc/slist.h"
#include "tkc/idle_info.h"

BEGIN_C_DECLS

struct _idle_manager_t {
  slist_t idles;
  uint32_t next_idle_id;
  uint32_t dispatch_times;
};

idle_manager_t* idle_manager(void);
ret_t idle_manager_set(idle_manager_t* idle_manager);

idle_manager_t* idle_manager_create(void);
idle_manager_t* idle_manager_init(idle_manager_t* idle_manager);
ret_t idle_manager_deinit(idle_manager_t* idle_manager);
ret_t idle_manager_destroy(idle_manager_t* idle_manager);

uint32_t idle_manager_count(idle_manager_t* idle_manager);
ret_t idle_manager_dispatch(idle_manager_t* idle_manager);
ret_t idle_manager_remove_all(idle_manager_t* idle_manager);
ret_t idle_manager_remove(idle_manager_t* idle_manager, uint32_t idle_id);
ret_t idle_manager_append(idle_manager_t* idle_manager, idle_info_t* idle);
const idle_info_t* idle_manager_find(idle_manager_t* idle_manager, uint32_t idle_id);
uint32_t idle_manager_add(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx);

END_C_DECLS

#endif /*TK_IDLE_MANAGER_H*/
