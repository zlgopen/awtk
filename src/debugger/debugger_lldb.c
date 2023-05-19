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
 * 2023-05-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define SHOW_PROTOCOL_MESSAGES 0

#include "tkc.h"
#include "debugger_lldb.h"
#include "conf_io/conf_json.h"
#include "streams/inet/iostream_tcp.h"

#define N_WRITE_TIMEOUT 5000
#define STR_CONTENT_LENGTH "Content-Length:"

#define CMD_NEXT "next"
#define CMD_PAUSE "pause"
#define CMD_ATTACH "attach"
#define CMD_LAUNCH "launch"
#define CMD_SOURCE "source"
#define CMD_SCOPES "scopes"
#define CMD_STEP_IN "stepIn"
#define CMD_THREADS "threads"
#define CMD_STEP_OUT "stepOut"
#define CMD_CONTINUE "continue"
#define CMD_VARIABLES "variables"
#define CMD_INITIALIZE "initialize"
#define CMD_DISCONNECT "disconnect"
#define CMD_STACK_TRACE "stackTrace"
#define CMD_SET_BREAK_POINTS "setBreakpoints"
#define CMD_CONFIGURATION_DONE "configurationDone"
#define CMD_SET_FUNCTION_BREAK_POINTS "setFunctionBreakpoints"

#define EVENT_EXITED "exited"
#define EVENT_OUTPUT "output"
#define EVENT_STOPPED "stopped"
#define EVENT_PROCESS "process"
#define EVENT_TERMINATED "terminated"
#define EVENT_INITIALIZED "initialized"

#define MSG_TYPE_EVENT "event"
#define MSG_TYPE_REQUEST "request"
#define MSG_TYPE_RESPONSE "response"

#define KEY_ID "id"
#define KEY_PID "pid"
#define KEY_PORT "port"
#define KEY_HOST "host"
#define KEY_CWD "cwd"
#define KEY_NAME "name"
#define KEY_ARGS "args"
#define KEY_LINE "line"
#define KEY_TYPE "type"
#define KEY_PATH "path"
#define KEY_START "start"
#define KEY_COUNT "count"
#define KEY_EVENT "event"
#define KEY_LEVELS "levels"
#define KEY_SOURCE "source"
#define KEY_PROGRAM "program"
#define KEY_SUCCESS "success"
#define KEY_COMMAND "command"
#define KEY_FRAME_ID "frameId"
#define KEY_THREAD_ID "threadId"
#define KEY_ARGUMENTS "arguments"
#define KEY_START_FRAME "startFrame"
#define KEY_BREAK_POINTS "breakpoints"
#define KEY_STOP_ON_ENTRY "stopOnEntry"
#define KEY_DEBUGGER_ROOT "debuggerRoot"
#define KEY_ATTACH_COMMANDS "attachCommands"
#define KEY_SOURCE_REFERENCE "sourceReference"
#define KEY_TERMINATE_DEBUGGEE "terminateDebuggee"
#define KEY_VARIABLES_REFERENCE "variablesReference"

#define VARREF_LOCALS (int64_t)1
#define VARREF_GLOBALS (int64_t)2
#define VARREF_REGS (int64_t)3

#define DEBUGGER_LLDB(d) (debugger_lldb_t*)(d)

static ret_t debugger_lldb_update_break_points(debugger_t* debugger);
static ret_t debugger_lldb_simple_command(debugger_t* debugger, const char* cmd);
static ret_t debugger_lldb_disconnect(debugger_t* debugger, bool_t terminate_debuggee);
static tk_object_t* debugger_lldb_get_callstack_impl(debugger_t* debugger, uint32_t start_frame,
                                                     uint32_t levels);

static ret_t debugger_lldb_lock(debugger_t* debugger) {
  return RET_NOT_IMPL;
}

static ret_t debugger_lldb_unlock(debugger_t* debugger) {
  return RET_NOT_IMPL;
}

static tk_object_t* debugger_lldb_get_callstack_obj(debugger_t* debugger) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);

  if (lldb->callstack == NULL) {
    lldb->callstack = debugger_lldb_get_callstack_impl(debugger, 0, 100);
  }

  return lldb->callstack;
}

static const char* debugger_lldb_get_source_path(debugger_t* debugger, uint32_t frame_index) {
  char path[MAX_PATH + 1] = {0};
  tk_object_t* callstack = debugger_lldb_get_callstack_obj(debugger);
  return_value_if_fail(callstack != NULL, NULL);

  tk_snprintf(path, sizeof(path), "body.stackFrames.[%u].source.path", frame_index);
  return tk_object_get_prop_str(callstack, path);
}

static int64_t debugger_lldb_get_source_reference(debugger_t* debugger, uint32_t frame_index) {
  char path[MAX_PATH + 1] = {0};
  tk_object_t* callstack = debugger_lldb_get_callstack_obj(debugger);
  return_value_if_fail(callstack != NULL, 0);

  tk_snprintf(path, sizeof(path), "body.stackFrames.[%u].source.sourceReference", frame_index);
  return tk_object_get_prop_int64(callstack, path, 0);
}

static int64_t debugger_lldb_get_source_line(debugger_t* debugger, uint32_t frame_index) {
  char path[MAX_PATH + 1] = {0};
  tk_object_t* callstack = debugger_lldb_get_callstack_obj(debugger);
  return_value_if_fail(callstack != NULL, 0);

  tk_snprintf(path, sizeof(path), "body.stackFrames.[%u].line", frame_index);
  return tk_object_get_prop_int64(callstack, path, 0);
}

