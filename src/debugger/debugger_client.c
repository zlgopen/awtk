/**
 * File:   debugger.c
 * Author: AWTK Develop Team
 * Brief:  debugger
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
 * 2022-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/buffer.h"
#include "ubjson/ubjson_parser.h"
#include "debugger/debugger_message.h"
#include "debugger/debugger_client.h"

static ret_t debugger_client_lock(debugger_t* debugger) {
  return RET_OK;
}

static ret_t debugger_client_unlock(debugger_t* debugger) {
  return RET_OK;
}

static ret_t debugger_client_write_data(tk_ostream_t* out, const void* data, uint32_t size) {
  return tk_ostream_write_len(out, data, size, DEBUGGER_IO_WRITE_TIMEOUT) == size ? RET_OK : RET_IO;
}

static ret_t debugger_client_read_data(tk_istream_t* in, void* data, uint32_t size) {
  return tk_istream_read_len(in, data, size, DEBUGGER_IO_READ_TIMEOUT) == size ? RET_OK : RET_IO;
}

static ret_t debugger_client_extend_buff(debugger_t* debugger, uint32_t size) {
  ret_t ret = RET_OK;
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);

  if (client->capacity < size) {
    void* buff = TKMEM_REALLOC(client->buff, size);
    if (buff != NULL) {
      client->buff = buff;
      client->capacity = size;
    } else {
      ret = RET_OOM;
    }
  }
  return ret;
}

static ret_t debugger_client_dispatch_message(debugger_t* debugger, debugger_resp_t* resp) {
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);

  switch (resp->code) {
    case DEBUGGER_RESP_MSG_BREAKED: {
      uint32_t line = 0;
      debugger_breaked_event_t event;
      tk_object_t* obj = ubjson_to_object(client->buff, resp->size);
      return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
      line = tk_object_get_prop_int(obj, STR_DEBUGGER_EVENT_PROP_LINE, 0);
      emitter_dispatch(EMITTER(debugger), debugger_breaked_event_init(&event, line));
      TK_OBJECT_UNREF(obj);
      break;
    }
    case DEBUGGER_RESP_MSG_LOG: {
      uint32_t line = 0;
      const char* message = NULL;
      debugger_log_event_t event;
      tk_object_t* obj = ubjson_to_object(client->buff, resp->size);
      return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
      line = tk_object_get_prop_int(obj, STR_DEBUGGER_EVENT_PROP_LINE, 0);
      message = tk_object_get_prop_str(obj, STR_DEBUGGER_EVENT_PROP_MESSAGE);
      emitter_dispatch(EMITTER(debugger), debugger_log_event_init(&event, line, message));
      TK_OBJECT_UNREF(obj);
      break;
    }
    case DEBUGGER_RESP_MSG_ERROR: {
      uint32_t line = 0;
      const char* message = NULL;
      debugger_error_event_t event;
      tk_object_t* obj = ubjson_to_object(client->buff, resp->size);
      return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
      line = tk_object_get_prop_int(obj, STR_DEBUGGER_EVENT_PROP_LINE, 0);
      message = tk_object_get_prop_str(obj, STR_DEBUGGER_EVENT_PROP_MESSAGE);
      emitter_dispatch(EMITTER(debugger), debugger_error_event_init(&event, line, message));
      TK_OBJECT_UNREF(obj);
      break;
    }
    case DEBUGGER_RESP_MSG_COMPLETED: {
      client->program_completed = TRUE;
      emitter_dispatch_simple_event(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED);
      break;
    }
    default: {
      break;
    }
  }

  return RET_OK;
}

static ret_t debugger_client_dispatch_one(debugger_t* debugger, debugger_resp_t* resp) {
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);
  tk_istream_t* in = tk_iostream_get_istream(client->io);

  return_value_if_fail(in != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger_client_read_data(in, resp, sizeof(*resp)) == RET_OK, RET_IO);
  assert(DEBUGGER_VERSION == resp->version);
  return_value_if_fail(debugger_client_extend_buff(debugger, resp->size) == RET_OK, RET_OOM);

  if (resp->size > 0) {
    return_value_if_fail(debugger_client_read_data(in, client->buff, resp->size) == RET_OK, RET_IO);
  }

  return debugger_client_dispatch_message(debugger, resp);
}

ret_t debugger_client_dispatch(debugger_t* debugger) {
  debugger_resp_t resp;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  memset(&resp, 0x00, sizeof(resp));

  return debugger_client_dispatch_one(debugger, &resp);
}

ret_t debugger_client_wait_for_completed(debugger_t* debugger) {
  debugger_resp_t resp;
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);

  memset(&resp, 0x00, sizeof(resp));
  while (!(client->program_completed)) {
    break_if_fail(debugger_client_dispatch_one(debugger, &resp) == RET_OK);
    if (resp.code == DEBUGGER_RESP_MSG_COMPLETED) {
      break;
    }
  }

  return RET_OK;
}

static ret_t debugger_client_read_packet(debugger_t* debugger, uint32_t resp_code, void** data,
                                         uint32_t* size, ret_t* ret) {
  debugger_resp_t resp;
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);

  memset(&resp, 0x00, sizeof(resp));
  while (TRUE) {
    break_if_fail(debugger_client_dispatch_one(debugger, &resp) == RET_OK);
    if (resp.code == resp_code) {
      *data = client->buff;
      *size = resp.size;
      *ret = resp.error;
      break;
    }
  }

  return RET_OK;
}

static ret_t debugger_client_read_simple(debugger_t* debugger, uint32_t resp_code) {
  void* data = NULL;
  uint32_t size = 0;
  ret_t ret = RET_FAIL;

  debugger_client_read_packet(debugger, resp_code, &data, &size, &ret);

  return ret;
}

static tk_object_t* debugger_client_read_object(debugger_t* debugger, uint32_t resp_code) {
  void* data = NULL;
  uint32_t size = 0;
  ret_t ret = RET_FAIL;

  if (debugger_client_read_packet(debugger, resp_code, &data, &size, &ret) == RET_OK &&
      ret == RET_OK) {
    assert(data != NULL && size > 0);
    return ubjson_to_object(data, size);
  }

  return NULL;
}

static ret_t debugger_client_read_binary(debugger_t* debugger, uint32_t resp_code,
                                         binary_data_t* data) {
  ret_t ret = RET_FAIL;

  debugger_client_read_packet(debugger, resp_code, &(data->data), &(data->size), &ret);

  return ret;
}

static ret_t debugger_client_write_binary(debugger_t* debugger, uint32_t code, const void* data,
                                          uint32_t size) {
  debugger_req_t req;
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);
  tk_ostream_t* out = tk_iostream_get_ostream(client->io);
  memset(&req, 0x00, sizeof(req));
  req.code = code;
  req.size = size;
  req.version = DEBUGGER_VERSION;
  debugger_client_write_data(out, &req, sizeof(req));

  if (req.size > 0) {
    return debugger_client_write_data(out, data, size);
  } else {
    return RET_OK;
  }
}

static ret_t debugger_client_write_simple(debugger_t* debugger, uint32_t code, uint32_t data) {
  debugger_req_t req;
  debugger_client_t* client = DEBUGGER_CLIENT(debugger);
  tk_ostream_t* out = tk_iostream_get_ostream(client->io);

  memset(&req, 0x00, sizeof(req));

  req.code = code;
  req.data = data;
  req.size = 0;
  req.version = DEBUGGER_VERSION;

  return debugger_client_write_data(out, &req, sizeof(req));
}

static ret_t debugger_client_request_simple(debugger_t* debugger, uint32_t code, uint32_t data) {
  return_value_if_fail(debugger_client_write_simple(debugger, code, data) == RET_OK, RET_FAIL);

  return debugger_client_read_simple(debugger, code);
}

static ret_t debugger_client_request_binary(debugger_t* debugger, uint32_t code, const void* data,
                                            uint32_t size) {
  return_value_if_fail(debugger_client_write_binary(debugger, code, data, size) == RET_OK,
                       RET_FAIL);

  return debugger_client_read_simple(debugger, code);
}

static ret_t debugger_client_stop(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_STOP, 0);
}

static ret_t debugger_client_pause(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_PAUSE, 0);
}

static bool_t debugger_client_is_paused(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_IS_PAUSED, 0) == RET_OK;
}

static ret_t debugger_client_next(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_NEXT, 0);
}

static ret_t debugger_client_step_in(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_STEP_IN, 0);
}

static ret_t debugger_client_step_out(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_STEP_OUT, 0);
}

static ret_t debugger_client_step_over(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_STEP_OVER, 0);
}

static ret_t debugger_client_continue(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_CONTINUE, 0);
}

static tk_object_t* debugger_client_get_local(debugger_t* debugger, uint32_t frame_index) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_LOCAL, frame_index) == RET_OK) {
    return debugger_client_read_object(debugger, DEBUGGER_RESP_GET_LOCAL);
  } else {
    return NULL;
  }
}

static tk_object_t* debugger_client_get_self(debugger_t* debugger) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_SELF, 0) == RET_OK) {
    return debugger_client_read_object(debugger, DEBUGGER_RESP_GET_SELF);
  } else {
    return NULL;
  }
}

static tk_object_t* debugger_client_get_global(debugger_t* debugger) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_GLOBAL, 0) == RET_OK) {
    return debugger_client_read_object(debugger, DEBUGGER_RESP_GET_GLOBAL);
  } else {
    return NULL;
  }
}

static ret_t debugger_client_get_callstack(debugger_t* debugger, binary_data_t* callstack) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_CALLSTACK, 0) == RET_OK) {
    return debugger_client_read_binary(debugger, DEBUGGER_RESP_GET_CALLSTACK, callstack);
  } else {
    return RET_FAIL;
  }
}

static ret_t debugger_client_clear_break_points(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_CLEAR_BREAK_POINTS, 0);
}

static ret_t debugger_client_set_break_point(debugger_t* debugger, uint32_t line) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_SET_BREAK_POINT, line);
}

static ret_t debugger_client_remove_break_point(debugger_t* debugger, uint32_t line) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_REMOVE_BREAK_POINT, line);
}

static ret_t debugger_client_attach(debugger_t* debugger, const char* lang, const char* code_id) {
  char data[256];
  tk_snprintf(data, sizeof(data) - 1, "%s:%s", lang, code_id);
  return debugger_client_request_binary(debugger, DEBUGGER_REQ_ATTACH, data, strlen(data) + 1);
}

static ret_t debugger_client_deinit(debugger_t* debugger) {
  return debugger_client_request_simple(debugger, DEBUGGER_REQ_DEINIT, 0);
}

static ret_t debugger_client_update_code(debugger_t* debugger, const binary_data_t* code) {
  return debugger_client_request_binary(debugger, DEBUGGER_REQ_UPDATE_CODE, code->data, code->size);
}

static ret_t debugger_client_launch(debugger_t* debugger, const char* lang,
                                    const binary_data_t* code) {
  wbuffer_t wb;
  ret_t ret = RET_FAIL;
  wbuffer_init_extendable(&wb);
  return_value_if_fail(wbuffer_extend_capacity(&wb, strlen(lang) + code->size + 1) == RET_OK,
                       RET_OOM);

  wbuffer_write_binary(&wb, lang, strlen(lang));
  wbuffer_write_binary(&wb, ":", 1);
  wbuffer_write_binary(&wb, code->data, code->size);

  ret = debugger_client_request_binary(debugger, DEBUGGER_REQ_LAUNCH, wb.data, wb.cursor);
  wbuffer_deinit(&wb);

  return ret;
}

static ret_t debugger_client_get_code(debugger_t* debugger, binary_data_t* code) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_CODE, 0) == RET_OK) {
    return debugger_client_read_binary(debugger, DEBUGGER_RESP_GET_CODE, code);
  } else {
    return RET_FAIL;
  }
}

static ret_t debugger_client_get_debuggers(debugger_t* debugger, binary_data_t* debuggers) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_DEBUGGERS, 0) == RET_OK) {
    return debugger_client_read_binary(debugger, DEBUGGER_RESP_GET_DEBUGGERS, debuggers);
  } else {
    return RET_FAIL;
  }
}

static ret_t debugger_client_get_break_points(debugger_t* debugger, binary_data_t* break_points) {
  if (debugger_client_write_simple(debugger, DEBUGGER_REQ_GET_BREAK_POINTS, 0) == RET_OK) {
    return debugger_client_read_binary(debugger, DEBUGGER_RESP_GET_BREAK_POINTS, break_points);
  } else {
    return RET_FAIL;
  }
}

static const debugger_vtable_t s_debugger_client_vtable = {
    .attach = debugger_client_attach,
    .launch = debugger_client_launch,
    .lang = "client",
    .lock = debugger_client_lock,
    .unlock = debugger_client_unlock,
    .stop = debugger_client_stop,
    .pause = debugger_client_pause,
    .is_paused = debugger_client_is_paused,
    .next = debugger_client_next,
    .step_in = debugger_client_step_in,
    .step_out = debugger_client_step_out,
    .step_over = debugger_client_step_over,
    .continve = debugger_client_continue,
    .get_local = debugger_client_get_local,
    .get_self = debugger_client_get_self,
    .get_global = debugger_client_get_global,
    .get_code = debugger_client_get_code,
    .get_debuggers = debugger_client_get_debuggers,
    .get_break_points = debugger_client_get_break_points,
    .get_callstack = debugger_client_get_callstack,
    .update_code = debugger_client_update_code,
    .set_break_point = debugger_client_set_break_point,
    .remove_break_point = debugger_client_remove_break_point,
    .clear_break_points = debugger_client_clear_break_points,
    .deinit = debugger_client_deinit,
};

debugger_client_t* debugger_client_cast(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt == &s_debugger_client_vtable, NULL);

  return (debugger_client_t*)debugger;
}

static ret_t debugger_client_on_destroy(tk_object_t* obj) {
  debugger_client_t* debugger = DEBUGGER_CLIENT(obj);
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(debugger->buff);
  TK_OBJECT_UNREF(debugger->io);

  return RET_OK;
}

static const object_vtable_t s_object_debugger_client_vtable = {
    .type = "object_debugger_client",
    .desc = "object_debugger_client",
    .size = sizeof(debugger_client_t),
    .is_collection = FALSE,
    .on_destroy = debugger_client_on_destroy};

debugger_t* debugger_client_create(tk_iostream_t* io) {
  debugger_client_t* debugger = NULL;
  return_value_if_fail(io != NULL, NULL);
  debugger = (debugger_client_t*)tk_object_create(&s_object_debugger_client_vtable);
  return_value_if_fail(debugger != NULL, NULL);

  debugger->io = io;
  TK_OBJECT_REF(debugger->io);
  debugger->debugger.vt = &s_debugger_client_vtable;
  debugger->capacity = 10 * 1024;
  debugger->buff = TKMEM_ALLOC(debugger->capacity);

  return (debugger_t*)debugger;
}
