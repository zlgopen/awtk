/**
 * File:   serial_helper.c
 * Author: AWTK Develop Team
 * Brief:  serial helper functions
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

/*
 *the following code adapted from: https://github.com/wjwwood/serial/tree/master/src/impl
 *
 * Copyright 2012 William Woodall and John Harrison
 *
 * Additional Contributors: Christopher Baker @bakercp
 */

#include "tkc/mem.h"
#include "tkc/wstr.h"
#include "tkc/thread.h"
#include "tkc/socket_pair.h"
#include "tkc/socket_helper.h"
#include "streams/serial/serial_helper.h"

#ifdef __AWORKS_LP__

#include "aworks.h"
#include "aw_stropts.h"
#include "aw_fcntl.h"
#include "aw_unistd.h"

#include "aw_serial.h"
#include "tkc/ring_buffer.h"
#include "tkc/darray.h"

static darray_t* s_darray = NULL;

typedef struct {
  tk_thread_t* thread;
  serial_handle_t handle;
  bool_t broken;
  ring_buffer_t* rb;
  bool_t is_usb2uart;
} serial_dev_prv_t;

static int da_serial_cmp(const void* a, const void* b) {
  serial_dev_prv_t* pa = a;
  if (pa->handle == b) {
    return 0;
  }
  return -1;
}

static ret_t da_serial_destroy(void* data) {
  serial_dev_prv_t* p_this = data;

  if (p_this->thread) {
    p_this->broken = TRUE;
    ENSURE(tk_thread_join(p_this->thread) == RET_OK);
    tk_thread_destroy(p_this->thread);
  }

  ring_buffer_destroy(p_this->rb);
  TKMEM_FREE(p_this);
  return RET_OK;
}

static void* thread_serial_recv(void* args) {
  serial_dev_prv_t* p_this = args;
  int32_t cnt = 0;
  int fd = serial_handle_get_fd(p_this->handle);
  uint8_t buff[64];
  ENSURE(p_this && fd >= 0);

  while (!p_this->broken) {
    cnt = aw_read(fd, buff, sizeof(buff));
    if (cnt > 0) {
      ring_buffer_write_len(p_this->rb, buff, cnt);
    } else {
      sleep_ms(10);
    }
  }
  return NULL;
}

int serial_handle_get_fd(serial_handle_t handle) {
  return handle->dev;
}

serial_dev_t serial_handle_get_dev(serial_handle_t handle) {
  return handle->dev;
}

serial_handle_t serial_open(const char* port) {
  serial_dev_prv_t* p_this = TKMEM_ZALLOC(serial_dev_prv_t);
  serial_handle_t handle = TKMEM_ZALLOC(serial_info_t);
  return_value_if_fail(p_this && handle != NULL && port != NULL && *port != '\0', NULL);
  handle->dev = aw_open(port, AW_O_RDWR, 0);
  return_value_if_fail(handle, NULL);

  if (handle->dev < 0) {
    TKMEM_FREE(p_this);
    TKMEM_FREE(handle);
    return_value_if_fail(FALSE, NULL);
  }

  memset(p_this, 0, sizeof(serial_dev_prv_t));
  p_this->handle = handle;
  p_this->rb = ring_buffer_create(256, 4096);
  ENSURE(p_this->rb);

  if (strstr(port, "USB")) {
    p_this->is_usb2uart = TRUE;
  }

  if (!s_darray) {
    s_darray = darray_create(5, da_serial_destroy, da_serial_cmp);
    ENSURE(s_darray);
  }
  ENSURE(darray_push(s_darray, p_this) == RET_OK);

  return handle;
}

ret_t serial_iflush(serial_handle_t handle) {
  int fd = serial_handle_get_fd(handle);
  return aw_ioctl(fd, AW_FIORFLUSH, NULL) == 0 ? RET_OK : RET_FAIL;
}

ret_t serial_oflush(serial_handle_t handle) {
  int fd = serial_handle_get_fd(handle);
  serial_dev_prv_t* p_this = darray_find(s_darray, handle);
  return_value_if_fail(p_this, RET_FAIL);
  ring_buffer_reset(p_this->rb);
  return aw_ioctl(fd, AW_FIOWFLUSH, NULL) == 0 ? RET_OK : RET_FAIL;
}

ret_t serial_wait_for_data(serial_handle_t handle, uint32_t timeout_ms) {
  serial_dev_prv_t* p_this = darray_find(s_darray, handle);
  return_value_if_fail(p_this, RET_FAIL);
  if (ring_buffer_size(p_this->rb)) {
    return RET_OK;
  }
  sleep_ms(timeout_ms);
  return RET_TIMEOUT;
}

int32_t serial_read(serial_handle_t handle, uint8_t* buff, uint32_t max_size) {
  int fd = serial_handle_get_fd(handle);
  serial_dev_prv_t* p_this = darray_find(s_darray, handle);
  return_value_if_fail(fd >= 0, RET_FAIL);
  return ring_buffer_read(p_this->rb, buff, max_size);
}

int32_t serial_write(serial_handle_t handle, const uint8_t* buff, uint32_t max_size) {
  int fd = serial_handle_get_fd(handle);
  return_value_if_fail(fd >= 0, RET_FAIL);
  int32_t cnt = aw_write(fd, buff, max_size);
  return_value_if_fail(cnt >= 0, cnt);
  return cnt;
}