static const char* debugger_lldb_get_frame_name(debugger_t* debugger, uint32_t frame_index) {
  char path[MAX_PATH + 1] = {0};
  tk_object_t* callstack = debugger_lldb_get_callstack_obj(debugger);
  return_value_if_fail(callstack != NULL, NULL);

  tk_snprintf(path, sizeof(path), "body.stackFrames.[%u].name", frame_index);
  return tk_object_get_prop_str(callstack, path);
}

static int64_t debugger_lldb_get_frame_id(debugger_t* debugger, uint32_t frame_index) {
  char path[MAX_PATH + 1] = {0};
  tk_object_t* callstack = debugger_lldb_get_callstack_obj(debugger);
  return_value_if_fail(callstack != NULL, 0);

  tk_snprintf(path, sizeof(path), "body.stackFrames.[%u].id", frame_index);
  return tk_object_get_prop_int64(callstack, path, 0);
}

static ret_t debugger_lldb_emit(debugger_t* debugger, tk_object_t* resp) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  const char* event = tk_object_get_prop_str(resp, KEY_EVENT);

  if (tk_str_eq(event, EVENT_STOPPED)) {
    int32_t line = 0;
    debugger_breaked_event_t event;
    TK_OBJECT_UNREF(lldb->callstack);

    lldb->stop_thread_id = tk_object_get_prop_int64(resp, "body.threadId", 0);
    lldb->callstack = debugger_lldb_get_callstack_impl(debugger, 0, 100);
    debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_PAUSED);
    debugger_set_current_frame(debugger, 0);
    /*LLDB 行号从1开始*/
    line = lldb->current_frame_line - 1;
    emitter_dispatch(EMITTER(debugger), debugger_breaked_event_init(&event, line));

    log_debug("threadId = %d stopped\n", (int)lldb->stop_thread_id);
  } else if (tk_str_eq(event, EVENT_OUTPUT)) {
    uint32_t line = 0;
    debugger_log_event_t event;
    const char* message = tk_object_get_prop_str(resp, "body.output");
    emitter_dispatch(EMITTER(debugger), debugger_log_event_init(&event, line, message));
  } else if (tk_str_eq(event, EVENT_EXITED)) {
    debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_TERMINATED);
    emitter_dispatch_simple_event(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED);
  } else if (tk_str_eq(event, EVENT_TERMINATED)) {
    debugger_lldb_disconnect(debugger, FALSE);
    debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_TERMINATED);
    emitter_dispatch_simple_event(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED);
  }

  log_debug("event:%s\n", event);
  TK_OBJECT_UNREF(resp);

  return RET_OK;
}

static ret_t debugger_lldb_write_req(debugger_t* debugger, tk_object_t* obj) {
  int32_t ret = 0;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  str_t* header = &(lldb->header);
  str_t* body = &(lldb->body);
  tk_ostream_t* out = tk_iostream_get_ostream(lldb->io);

  str_clear(body);
  str_clear(header);

  tk_object_to_json(obj, body, 2, 0, FALSE);
  str_format(header, 200, "%s %u\r\n\r\n", STR_CONTENT_LENGTH, body->size);
  ret = tk_ostream_write_len(out, header->str, header->size, N_WRITE_TIMEOUT);

  if (ret == header->size) {
#if SHOW_PROTOCOL_MESSAGES
    log_debug("send:%s\n", body->str);
#endif /*SHOW_PROTOCOL_MESSAGES*/
    ret = tk_ostream_write_len(out, body->str, body->size, N_WRITE_TIMEOUT);
  }

  return RET_OK;
}

typedef enum _header_state_t {
  STATE_KEY = 0,
  STATE_VALUE,
  STATE_END0,
  STATE_END1,
  STATE_END2,
  STATE_END3,
} header_state_t;

static int32_t debugger_lldb_read_content_length(debugger_t* debugger) {
  char c = 0;
  int32_t content_length = 0;
  header_state_t state = STATE_KEY;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  str_t* header = &(lldb->header);
  tk_istream_t* in = tk_iostream_get_istream(lldb->io);

  str_clear(header);
  while (tk_istream_read_len(in, &c, 1, N_WRITE_TIMEOUT) == 1) {
    str_append_char(header, c);
    switch (state) {
      case STATE_KEY: {
        if (c == ':') {
          state = STATE_VALUE;
          assert(tk_str_eq(header->str, STR_CONTENT_LENGTH));
          str_clear(header);
        }
        break;
      }
      case STATE_VALUE: {
        if (c == '\r') {
          str_trim(header, " \r\n");
          content_length = tk_atoi(header->str);
          state = STATE_END0;
        }
        break;
      }
      case STATE_END0: {
        assert(c == '\n');
        state = STATE_END1;
        break;
      }
      case STATE_END1: {
        assert(c == '\r');
        state = STATE_END2;
        break;
      }
      case STATE_END2: {
        assert(c == '\n');
        state = STATE_END3;
        break;
      }
      default:
        break;
    }

    if (state == STATE_END3) {
      break;
    }
  }

  return content_length;
}

static tk_object_t* debugger_lldb_read_resp(debugger_t* debugger) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  str_t* body = &(lldb->body);
  tk_istream_t* in = tk_iostream_get_istream(lldb->io);
  int32_t content_length = debugger_lldb_read_content_length(debugger);

  if (content_length > 0) {
    char url[MAX_PATH + 1] = {0};

    str_clear(body);
    goto_error_if_fail(str_extend(body, content_length + 1) == RET_OK);
    goto_error_if_fail(tk_istream_read_len(in, body->str, content_length, N_WRITE_TIMEOUT) ==
                       content_length);
    body->size = content_length;
    body->str[content_length] = '\0';
    data_reader_mem_build_url(body->str, body->size, url);

#if SHOW_PROTOCOL_MESSAGES
    log_debug("recv:%s\n", body->str);
#endif /*SHOW_PROTOCOL_MESSAGES*/

    return conf_json_load(url, FALSE);
  }

