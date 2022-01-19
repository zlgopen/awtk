/**
 * File:   debugger_server.h
 * Author: AWTK Develop Team
 * Brief:  debugger server
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/thread.h"
#include "tkc/buffer.h"
#include "tkc/object_default.h"
#include "ubjson/ubjson_writer.h"
#include "debugger/debugger_factory.h"
#include "debugger/debugger_server.h"
#include "debugger/debugger_message.h"

typedef struct _debugger_server_t {
  darray_t debuggers;
  debugger_t* debugger;
  tk_thread_t* thread;

  /*等待退出*/
  bool_t quiting;
  /*退出完成*/
  bool_t quited;
  /*启动完成*/
  bool_t started;

  tk_istream_t* in;
  tk_ostream_t* out;
  tk_iostream_t* io;
  tk_mutex_nest_t* mutex;

  /*读取数据的缓冲区*/
  void* buff;
  uint32_t capacity;
} debugger_server_t;

static void* debugger_server_run(void* ctx);
static ret_t debugger_server_send_object(debugger_server_t* server, debugger_resp_t* resp,
                                         tk_object_t* obj);

static debugger_server_t* debugger_server_create(tk_iostream_t* io) {
  debugger_server_t* server = TKMEM_ZALLOC(debugger_server_t);
  return_value_if_fail(server != NULL, NULL);

  assert(io != NULL);

  server->io = io;
  server->in = tk_iostream_get_istream(io);
  server->out = tk_iostream_get_ostream(io);
  darray_init(&(server->debuggers), 5, (tk_destroy_t)tk_object_unref, NULL);

  server->mutex = tk_mutex_nest_create();
  goto_error_if_fail(server->mutex != NULL);

  server->thread = tk_thread_create(debugger_server_run, server);
  goto_error_if_fail(server->thread != NULL);
  tk_thread_start(server->thread);

  while (!(server->started)) {
    sleep_ms(100);
  }
  log_debug("debugger_server_thread started\n");

  return server;

error:
  TK_OBJECT_UNREF(io);
  darray_deinit(&(server->debuggers));
  TKMEM_FREE(server);

  return NULL;
}

static ret_t debugger_server_extend(debugger_server_t* server, uint32_t size) {
  ret_t ret = RET_OK;
  return_value_if_fail(server != NULL, RET_BAD_PARAMS);

  if (server->capacity < size) {
    void* buff = TKMEM_REALLOC(server->buff, size);
    if (buff != NULL) {
      server->buff = buff;
      server->capacity = size;
    } else {
      ret = RET_FAIL;
    }
  }

  return ret;
}

static ret_t debugger_server_write_data(tk_ostream_t* out, const void* data, uint32_t size) {
  return tk_ostream_write_len(out, data, size, DEBUGGER_IO_WRITE_TIMEOUT) == size ? RET_OK : RET_IO;
}

static ret_t debugger_server_read_data(tk_istream_t* in, void* data, uint32_t size) {
  return tk_istream_read_len(in, data, size, DEBUGGER_IO_READ_TIMEOUT) == size ? RET_OK : RET_IO;
}

static ret_t debugger_server_send_data_impl(tk_ostream_t* out, debugger_resp_t* resp,
                                            binary_data_t* data) {
  ret_t ret = RET_FAIL;
  resp->version = DEBUGGER_VERSION;
  resp->size = data != NULL ? data->size : 0;

  ret = debugger_server_write_data(out, resp, sizeof(*resp));
  return_value_if_fail(ret == RET_OK, RET_IO);

  if (resp->size > 0) {
    ret = debugger_server_write_data(out, data->data, data->size);
  }

  return ret;
}

static ret_t debugger_server_send_data(debugger_server_t* server, debugger_resp_t* resp,
                                       binary_data_t* data) {
  ret_t ret = RET_FAIL;

  if (tk_mutex_nest_lock(server->mutex) == RET_OK) {
    ret = debugger_server_send_data_impl(server->out, resp, data);
    tk_mutex_nest_unlock(server->mutex);
  }

  return ret;
}

