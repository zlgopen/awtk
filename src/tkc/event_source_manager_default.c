/**
 * File:   event_source_manager_default.c
 * Author: AWTK Develop Team
 * Brief:  event manager_default manager_default
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/platform.h"
#include "tkc/event_source_manager_default.h"

#ifdef WITH_SOCKET

#ifdef WIN32
#include "windows.h"
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
#else
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif /*WIN32*/

static ret_t event_source_manager_default_dispatch_fds(event_source_manager_t* manager,
                                                       uint32_t sleep_time) {
  fd_set fdsr;
  uint32_t i = 0;
  int32_t fd = 0;
  int32_t ret = 0;
  int32_t max_fd = 0;
  struct timeval tv = {0, 0};
  event_source_t* iter = NULL;
  event_source_t** sources = NULL;
  return_value_if_fail(manager != NULL, 0);

  FD_ZERO(&fdsr);
  tv.tv_sec = sleep_time / 1000;
  tv.tv_usec = (sleep_time % 1000) * 1000;

  sources = (event_source_t**)(manager->dispatching_sources.elms);

  for (i = 0; i < manager->dispatching_sources.size; i++) {
    iter = sources[i];
    fd = event_source_get_fd(iter);
    if (fd >= 0) {
      FD_SET(fd, &fdsr);
      if (fd > max_fd) {
        max_fd = fd;
      }
    }
  }

  if (max_fd == 0) {
    return RET_OK;
  }

  ret = select(max_fd + 1, &fdsr, NULL, NULL, &tv);
  if (ret < 0) {
    perror("select");
    return RET_FAIL;
  } else if (ret == 0) {
    return RET_TIMEOUT;
  }

  for (i = 0; i < manager->dispatching_sources.size; i++) {
    iter = sources[i];
    fd = event_source_get_fd(iter);

    if (fd >= 0) {
      if (FD_ISSET(fd, &fdsr)) {
        ret_t r = event_source_dispatch(iter);
        if (r == RET_REMOVE) {
          event_source_manager_remove(manager, iter);
        }
      }
    }
  }

  return RET_OK;
}
#else
static ret_t event_source_manager_default_dispatch_fds(event_source_manager_t* manager,
                                                       uint32_t sleep_time) {
  return RET_OK;
}
#endif /*WITH_SOCKET*/

static ret_t event_source_manager_default_dispatch_no_fd(event_source_manager_t* manager) {
  uint32_t i = 0;
  int32_t fd = 0;
  event_source_t* iter = NULL;
  event_source_t** sources = NULL;
  return_value_if_fail(manager != NULL, RET_BAD_PARAMS);

  sources = (event_source_t**)(manager->dispatching_sources.elms);

  for (i = 0; i < manager->dispatching_sources.size; i++) {
    iter = sources[i];
    fd = event_source_get_fd(iter);
    if (fd >= 0) {
      continue;
    }
    if (event_source_check(iter) == RET_OK) {
      ret_t r = event_source_dispatch(iter);
      if (r == RET_REMOVE) {
        event_source_manager_remove(manager, iter);
      }
    }
  }

  return RET_OK;
}

static ret_t event_source_manager_default_dispatch(event_source_manager_t* manager) {
  uint32_t sleep_time = event_source_manager_get_wakeup_time(manager);

  event_source_manager_default_dispatch_fds(manager, sleep_time);

  return event_source_manager_default_dispatch_no_fd(manager);
}

static ret_t event_source_manager_default_destroy(event_source_manager_t* manager) {
  return_value_if_fail(manager != NULL && manager->dispatch != NULL, RET_BAD_PARAMS);

  memset(manager, 0x00, sizeof(event_source_manager_t));
  TKMEM_FREE(manager);

  return RET_OK;
}

event_source_manager_t* event_source_manager_default_create(void) {
  event_source_manager_t* manager = TKMEM_ZALLOC(event_source_manager_t);
  return_value_if_fail(manager != NULL, NULL);

  event_source_manager_init(manager);
  manager->dispatch = event_source_manager_default_dispatch;
  manager->destroy = event_source_manager_default_destroy;

  return manager;
}