error:
  return NULL;
}

static tk_object_t* debugger_lldb_dispatch_until_get_resp(debugger_t* debugger, const char* cmd,
                                                          uint32_t timeout) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  tk_istream_t* in = tk_iostream_get_istream(lldb->io);

  while (tk_istream_wait_for_data(in, timeout) == RET_OK) {
    tk_object_t* resp = debugger_lldb_read_resp(debugger);
    const char* type = tk_object_get_prop_str(resp, KEY_TYPE);
    break_if_fail(resp != NULL && type != NULL);

    if (tk_str_eq(type, MSG_TYPE_EVENT)) {
      debugger_lldb_emit(debugger, resp);
    } else if (tk_str_eq(type, MSG_TYPE_RESPONSE)) {
      const char* command = tk_object_get_prop_str(resp, KEY_COMMAND);
      if (tk_str_eq(command, cmd)) {
        return resp;
      } else {
        /*嵌套调用时，上级的结果，先保存起来。*/
        tk_object_set_prop_object(lldb->resps, command, resp);
      }
    } else {
      assert(!"impossible");
      TK_OBJECT_UNREF(resp);
    }

    /*嵌套调用时，在下级调用中已经得到结果。*/
    resp = tk_object_get_prop_object(lldb->resps, cmd);
    if (resp != NULL) {
      return resp;
    }
  }

  return NULL;
}

static ret_t debugger_lldb_dispatch_until_get_resp_simple(debugger_t* debugger, const char* cmd,
                                                          uint32_t timeout) {
  tk_object_t* resp = debugger_lldb_dispatch_until_get_resp(debugger, cmd, timeout);

  if (resp != NULL) {
    bool_t success = tk_object_get_prop_bool(resp, KEY_SUCCESS, FALSE);
    log_debug("%s:%s\n", cmd, success ? "true" : "false");
    TK_OBJECT_UNREF(resp);
    return success ? RET_OK : RET_FAIL;
  }

  return RET_FAIL;
}

static ret_t debugger_lldb_dispatch_one(debugger_t* debugger, uint32_t timeout) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  tk_istream_t* in = tk_iostream_get_istream(lldb->io);

  if (tk_istream_wait_for_data(in, timeout) == RET_OK) {
    tk_object_t* resp = debugger_lldb_read_resp(debugger);
    if (resp != NULL) {
      const char* type = tk_object_get_prop_str(resp, KEY_TYPE);
      if (tk_str_eq(type, MSG_TYPE_EVENT)) {
        debugger_lldb_emit(debugger, resp);
      } else if (tk_str_eq(type, MSG_TYPE_RESPONSE)) {
        TK_OBJECT_UNREF(resp);
      } else {
        assert(!"impossible");
        TK_OBJECT_UNREF(resp);
      }
      return RET_OK;
    }
  }

  return RET_FAIL;
}

static tk_object_t* debugger_lldb_create_init_req(debugger_t* debugger) {
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, NULL);

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_INITIALIZE);

  return req;
}

static ret_t debugger_lldb_init(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);
  req = debugger_lldb_create_init_req(debugger);
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_NONE);
  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_INITIALIZE, 3000);
  }

  TK_OBJECT_UNREF(req);

  return ret;
}

static tk_object_t* debugger_lldb_create_disconnect_req(debugger_t* debugger,
                                                        bool_t terminate_debuggee) {
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;
  return_value_if_fail(debugger != NULL, NULL);

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_DISCONNECT);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);
  tk_object_set_prop_bool(arguments, KEY_TERMINATE_DEBUGGEE, terminate_debuggee);
  TK_OBJECT_UNREF(arguments);

  return req;
}

static ret_t debugger_lldb_disconnect(debugger_t* debugger, bool_t terminate_debuggee) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);
  req = debugger_lldb_create_disconnect_req(debugger, terminate_debuggee);
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_DISCONNECT, 3000);
  }

  TK_OBJECT_UNREF(req);

  return ret;
}

static tk_object_t* debugger_lldb_create_launch_req(debugger_t* debugger, const char* program,
                                                    const char* work_root, int argc, char* argv[]) {
  int i = 0;
  value_t v;
  tk_object_t* args = NULL;
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;
  return_value_if_fail(debugger != NULL, NULL);
  return_value_if_fail(program != NULL, NULL);

  req = object_default_create();
  goto_error_if_fail(req != NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_LAUNCH);

  arguments = object_default_create();
  goto_error_if_fail(arguments != NULL);
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);

  tk_object_set_prop_bool(arguments, KEY_STOP_ON_ENTRY, FALSE);
  tk_object_set_prop_str(arguments, KEY_DEBUGGER_ROOT, work_root);
  tk_object_set_prop_str(arguments, KEY_CWD, work_root);
  tk_object_set_prop_str(arguments, KEY_PROGRAM, program);

  args = object_array_create();
  tk_object_set_prop_object(arguments, KEY_ARGS, args);

  for (i = 0; i < argc; i++) {
    value_set_str(&v, argv[i]);
    object_array_push(args, &v);
  }
  TK_OBJECT_UNREF(args);
  TK_OBJECT_UNREF(arguments);

  return req;
error:
  TK_OBJECT_UNREF(req);
  return NULL;
}