static ret_t debugger_server_send_object(debugger_server_t* server, debugger_resp_t* resp,
                                         tk_object_t* obj) {
  wbuffer_t wb;
  ret_t ret = RET_FAIL;
  ubjson_writer_t writer;
  binary_data_t data = {0, NULL};
  return_value_if_fail(wbuffer_init_extendable(&wb) != NULL, RET_OOM);

  if (obj != NULL) {
    ubjson_writer_init(&writer, (ubjson_write_callback_t)wbuffer_write_binary, &wb);
    ret = ubjson_writer_write_object(&writer, obj);
    goto_error_if_fail(ret == RET_OK);
  } else {
    wbuffer_write_string(&wb, "{}"); 
  }

  data.data = wb.data;
  data.size = wb.cursor;
  ret = debugger_server_send_data(server, resp, &data);

error:
  wbuffer_deinit(&wb);

  return ret;
}

static ret_t debugger_server_read_request_impl(debugger_server_t* server, debugger_req_t* req) {
  tk_istream_t* in = server->in;
  ret_t ret = debugger_server_read_data(in, req, sizeof(*req));
  return_value_if_fail(ret == RET_OK, RET_IO);

  assert(req->version == DEBUGGER_VERSION);
  return_value_if_fail(debugger_server_extend(server, req->size) == RET_OK, RET_OOM);

  if (req->size > 0) {
    ret = debugger_server_read_data(in, server->buff, req->size);
  } else {
    ret = RET_OK;
  }

  return ret;
}

static ret_t debugger_server_read_request(debugger_server_t* server, debugger_req_t* req) {
  ret_t ret = tk_istream_wait_for_data(server->in, 500);

  if (ret != RET_OK) {
    return ret;
  }

  if (tk_mutex_nest_lock(server->mutex) == RET_OK) {
    ret = debugger_server_read_request_impl(server, req);
    tk_mutex_nest_unlock(server->mutex);
  }

  return ret;
}

static debugger_t* debugger_server_find(debugger_server_t* server, const char* code_id) {
  uint32_t i = 0;
  debugger_t* debugger = NULL;
  return_value_if_fail(code_id != NULL, NULL);
  return_value_if_fail(server != NULL, NULL);

  if (tk_mutex_nest_lock(server->mutex) == RET_OK) {
    for (i = 0; i < server->debuggers.size; i++) {
      debugger_t* iter = (debugger_t*)darray_get(&(server->debuggers), i);
      if (debugger_match(iter, code_id)) {
        debugger = iter;
        break;
      }
    }
    tk_mutex_nest_unlock(server->mutex);
  }

  return debugger;
}

static ret_t debugger_server_on_events(void* ctx, event_t* e) {
  ret_t ret = RET_OK;
  debugger_resp_t msg;
  tk_object_t* obj = NULL;
  tk_ostream_t* out = NULL;
  debugger_t* debugger = DEBUGGER(e->target);
  debugger_server_t* server = (debugger_server_t*)ctx;
  return_value_if_fail(debugger != NULL && server != NULL, RET_BAD_PARAMS);

  out = server->out;
  return_value_if_fail(out != NULL, RET_BAD_PARAMS);

  obj = object_default_create();
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  memset(&msg, 0x00, sizeof(msg));
  msg.code = e->type;
  msg.error = RET_OK;

  switch (e->type) {
    case DEBUGGER_RESP_MSG_LOG: {
      debugger_log_event_t* event = debugger_log_event_cast(e);
      tk_object_set_prop_int(obj, STR_DEBUGGER_EVENT_PROP_LINE, event->line);
      tk_object_set_prop_str(obj, STR_DEBUGGER_EVENT_PROP_MESSAGE, event->message);
      ret = debugger_server_send_object(server, &msg, obj);
      break;
    }
    case DEBUGGER_RESP_MSG_ERROR: {
      debugger_error_event_t* event = debugger_error_event_cast(e);
      tk_object_set_prop_int(obj, STR_DEBUGGER_EVENT_PROP_LINE, event->line);
      tk_object_set_prop_str(obj, STR_DEBUGGER_EVENT_PROP_MESSAGE, event->message);
      ret = debugger_server_send_object(server, &msg, obj);
      break;
    }
    case DEBUGGER_RESP_MSG_BREAKED: {
      debugger_breaked_event_t* event = debugger_breaked_event_cast(e);
      tk_object_set_prop_int(obj, STR_DEBUGGER_EVENT_PROP_LINE, event->line);
      ret = debugger_server_send_object(server, &msg, obj);
      break;
    }
    case DEBUGGER_RESP_MSG_COMPLETED: {
      debugger_server_send_data(server, &msg, NULL);
      break;
    }
    default: {
      log_debug("not supported event:%d\n", (int)(e->type));
      break;
    }
  }

  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t debugger_server_init_debugger(debugger_server_t* server, const char* arg) {
  ret_t ret = RET_FAIL;
  char lang[TK_NAME_LEN + 1];
  const char* code_id = NULL;
  return_value_if_fail(server != NULL, RET_BAD_PARAMS);
  code_id = strchr(arg, ':');
  return_value_if_fail(code_id != NULL, RET_BAD_PARAMS);

  tk_strncpy_s(lang, TK_NAME_LEN, arg, code_id - arg);
  code_id++;
  if (tk_mutex_nest_lock(server->mutex) == RET_OK) {
    debugger_t* debugger = debugger_server_find(server, code_id);
    if (debugger == NULL) {
      debugger = debugger_factory_create_debugger(lang, code_id);
      if (debugger != NULL) {
        ret = darray_push(&(server->debuggers), debugger);
        if (ret != RET_OK) {
          OBJECT_UNREF(debugger);
        } else {
          emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_LOG, debugger_server_on_events, server);
          emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_ERROR, debugger_server_on_events, server);
          emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_BREAKED, debugger_server_on_events,
                     server);
          emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED, debugger_server_on_events,
                     server);
        }
      } else {
        log_debug("create debugger:%s %s failed\n", lang, code_id);
      }
    }

    if (ret == RET_OK) {
      server->debugger = debugger;
    }
    tk_mutex_nest_unlock(server->mutex);
  }

  return ret;
}

