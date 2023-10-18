/**
 * File:   process_helper.c
 * Author: AWTK Develop Team
 * Brief:  process helper
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-09-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef WITH_WASM

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/socket_pair.h"
#include "tkc/socket_helper.h"
#include "process_helper.h"

#define CMD_LINE_SIZE  8 * 1024

#ifdef WIN32

static ret_t process_close_server_sock(process_handle_t handle) {
  if (handle->broken && tk_socket_wait_for_data(handle->server_fd, 1) == RET_TIMEOUT) {
    tk_socket_close(handle->server_fd);
    handle->server_fd = -1;
  }
  return RET_OK;
}

static void* process_read_pipe_on_thread(void* args) {
  process_handle_t handle = (process_handle_t)args;
  return_value_if_fail(handle != NULL, NULL);

  while (!handle->quit) {
    DWORD size = 0;
    uint32_t pos = 0;
    char buff[1024];
    BOOL ret = ReadFile(handle->h_std_out_rd, buff, sizeof(buff), &size, NULL);
		if (!ret || size < 0) {
      handle->broken = TRUE;
      break;
    }
    while (size > 0) {
      uint32_t len = tk_socket_send(handle->server_fd, buff + pos, size, 0);
      if (len <= 0) {
        log_warn("send fb buff fail \r\n");
      }
      size -= len;
      pos += len;
    }
  }
  process_close_server_sock(handle);
  return NULL;
}

ret_t process_destroy(process_handle_t handle) {
  return_value_if_fail(handle != NULL, RET_BAD_PARAMS);
  handle->quit = TRUE;

  // TerminateProcess

  if (handle->h_std_out_rd != NULL) {
    CloseHandle(handle->h_std_out_rd);
    handle->h_std_out_rd = NULL;
  }
  if (handle->h_std_in_wr != NULL) {
    CloseHandle(handle->h_std_in_wr);
    handle->h_std_in_wr = NULL;
  }

  if (handle->rthread != NULL) {
    tk_thread_join(handle->rthread);
    tk_thread_destroy(handle->rthread);
    handle->rthread = NULL;
  }

  tk_socket_close(handle->client_fd);
  if (handle->server_fd > 0) {
    tk_socket_close(handle->server_fd);
  }

  TKMEM_FREE(handle->file_path);
  wstr_reset(&handle->cmd_line);
  TKMEM_FREE(handle);
  return RET_OK;
}

process_handle_t process_create(const char* file_path, const char** args, uint32_t argc) {
  uint32_t i = 0;
  BOOL ret = FALSE;
  HANDLE h_std_in_rd;
  HANDLE h_std_out_wr;
  int socks[2] = {0, 0};
  wchar_t* str_tmp = NULL;
	SECURITY_ATTRIBUTES sa_attr;
  process_handle_t handle = TKMEM_ZALLOC(process_info_t);
  return_value_if_fail(handle != NULL, NULL);

	sa_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa_attr.bInheritHandle = TRUE;
	sa_attr.lpSecurityDescriptor = NULL;

	goto_error_if_fail(CreatePipe(&handle->h_std_out_rd, &h_std_out_wr, &sa_attr, 0));
	goto_error_if_fail(SetHandleInformation(handle->h_std_out_rd, HANDLE_FLAG_INHERIT, 0));

	goto_error_if_fail(CreatePipe(&h_std_in_rd, &handle->h_std_in_wr, &sa_attr, 0));
	goto_error_if_fail(SetHandleInformation(handle->h_std_in_wr, HANDLE_FLAG_INHERIT, 0));

  handle->start_info.cb = sizeof(STARTUPINFO);
	handle->start_info.hStdError = h_std_out_wr;
	handle->start_info.hStdOutput = h_std_out_wr;
	handle->start_info.hStdInput = h_std_in_rd;
  handle->start_info.wShowWindow = SW_HIDE;
	handle->start_info.dwFlags |= (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);

  wstr_init(&handle->cmd_line, CMD_LINE_SIZE);
  if (file_path != NULL) {
    handle->file_path = tk_wstr_dup_utf8(file_path);
  }

  for (i = 0; i < argc; i++) {
    str_tmp = tk_wstr_dup_utf8(args[i]);
    wstr_append(&handle->cmd_line, str_tmp);
    TKMEM_FREE(str_tmp);
    if (i != argc) {
      wstr_push(&handle->cmd_line, L' ');
    }
  }

	ret = CreateProcessW(handle->file_path,
                      handle->cmd_line.str,     // command line 
                      NULL,                     // process security attributes 
                      NULL,                     // primary thread security attributes 
                      TRUE,                     // handles are inherited 
                      0,                        // creation flags 
                      NULL,                     // use parent's environment 
                      NULL,                     // use parent's current directory 
                      &handle->start_info,      // STARTUPINFO pointer 
                      &handle->proc_info);      // receives PROCESS_INFORMATION 
  
  goto_error_if_fail(ret);

  CloseHandle(handle->proc_info.hProcess);
  CloseHandle(handle->proc_info.hThread);
  CloseHandle(h_std_out_wr);
  CloseHandle(h_std_in_rd);

  goto_error_if_fail(tk_socketpair(socks) == 0);
  handle->client_fd = socks[0];
  handle->server_fd = socks[1];

  handle->rthread = tk_thread_create(process_read_pipe_on_thread, handle);
  goto_error_if_fail(handle->rthread != NULL);
  goto_error_if_fail(tk_thread_start(handle->rthread) == RET_OK);
  
  return handle;
error :
  process_destroy(handle);
  return NULL;
}

int process_handle_get_fd(process_handle_t handle) {
  return_value_if_fail(handle != NULL, -1);
  return handle->client_fd;
}

ret_t process_wait_for_data(process_handle_t handle, uint32_t timeout_ms) {
  int fd = process_handle_get_fd(handle);
  return tk_socket_wait_for_data(fd, timeout_ms);
}

int32_t process_read(process_handle_t handle, uint8_t* buff, uint32_t max_size) {
  int fd = process_handle_get_fd(handle);
  int32_t size = tk_socket_recv(fd, buff, max_size, 0);
  process_close_server_sock(handle);
  return size;
}

int32_t process_write(process_handle_t handle, const uint8_t* buff, uint32_t max_size) {
  uint32_t size = 0;
  return_value_if_fail(handle != NULL && buff != NULL, -1);

  goto_error_if_fail(WriteFile(handle->h_std_in_wr, buff, max_size, &size, NULL));
  return size;
error:
  return -1;
}

bool_t process_is_broken(process_handle_t handle) {
  return_value_if_fail(handle != NULL, TRUE);
  return handle->broken;
}

#elif defined(LINUX) || defined(MACOS)

static ret_t process_close_read_pipe(process_handle_t handle) {
  if (handle->broken && tk_socket_wait_for_data(handle->read_pfd[0], 1) == RET_TIMEOUT) {
    close(handle->read_pfd[1]);
    close(handle->write_pfd[0]);
    handle->read_pfd[1] = -1;
    handle->write_pfd[0] = -1;
  }
  return RET_OK;
}

static void* process_wait_for_on_thread(void* args) {
  int stat = 0;
  process_handle_t handle = (process_handle_t)args;
  return_value_if_fail(handle != NULL, NULL);

  while (waitpid(handle->pid, &stat, 0) < 0) {
    break_if_fail(errno == EINTR);
  }

  handle->broken = TRUE;
  process_close_read_pipe(handle);

  return NULL;
}

ret_t process_destroy(process_handle_t handle) { 
  return_value_if_fail(handle != NULL, RET_BAD_PARAMS);

  if (handle->thread != NULL) {
    tk_thread_join(handle->thread);
    tk_thread_destroy(handle->thread);
  }

  close(handle->read_pfd[0]);
  if (handle->read_pfd[1] > 0) {
    close(handle->read_pfd[1]);
  }

  if (handle->write_pfd[0] > 0) {
    close(handle->write_pfd[0]);
  }
  close(handle->write_pfd[1]);
  str_reset(&handle->str_tmp);
  TKMEM_FREE(handle);
  return RET_OK;
}

process_handle_t process_create(const char* file_path, const char** args, uint32_t argc) {
  pid_t pid;
  uint32_t i = 0;
  process_handle_t handle = TKMEM_ZALLOC(process_info_t);
  return_value_if_fail(handle != NULL, NULL);

  str_init(&handle->str_tmp, CMD_LINE_SIZE);
  if (file_path != NULL) {
    str_append(&handle->str_tmp, file_path);
  }
  for (i = 0; i < argc; i++) {
    str_append_char(&handle->str_tmp, ' ');
    str_append(&handle->str_tmp, args[i]);
  }
  goto_error_if_fail(pipe(handle->read_pfd) >= 0);
  goto_error_if_fail(pipe(handle->write_pfd) >= 0);

  pid = fork();
  goto_error_if_fail(pid >= 0);
  if (pid == 0) { /* child */
    if (handle->read_pfd[1] != STDOUT_FILENO) {  
      goto_error_if_fail(dup2(handle->read_pfd[1], STDOUT_FILENO) >= 0);  
      close(handle->read_pfd[1]);
    }
    if (handle->write_pfd[0] != STDIN_FILENO) {  
      goto_error_if_fail(dup2(handle->write_pfd[0], STDIN_FILENO) >= 0);  
      close(handle->write_pfd[0]);
    }
    execl("/bin/sh", "sh", "-c", handle->str_tmp.str, NULL);
    _exit(127);  
  } else {
    handle->pid = pid;

    handle->thread = tk_thread_create(process_wait_for_on_thread, handle);
    goto_error_if_fail(handle->thread != NULL);
    goto_error_if_fail(tk_thread_start(handle->thread) == RET_OK);
  }
  return handle;