static ret_t debugger_lldb_launch_app_impl(debugger_t* debugger, const char* program,
                                           const char* cwd, int argc, char* argv[]) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);
  req = debugger_lldb_create_launch_req(debugger, program, cwd, argc, argv);
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_LAUNCH, 60000);
    if (ret == RET_OK) {
      debugger_lldb_simple_command(debugger, CMD_CONFIGURATION_DONE);
      debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_RUNNING);
      debugger_lldb_update_break_points(debugger);
    }
  }

  TK_OBJECT_UNREF(req);

  return ret;
}

static tk_object_t* debugger_lldb_create_attach_req(debugger_t* debugger, const char* cmds,
                                                    int32_t pid) {
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;
  return_value_if_fail(debugger != NULL, NULL);
  return_value_if_fail(cmds != NULL, NULL);

  req = object_default_create();
  goto_error_if_fail(req != NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_ATTACH);

  arguments = object_default_create();
  goto_error_if_fail(arguments != NULL);
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);

  if (cmds != NULL) {
    value_t v;
    tk_object_t* attach_commands = object_array_create();

    /*执行attach_commands，实现attach. WASM走这条路径。*/
    object_array_push(attach_commands, value_set_str(&v, cmds));
    tk_object_set_prop_object(arguments, KEY_ATTACH_COMMANDS, attach_commands);
    TK_OBJECT_UNREF(attach_commands);
  } else {
    /*attach到指定的PID*/
    tk_object_set_prop_int(arguments, KEY_PID, pid);
  }

  return req;
error:
  TK_OBJECT_UNREF(req);
  return NULL;
}

static ret_t debugger_lldb_attach_impl(debugger_t* debugger, const char* cmds, int32_t pid) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);
  req = debugger_lldb_create_attach_req(debugger, cmds, pid);
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_ATTACH, 60000);
    if (ret == RET_OK) {
      debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_RUNNING);
      debugger_lldb_update_break_points(debugger);
      debugger_lldb_simple_command(debugger, CMD_CONFIGURATION_DONE);
    }
  }

  TK_OBJECT_UNREF(req);

  return ret;
}

static ret_t debugger_lldb_launch_app(debugger_t* debugger, const char* program, const char* cwd,
                                      int argc, char* argv[]) {
  if (tk_str_start_with(program, STR_SCHEMA_WASM)) {
    char cmd[MAX_PATH + 1] = {0};
    const char* p = program + strlen(STR_SCHEMA_WASM);
    return_value_if_fail(p != NULL, RET_BAD_PARAMS);
    tk_snprintf(cmd, sizeof(cmd) - 1, "process connect -p wasm connect://%s", p);
    return debugger_lldb_attach_impl(debugger, cmd, -1);
  } else if (tk_str_start_with(program, STR_SCHEMA_PID)) {
    const char* p = program + strlen(STR_SCHEMA_PID);
    return debugger_lldb_attach_impl(debugger, NULL, tk_atoi(p));
  } else {
    return debugger_lldb_launch_app_impl(debugger, program, cwd, argc, argv);
  }
}

static tk_object_t* debugger_lldb_create_set_break_points_req(debugger_t* debugger,
                                                              const char* path) {
  value_t v;
  uint32_t i = 0;
  tk_object_t* bp = NULL;
  tk_object_t* req = NULL;
  tk_object_t* source = NULL;
  tk_object_t* arguments = NULL;
  tk_object_t* break_points = NULL;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  object_array_t* lines =
      (object_array_t*)tk_object_get_prop_object(lldb->source_break_points, path);

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_SET_BREAK_POINTS);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);

  source = object_default_create();
  tk_object_set_prop_object(arguments, KEY_SOURCE, source);
  tk_object_set_prop_str(source, KEY_PATH, path);

  break_points = object_array_create();
  tk_object_set_prop_object(arguments, KEY_BREAK_POINTS, break_points);

  for (i = 0; i < lines->size; i++) {
    value_t* iter = lines->props + i;
    int32_t line = value_int(iter);

    bp = object_default_create();
    tk_object_set_prop_int(bp, KEY_LINE, line);
    object_array_push(break_points, value_set_object(&v, bp));
    TK_OBJECT_UNREF(bp);
  }

  TK_OBJECT_UNREF(source);
  TK_OBJECT_UNREF(arguments);
  TK_OBJECT_UNREF(break_points);

  return req;
}

static ret_t debugger_lldb_on_source_break_point(void* ctx, const void* data) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  debugger_t* debugger = (debugger_t*)ctx;
  named_value_t* nv = (named_value_t*)data;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  req = debugger_lldb_create_set_break_points_req(debugger, nv->name);
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_SET_BREAK_POINTS, 3000);
  }

  TK_OBJECT_UNREF(req);

  return ret;
}

static ret_t debugger_lldb_update_source_break_points(debugger_t* debugger) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  tk_object_foreach_prop(lldb->source_break_points, debugger_lldb_on_source_break_point, debugger);

  return RET_OK;
}

static tk_object_t* debugger_lldb_create_set_func_break_points_req(debugger_t* debugger) {
  value_t v;
  uint32_t i = 0;
  tk_object_t* bp = NULL;
  tk_object_t* req = NULL;
  tk_object_t* source = NULL;
  tk_object_t* arguments = NULL;
  tk_object_t* break_points = NULL;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  darray_t* arr = &(lldb->functions_break_points);
  return_value_if_fail(debugger != NULL, NULL);

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_SET_FUNCTION_BREAK_POINTS);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);

  break_points = object_array_create();
  tk_object_set_prop_object(arguments, KEY_BREAK_POINTS, break_points);

  for (i = 0; i < arr->size; i++) {
    const char* func = (const char*)darray_get(arr, i);

    bp = object_default_create();
    tk_object_set_prop_str(bp, KEY_NAME, func);
    object_array_push(break_points, value_set_object(&v, bp));
  }

  TK_OBJECT_UNREF(source);
  TK_OBJECT_UNREF(arguments);
  TK_OBJECT_UNREF(break_points);

  return req;
}