static ret_t debugger_server_destroy(debugger_server_t* server) {
  return_value_if_fail(server != NULL, RET_BAD_PARAMS);

  server->quiting = TRUE;
  while (!server->quited) {
    sleep_ms(100);
  }

  tk_mutex_nest_destroy(server->mutex);
  server->mutex = NULL;

  darray_deinit(&(server->debuggers));
  tk_thread_destroy(server->thread);
  TK_OBJECT_UNREF(server->io);
  TKMEM_FREE(server->buff);
  TKMEM_FREE(server);

  return RET_OK;
}

static ret_t debugger_server_dispatch(debugger_server_t* server) {
  ret_t ret = RET_OK;
  debugger_req_t req;
  debugger_resp_t resp;
  debugger_t* debugger = NULL;

  server->started = TRUE;
  while (!(server->quiting)) {
    ret = debugger_server_read_request(server, &req);
    if (ret == RET_TIMEOUT) {
      continue;
    } else if (ret != RET_OK) {
      break;
    }
    resp.size = 0;
    resp.code = req.code;
    resp.error = RET_OK;

    if (req.code == DEBUGGER_REQ_INIT) {
      resp.error = debugger_server_init_debugger(server, (const char*)(server->buff));
      break_if_fail(debugger_server_send_data(server, &resp, NULL) == RET_OK);
      continue;
    }

    if (server->debugger == NULL) {
      resp.error = RET_NOT_FOUND;
      break_if_fail(debugger_server_send_data(server, &resp, NULL) == RET_OK);
      continue;
    }

    debugger = server->debugger;

    switch (req.code) {
      case DEBUGGER_REQ_STOP: {
        resp.error = debugger_stop(debugger);
        break;
      }
      case DEBUGGER_REQ_PAUSE: {
        resp.error = debugger_pause(debugger);
        break;
      }
      case DEBUGGER_REQ_IS_PAUSED: {
        resp.error = debugger_is_paused(debugger) ? RET_OK : RET_FAIL;
        break;
      }
      case DEBUGGER_REQ_NEXT: {
        resp.error = debugger_next(debugger);
        break;
      }
      case DEBUGGER_REQ_STEP_IN: {
        resp.error = debugger_step_in(debugger);
        break;
      }
      case DEBUGGER_REQ_STEP_OUT: {
        resp.error = debugger_step_out(debugger);
        break;
      }
      case DEBUGGER_REQ_STEP_OVER: {
        resp.error = debugger_step_over(debugger);
        break;
      }
      case DEBUGGER_REQ_CONTINUE: {
        resp.error = debugger_continue(debugger);
        break;
      }
      case DEBUGGER_REQ_SET_BREAK_POINT: {
        resp.error = debugger_set_break_point(debugger, req.data);
        break;
      }
      case DEBUGGER_REQ_REMOVE_BREAK_POINT: {
        resp.error = debugger_remove_break_point(debugger, req.data);
        break;
      }
      case DEBUGGER_REQ_CLEAR_BREAK_POINTS: {
        resp.error = debugger_clear_break_points(debugger);
        break;
      }
      case DEBUGGER_REQ_GET_SELF: {
        if (debugger_lock(debugger) == RET_OK) {
          tk_object_t* obj = debugger_get_self(debugger);
          ret = debugger_server_send_object(server, &resp, obj);
          TK_OBJECT_UNREF(obj);
          debugger_unlock(debugger);
          goto_error_if_fail(ret == RET_OK);
          continue;
        }
        break;
      }
      case DEBUGGER_REQ_GET_LOCAL: {
        if (debugger_lock(debugger) == RET_OK) {
          tk_object_t* obj = debugger_get_local(debugger, req.data);
          ret = debugger_server_send_object(server, &resp, obj);
          TK_OBJECT_UNREF(obj);
          debugger_unlock(debugger);
          goto_error_if_fail(ret == RET_OK);
          continue;
        }
        break;
      }
      case DEBUGGER_REQ_GET_GLOBAL: {
        if (debugger_lock(debugger) == RET_OK) {
          tk_object_t* obj = debugger_get_global(debugger);
          ret = debugger_server_send_object(server, &resp, obj);
          TK_OBJECT_UNREF(obj);
          debugger_unlock(debugger);
          goto_error_if_fail(ret == RET_OK);
          continue;
        }
        break;
      }
      case DEBUGGER_REQ_GET_CODE: {
        binary_data_t data = {0, NULL};
        if (debugger_lock(debugger) == RET_OK) {
          resp.error = debugger_get_code(debugger, &data);
          ret = debugger_server_send_data(server, &resp, &data);
          debugger_unlock(debugger);
          goto_error_if_fail(ret == RET_OK);
        }
        continue;
      }
      case DEBUGGER_REQ_GET_CALLSTACK: {
        binary_data_t data = {0, NULL};
        if (debugger_lock(debugger) == RET_OK) {
          resp.error = debugger_get_callstack(debugger, &data);
          ret = debugger_server_send_data(server, &resp, &data);
          debugger_unlock(debugger);
          goto_error_if_fail(ret == RET_OK);
        }
        continue;
      }
      case DEBUGGER_REQ_UPDATE_CODE: {
        binary_data_t code = {req.size, server->buff};
        resp.error = debugger_update_code(debugger, &code);
        break;
      }
      case DEBUGGER_REQ_DEINIT: {
        resp.error = debugger_deinit(debugger);
        break;
      }
      default: {
        log_debug("not supported code:%u\n", req.code);
        resp.error = RET_NOT_IMPL;
        break;
      }
    }

    break_if_fail(debugger_server_send_data(server, &resp, NULL) == RET_OK);

    if (req.code == DEBUGGER_REQ_DEINIT) {
      log_debug("quit because deinit\n");
      break;
    }
  }

error:
  server->quited = TRUE;

  return RET_OK;
}

static void* debugger_server_run(void* ctx) {
  debugger_server_dispatch((debugger_server_t*)ctx);
  return NULL;
}

static debugger_server_t* s_debugger_server = NULL;

ret_t debugger_server_start(tk_iostream_t* io) {
  debugger_server_t* server = NULL;
  return_value_if_fail(io != NULL && s_debugger_server == NULL, RET_BAD_PARAMS);

  server = debugger_server_create(io);
  return_value_if_fail(server != NULL, RET_BAD_PARAMS);

  s_debugger_server = server;

  return RET_OK;
}

ret_t debugger_server_stop(void) {
  return_value_if_fail(s_debugger_server != NULL, RET_BAD_PARAMS);

  debugger_server_destroy(s_debugger_server);
  s_debugger_server = NULL;

  return RET_OK;
}

debugger_t* debugger_server_find_debugger(const char* code_id) {
  debugger_server_t* server = s_debugger_server;
  return_value_if_fail(code_id != NULL, NULL);

  return debugger_server_find(server, code_id);
}

bool_t debugger_server_is_running(void) {
  return s_debugger_server != NULL;
}