int serial_close(serial_handle_t handle) {
  int fd = serial_handle_get_fd(handle);
  serial_dev_prv_t* p_this = darray_find(s_darray, handle);
  return_value_if_fail(aw_close(fd) == 0, RET_FAIL);
  darray_remove(s_darray, handle);
  return RET_OK;
}

ret_t serial_config(serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize,
                    stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity) {
  struct aw_serial_dcb dcb;
  struct aw_serial_timeout timeout;
  ret_t ret;
  serial_dev_prv_t* p_this = darray_find(s_darray, handle);

  int fd = serial_handle_get_fd(handle);
  return_value_if_fail(flowcontrol != flowcontrol_software, RET_FAIL);
  return_value_if_fail(fd >= 0 && p_this, RET_FAIL);

  if (!p_this->is_usb2uart) {
    ret = aw_serial_dcb_get(fd, &dcb);
    return_value_if_fail(ret == RET_OK, RET_FAIL);

    dcb.baud_rate = baudrate;
    dcb.byte_size = bytesize;
    dcb.stop_bits = stopbits;

    if (flowcontrol == flowcontrol_hardware) {
      dcb.f_rtsctrl = 1;
      dcb.f_ctsflow = 1;
    }
    if (parity == 1) {
      dcb.parity = 1;
    }

    ret = aw_serial_dcb_set(fd, &dcb);
    return_value_if_fail(ret == RET_OK, RET_FAIL);

    /* 串口超时配置获取 */
    ret = aw_serial_timeout_get(fd, &timeout);
    return_value_if_fail(ret == RET_OK, RET_FAIL);

    timeout.rd_timeout = 1000;        /* 读总超时 1s */
    timeout.rd_interval_timeout = 50; /* 码间超时为 50ms */

    /* 配置串口超时 */
    ret = aw_serial_timeout_set(fd, &timeout);
    return_value_if_fail(ret == RET_OK, RET_FAIL);
  }

  if (!p_this->thread) {
    p_this->thread = tk_thread_create(thread_serial_recv, p_this);
    //  tk_thread_set_priority(p_this->thread, 6);
    tk_thread_set_stack_size(p_this->thread, 2048);
    ENSURE(tk_thread_start(p_this->thread) == RET_OK);
  }

  return RET_OK;
}
#endif /* __AWORKS_LP__ */

#ifdef __AWORKS__

#include "aworks.h"
#include "io/aw_stropts.h"
#include "io/aw_fcntl.h"
#include "io/aw_unistd.h"

#include "aw_serial.h"

int serial_handle_get_fd(serial_handle_t handle) {
  return handle->dev;
}

serial_dev_t serial_handle_get_dev(serial_handle_t handle) {
  return handle->dev;
}

serial_handle_t serial_open(const char* port) {
  serial_handle_t handle = TKMEM_ZALLOC(serial_info_t);
  return_value_if_fail(handle != NULL && port != NULL && *port != '\0', NULL);
  handle->dev = aw_open(port, 0, O_RDWR);

  return handle;
}

ret_t serial_iflush(serial_handle_t handle) {
  int fd = serial_handle_get_fd(handle);
  return aw_ioctl(fd, AW_FIORFLUSH, NULL) == 0 ? RET_OK : RET_FAIL;
}

ret_t serial_oflush(serial_handle_t handle) {
  int fd = serial_handle_get_fd(handle);
  return aw_ioctl(fd, AW_FIOWFLUSH, NULL) == 0 ? RET_OK : RET_FAIL;
}

ret_t serial_wait_for_data(serial_handle_t handle, uint32_t timeout_ms) {
  int len = 0;
  int fd = serial_handle_get_fd(handle);
  while (timeout_ms) {
    aw_ioctl(fd, AW_FIONREAD, &len);
    if (len > 0) {
      return RET_OK;
    }
    sleep_ms(10);
    timeout_ms -= tk_min(timeout_ms, 10);
  }
  return RET_TIMEOUT;
}

int32_t serial_read(serial_handle_t handle, uint8_t* buff, uint32_t max_size) {
  int fd = serial_handle_get_fd(handle);
  return aw_read(fd, buff, max_size);
}

int32_t serial_write(serial_handle_t handle, const uint8_t* buff, uint32_t max_size) {
  int fd = serial_handle_get_fd(handle);
  return aw_write(fd, buff, max_size);
}

int serial_close(serial_handle_t handle) {
  int fd = serial_handle_get_fd(handle);
  return aw_close(fd);
}

ret_t serial_config(serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize,
                    stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity) {
  struct aw_serial_dcb dcb;
  int fd = serial_handle_get_fd(handle);
  return_value_if_fail(flowcontrol != flowcontrol_software, RET_FAIL);

  memset(&dcb, 0, sizeof(dcb));
  dcb.baud_rate = baudrate;
  dcb.byte_size = bytesize;
  dcb.stop_bits = stopbits;

  if (flowcontrol == flowcontrol_hardware) {
    dcb.f_rtsctrl = 1;
    dcb.f_ctsflow = 1;
  }
  if (parity == 1) {
    dcb.parity = 1;
  }

  if (0) {
    struct aw_serial_timeout tm = {.rd_interval_timeout = 10, .rd_timeout = 100};
    aw_ioctl(fd, AW_TIOCRDTIMEOUT, &tm);
  }
  return aw_ioctl(fd, AW_TIOSETOPTIONS, &dcb);
}

#endif /* __AWORKS__ */