static ret_t debugger_lldb_update_func_break_points(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);
  req = debugger_lldb_create_set_func_break_points_req(debugger);
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret =
        debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_SET_FUNCTION_BREAK_POINTS, 3000);
  }

  TK_OBJECT_UNREF(req);

  return ret;
}

static tk_object_t* debugger_lldb_create_get_callstack_req(debugger_t* debugger,
                                                           uint32_t start_frame, uint32_t levels) {
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  int64_t thread_id = lldb->stop_thread_id;

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_STACK_TRACE);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);
  tk_object_set_prop_uint32(arguments, KEY_START_FRAME, start_frame);
  tk_object_set_prop_uint32(arguments, KEY_LEVELS, levels);
  tk_object_set_prop_int64(arguments, KEY_THREAD_ID, thread_id);

  TK_OBJECT_UNREF(arguments);

  return req;
}

static tk_object_t* debugger_lldb_get_callstack_impl(debugger_t* debugger, uint32_t start_frame,
                                                     uint32_t levels) {
  tk_object_t* req = NULL;
  tk_object_t* resp = NULL;
  return_value_if_fail(debugger != NULL, NULL);
  req = debugger_lldb_create_get_callstack_req(debugger, start_frame, levels);
  return_value_if_fail(req != NULL, NULL);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    resp = debugger_lldb_dispatch_until_get_resp(debugger, CMD_STACK_TRACE, 3000);
  }

  TK_OBJECT_UNREF(req);

  return resp;
}

static tk_object_t* debugger_lldb_create_get_variables_req(debugger_t* debugger, uint32_t type,
                                                           uint32_t start, uint32_t count) {
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_VARIABLES);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);
  tk_object_set_prop_uint32(arguments, KEY_VARIABLES_REFERENCE, type);
  tk_object_set_prop_uint32(arguments, KEY_START, start);
  tk_object_set_prop_int64(arguments, KEY_COUNT, count);

  TK_OBJECT_UNREF(arguments);

  return req;
}

static tk_object_t* debugger_lldb_get_variables_impl(debugger_t* debugger, uint32_t type,
                                                     uint32_t start, uint32_t count) {
  tk_object_t* req = NULL;
  tk_object_t* resp = NULL;
  return_value_if_fail(debugger != NULL, NULL);
  req = debugger_lldb_create_get_variables_req(debugger, type, start, count);
  return_value_if_fail(req != NULL, NULL);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    resp = debugger_lldb_dispatch_until_get_resp(debugger, CMD_VARIABLES, 3000);
  }

  TK_OBJECT_UNREF(req);

  return resp;
}

static tk_object_t* debugger_lldb_create_get_threads_req(debugger_t* debugger) {
  tk_object_t* req = NULL;
  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_THREADS);

  return req;
}

static tk_object_t* debugger_lldb_get_threads(debugger_t* debugger) {
  tk_object_t* req = NULL;
  tk_object_t* resp = NULL;
  return_value_if_fail(debugger != NULL, NULL);
  req = debugger_lldb_create_get_threads_req(debugger);
  return_value_if_fail(req != NULL, NULL);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    resp = debugger_lldb_dispatch_until_get_resp(debugger, CMD_THREADS, 3000);
  }

  TK_OBJECT_UNREF(req);

  return resp;
}

static tk_object_t* debugger_lldb_create_scopes_req(debugger_t* debugger, uint32_t frame_id) {
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, CMD_SCOPES);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);
  tk_object_set_prop_uint32(arguments, KEY_FRAME_ID, frame_id);

  TK_OBJECT_UNREF(arguments);

  return req;
}

static ret_t debugger_lldb_scopes_command(debugger_t* debugger, uint32_t frame_id) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, ret);
  req = debugger_lldb_create_scopes_req(debugger, frame_id);
  return_value_if_fail(req != NULL, ret);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, CMD_SCOPES, 3000);
  }

  return ret;
}

static tk_object_t* debugger_lldb_create_simple_req(debugger_t* debugger, const char* cmd) {
  tk_object_t* req = NULL;
  tk_object_t* arguments = NULL;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  int64_t thread_id = lldb->stop_thread_id;

  req = object_default_create();
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(req, KEY_TYPE, MSG_TYPE_REQUEST);
  tk_object_set_prop_str(req, KEY_COMMAND, cmd);

  arguments = object_default_create();
  tk_object_set_prop_object(req, KEY_ARGUMENTS, arguments);
  tk_object_set_prop_int64(arguments, KEY_THREAD_ID, thread_id);

  TK_OBJECT_UNREF(arguments);

  return req;
}

static ret_t debugger_lldb_simple_command(debugger_t* debugger, const char* cmd) {
  ret_t ret = RET_FAIL;
  tk_object_t* req = NULL;
  return_value_if_fail(debugger != NULL, ret);
  req = debugger_lldb_create_simple_req(debugger, cmd);
  return_value_if_fail(req != NULL, ret);

  if (debugger_lldb_write_req(debugger, req) == RET_OK) {
    ret = debugger_lldb_dispatch_until_get_resp_simple(debugger, cmd, 3000);
  }

  return ret;
}

ret_t debugger_lldb_dispatch_messages(debugger_t* debugger) {
  while (debugger_lldb_dispatch_one(debugger, 10) == RET_OK) {
    if (debugger_get_state(debugger) == DEBUGGER_PROGRAM_STATE_RUNNING) {
      break;
    }
  }

  return RET_OK;
}