error :
  process_destroy(handle);
  return NULL;
}

int process_handle_get_fd(process_handle_t handle) {
  return_value_if_fail(handle != NULL, -1);
  if (handle->read_pfd[1] == -1) {
    return -1;
  }
  return handle->read_pfd[0];
}

ret_t process_wait_for_data(process_handle_t handle, uint32_t timeout_ms) {
  int fd = process_handle_get_fd(handle);
  return_value_if_fail(fd > 0, RET_FAIL);
  return tk_socket_wait_for_data(fd, timeout_ms);
}

int32_t process_read(process_handle_t handle, uint8_t* buff, uint32_t max_size) {
  int32_t size = 0;
  int fd = process_handle_get_fd(handle);
  return_value_if_fail(handle != NULL && buff != NULL && fd > 0, -1);
  size = read(fd, buff, max_size);
  process_close_read_pipe(handle);
  return size;
}

int32_t process_write(process_handle_t handle, const uint8_t* buff, uint32_t max_size) {
  return_value_if_fail(handle != NULL && buff != NULL, -1);
  return_value_if_fail(handle->write_pfd[0] > 0, -1);
  return write(handle->write_pfd[1], buff, max_size);
}

bool_t process_is_broken(process_handle_t handle) {
  int stat = 0;
  return_value_if_fail(handle != NULL, TRUE);
  return waitpid(handle->pid, &stat, WNOHANG) != 0;
}

#endif


#endif