ret_t debugger_lldb_wait_for_completed(debugger_t* debugger) {
  while (debugger_get_state(debugger) == DEBUGGER_PROGRAM_STATE_RUNNING) {
    if (debugger_lldb_dispatch_one(debugger, 100) != RET_OK) {
      break;
    }
  }

  return RET_OK;
}

static ret_t debugger_lldb_stop(debugger_t* debugger) {
  return debugger_lldb_disconnect(debugger, TRUE);
}

static ret_t debugger_lldb_pause(debugger_t* debugger) {
  return debugger_lldb_simple_command(debugger, CMD_PAUSE);
}

static ret_t debugger_lldb_restart(debugger_t* debugger) {
  return RET_NOT_IMPL;
}

static ret_t debugger_lldb_step_over(debugger_t* debugger) {
  return debugger_lldb_simple_command(debugger, CMD_NEXT);
}

static ret_t debugger_lldb_step_in(debugger_t* debugger) {
  return debugger_lldb_simple_command(debugger, CMD_STEP_IN);
}

static ret_t debugger_lldb_step_out(debugger_t* debugger) {
  return debugger_lldb_simple_command(debugger, CMD_STEP_OUT);
}

static ret_t debugger_lldb_step_loop_over(debugger_t* debugger) {
  return RET_NOT_IMPL;
}

static ret_t debugger_lldb_continue(debugger_t* debugger) {
  return debugger_lldb_simple_command(debugger, CMD_CONTINUE);
}

static ret_t debugger_lldb_load_source(debugger_t* debugger, const char* path) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);

  if (path != NULL) {
    char* source = (char*)tk_object_get_prop_str(lldb->sources, path);
    if (source == NULL) {
      uint32_t size = 0;
      source = (char*)file_read(path, &size);
      if (source != NULL) {
        tk_object_set_prop_str(lldb->sources, path, source);
        TKMEM_FREE(source);
      }
    } else {
      log_debug("read source failed:%s\n", path);
    }
    lldb->current_frame_source = tk_object_get_prop_str(lldb->sources, path);
  } else {
    lldb->current_frame_source = NULL;
  }

  return RET_OK;
}

static ret_t debugger_lldb_set_current_frame(debugger_t* debugger, uint32_t frame_index) {
  const char* path = NULL;
  debugger_frame_changed_event_t event;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);

  path = debugger_lldb_get_source_path(debugger, frame_index);
  debugger_lldb_load_source(debugger, path);

  lldb->current_frame_id = debugger_lldb_get_frame_id(debugger, frame_index);
  lldb->current_frame_name = debugger_lldb_get_frame_name(debugger, frame_index);
  lldb->current_frame_line = debugger_lldb_get_source_line(debugger, frame_index);

  debugger_lldb_scopes_command(debugger, lldb->current_frame_id);

  /*LLDB 行号从1开始*/
  debugger_frame_changed_event_init(&event, lldb->current_frame_name, lldb->current_frame_line - 1);
  emitter_dispatch(EMITTER(debugger), (event_t*)&(event));

  return RET_OK;
}

static int32_t object_find_variables_reference(tk_object_t* obj, const char* name) {
  uint32_t i = 0;
  char path[MAX_PATH + 1] = {0};
  uint32_t n = tk_object_get_prop_uint32(obj, "body.variables.#size", 0);

  for (i = 0; i < n; i++) {
    tk_snprintf(path, sizeof(path) - 1, "body.variables.[%d].name", i);
    if (tk_str_eq(name, tk_object_get_prop_str(obj, path))) {
      tk_snprintf(path, sizeof(path) - 1, "body.variables.[%d].variablesReference", i);
      return tk_object_get_prop_int32(obj, path, 0);
    }
  }

  return -1;
}

static tk_object_t* debugger_lldb_get_var(debugger_t* debugger, const char* path) {
  tk_object_t* obj = NULL;
  int64_t variables_reference = 0;
  return_value_if_fail(debugger != NULL, NULL);

  obj = debugger_lldb_get_variables_impl(debugger, VARREF_LOCALS, 0, 0xffff);
  return_value_if_fail(obj != NULL, NULL);

  if (TK_STR_IS_NOT_EMPTY(path)) {
    str_t str;
    tokenizer_t t;
    char name[MAX_PATH + 1] = {0};
    bool_t is_array = FALSE;

    str_init(&str, 100);
    str_set(&str, path);
    str_replace(&str, "->", ".");

    path = str.str;
    tokenizer_init_ex(&t, path, strlen(path), ".", ".[]");

    while (tokenizer_has_more(&t)) {
      const char* iter = tokenizer_next(&t);
      if (tk_str_eq(iter, "[")) {
        is_array = TRUE;
      } else if (tk_str_eq(iter, "]")) {
        is_array = FALSE;
      } else {
        if (is_array) {
          tk_snprintf(name, sizeof(name) - 1, "[%s]", iter);
          variables_reference = object_find_variables_reference(obj, name);
        } else {
          variables_reference = object_find_variables_reference(obj, iter);
        }
        TK_OBJECT_UNREF(obj);
        if (variables_reference > 0) {
          obj = debugger_lldb_get_variables_impl(debugger, variables_reference, 0, 0);
        }
      }
    }
    tokenizer_deinit(&t);
    str_reset(&str);
  }

  return obj;
}

static tk_object_t* debugger_lldb_get_local(debugger_t* debugger, uint32_t frame_index) {
  int64_t frame_id = debugger_lldb_get_frame_id(debugger, frame_index);
  debugger_lldb_scopes_command(debugger, frame_id);

  return debugger_lldb_get_variables_impl(debugger, VARREF_LOCALS, 0, 0xffff);
}

static tk_object_t* debugger_lldb_get_self(debugger_t* debugger) {
  return debugger_lldb_get_var(debugger, "this");
}

static tk_object_t* debugger_lldb_get_global(debugger_t* debugger) {
  return debugger_lldb_get_variables_impl(debugger, VARREF_LOCALS, 0, 0xffff);
}

static ret_t debugger_lldb_get_callstack(debugger_t* debugger, binary_data_t* ret) {
  int32_t i = 0;
  int32_t n = 0;
  char path[MAX_PATH + 1] = {0};
  tk_object_t* callstack = NULL;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  str_t* str = &(lldb->scallstack);
  return_value_if_fail(ret != NULL, RET_BAD_PARAMS);

  debugger_lldb_dispatch_messages(debugger);
  callstack = debugger_lldb_get_callstack_obj(debugger);
  return_value_if_fail(callstack != NULL, RET_BAD_PARAMS);

  str_clear(str);
  n = tk_object_get_prop_uint32(callstack, "body.stackFrames.#size", 0);
  for (i = 0; i < n; i++) {
    tk_snprintf(path, sizeof(path) - 1, "body.stackFrames.[%d].name", i);
    str_append(str, tk_object_get_prop_str(callstack, path));
    str_append(str, "\n");
  }

  ret->data = str->str;
  ret->size = str->size;

  return RET_OK;
}

static ret_t debugger_lldb_update_break_points(debugger_t* debugger) {
  if (debugger_is_paused_or_running(debugger)) {
    debugger_lldb_update_func_break_points(debugger);
    debugger_lldb_update_source_break_points(debugger);
  }

  return RET_OK;
}

static ret_t debugger_lldb_clear_break_points(debugger_t* debugger) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);

  darray_clear(&(lldb->functions_break_points));
  object_default_clear_props(lldb->source_break_points);
  debugger_lldb_update_break_points(debugger);

  return RET_FAIL;
}

static ret_t debugger_lldb_set_break_point_ex(debugger_t* debugger, const char* position) {
  char* p = NULL;
  char* bp = tk_strdup(position);
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  return_value_if_fail(bp != NULL, RET_BAD_PARAMS);

  p = strchr(bp, ':');
  if (p != NULL) {
    value_t v;
    int32_t line = tk_atoi(p + 1);
    tk_object_t* lines = NULL;

    *p = '\0';
    lines = tk_object_get_prop_object(lldb->source_break_points, bp);
    if (lines != NULL) {
      object_array_remove_value(lines, value_set_int(&v, line));
      object_array_push(lines, value_set_int(&v, line));
    } else {
      lines = object_array_create();
      if (lines != NULL) {
        object_array_push(lines, value_set_int(&v, line));
        tk_object_set_prop_object(lldb->source_break_points, bp, lines);
        TK_OBJECT_UNREF(lines);
      }
    }
  } else {
    darray_remove(&(lldb->functions_break_points), bp);
    if (darray_push(&(lldb->functions_break_points), bp) != RET_OK) {
      TKMEM_FREE(bp);
    }
  }

  return debugger_lldb_update_break_points(debugger);
}

static ret_t debugger_lldb_remove_break_point_ex(debugger_t* debugger, const char* position) {
  char* p = NULL;
  char* bp = tk_strdup(position);
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  return_value_if_fail(bp != NULL, RET_BAD_PARAMS);

  p = strchr(bp, ':');
  if (p != NULL) {
    value_t v;
    int32_t line = tk_atoi(p + 1);
    tk_object_t* lines = NULL;

    *p = '\0';
    lines = tk_object_get_prop_object(lldb->source_break_points, bp);
    if (lines != NULL) {
      object_array_remove_value(lines, value_set_int(&v, line));
    } else {
      log_debug("not found break point: %s\n", position);
    }
  } else {
    darray_remove(&(lldb->functions_break_points), bp);
    TKMEM_FREE(bp);
  }

  return debugger_lldb_update_break_points(debugger);
}

static ret_t debugger_lldb_attach(debugger_t* debugger, const char* lang, const char* code_id) {
  return RET_FAIL;
}

static ret_t debugger_lldb_deinit(debugger_t* debugger) {
  return RET_FAIL;
}

static ret_t debugger_lldb_update_code(debugger_t* debugger, const binary_data_t* code) {
  return RET_NOT_IMPL;
}

static ret_t debugger_lldb_launch(debugger_t* debugger, const char* lang,
                                  const binary_data_t* code) {
  return RET_NOT_IMPL;
}

static ret_t debugger_lldb_get_code(debugger_t* debugger, binary_data_t* code) {
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  return_value_if_fail(lldb != NULL && code != NULL, RET_BAD_PARAMS);

  debugger_lldb_dispatch_messages(debugger);

  if (code != NULL && lldb->current_frame_source != NULL) {
    code->data = (void*)(lldb->current_frame_source);
    code->size = tk_strlen(lldb->current_frame_source);
    return RET_OK;
  } else {
    code->data = NULL;
    code->size = 0;
    return RET_FAIL;
  }
}

static ret_t debugger_lldb_get_debuggers(debugger_t* debugger, binary_data_t* debuggers) {
  return RET_NOT_IMPL;
}

static ret_t on_source_breakpoints(void* ctx, const void* data) {
  uint32_t i = 0;
  str_t* str = (str_t*)ctx;
  named_value_t* nv = (named_value_t*)data;
  object_array_t* arr = (object_array_t*)value_object(&(nv->value));

  for (i = 0; i < arr->size; i++) {
    int32_t line = value_int(arr->props + i);
    str_append_format(str, MAX_PATH + 32, "%s:%d\n", nv->name, line);
  }

  return RET_OK;
}

static ret_t debugger_lldb_get_break_points(debugger_t* debugger, binary_data_t* break_points) {
  uint32_t i = 0;
  darray_t* arr = NULL;
  debugger_lldb_t* lldb = DEBUGGER_LLDB(debugger);
  str_t* str = &(lldb->scallstack);

  str_clear(str);
  arr = &(lldb->functions_break_points);
  for (i = 0; i < arr->size; i++) {
    const char* iter = (const char*)darray_get(arr, i);
    str_append_more(str, iter, "\n", NULL);
  }

  tk_object_foreach_prop(lldb->source_break_points, on_source_breakpoints, str);

  break_points->data = str->str;
  break_points->size = str->size;

  return RET_OK;
}

static const debugger_vtable_t s_debugger_lldb_vtable = {
    .attach = debugger_lldb_attach,
    .launch = debugger_lldb_launch,
    .launch_app = debugger_lldb_launch_app,
    .lock = debugger_lldb_lock,
    .unlock = debugger_lldb_unlock,
    .stop = debugger_lldb_stop,
    .pause = debugger_lldb_pause,
    .get_var = debugger_lldb_get_var,
    .restart = debugger_lldb_restart,
    .step_in = debugger_lldb_step_in,
    .step_out = debugger_lldb_step_out,
    .step_over = debugger_lldb_step_over,
    .step_loop_over = debugger_lldb_step_loop_over,
    .continve = debugger_lldb_continue,
    .get_local = debugger_lldb_get_local,
    .get_self = debugger_lldb_get_self,
    .get_global = debugger_lldb_get_global,
    .get_threads = debugger_lldb_get_threads,
    .get_code = debugger_lldb_get_code,
    .get_debuggers = debugger_lldb_get_debuggers,
    .get_break_points = debugger_lldb_get_break_points,
    .get_callstack = debugger_lldb_get_callstack,
    .dispatch_messages = debugger_lldb_dispatch_messages,
    .update_code = debugger_lldb_update_code,
    .set_break_point_ex = debugger_lldb_set_break_point_ex,
    .remove_break_point_ex = debugger_lldb_remove_break_point_ex,
    .clear_break_points = debugger_lldb_clear_break_points,
    .set_current_frame = debugger_lldb_set_current_frame,
    .deinit = debugger_lldb_deinit,
};

debugger_lldb_t* debugger_lldb_cast(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt == &s_debugger_lldb_vtable, NULL);

  return (debugger_lldb_t*)debugger;
}

static ret_t debugger_lldb_on_destroy(tk_object_t* obj) {
  debugger_t* debugger = DEBUGGER(obj);
  debugger_lldb_t* lldb = DEBUGGER_LLDB(obj);
  return_value_if_fail(lldb != NULL, RET_BAD_PARAMS);

  if (debugger_get_state(debugger) == DEBUGGER_PROGRAM_STATE_RUNNING) {
    debugger_lldb_disconnect(DEBUGGER(obj), TRUE);
  }

  str_reset(&(lldb->body));
  str_reset(&(lldb->header));
  str_reset(&(lldb->scallstack));
  str_reset(&(lldb->sbreakpoints));
  TK_OBJECT_UNREF(lldb->io);
  TK_OBJECT_UNREF(lldb->sources);
  TK_OBJECT_UNREF(lldb->callstack);
  TK_OBJECT_UNREF(lldb->source_break_points);
  TK_OBJECT_UNREF(lldb->resps);
  darray_deinit(&(lldb->functions_break_points));

  return RET_OK;
}

static const object_vtable_t s_object_debugger_lldb_vtable = {
    .type = "object_debugger_lldb",
    .desc = "object_debugger_lldb",
    .size = sizeof(debugger_lldb_t),
    .is_collection = FALSE,
    .on_destroy = debugger_lldb_on_destroy};

debugger_t* debugger_lldb_create_impl(tk_iostream_t* io) {
  debugger_lldb_t* debugger = NULL;
  return_value_if_fail(io != NULL, NULL);
  debugger = (debugger_lldb_t*)tk_object_create(&s_object_debugger_lldb_vtable);
  return_value_if_fail(debugger != NULL, NULL);

  debugger->io = io;
  TK_OBJECT_REF(debugger->io);
  debugger->debugger.vt = &s_debugger_lldb_vtable;
  str_init(&(debugger->body), 10000);
  str_init(&(debugger->header), 10000);
  str_init(&(debugger->scallstack), 10000);
  str_init(&(debugger->sbreakpoints), 10000);
  debugger->resps = object_default_create();
  debugger->sources = object_default_create();
  debugger->source_break_points = object_default_create();
  darray_init(&(debugger->functions_break_points), 10, default_destroy, (tk_compare_t)strcmp);

  return (debugger_t*)debugger;
}

debugger_t* debugger_lldb_create(const char* host, uint32_t port) {
  int32_t sock = 0;
  tk_iostream_t* io = NULL;
  debugger_t* debugger = NULL;

  return_value_if_fail(host != NULL, NULL);
  sock = tk_tcp_connect(host, port);
  return_value_if_fail(sock >= 0, NULL);

  io = tk_iostream_tcp_create(sock);
  if (io != NULL) {
    debugger = debugger_lldb_create_impl(io);
    TK_OBJECT_UNREF(io);
    debugger_lldb_init(debugger);
  } else {
    socket_close(sock);
  }

  return debugger;
}
