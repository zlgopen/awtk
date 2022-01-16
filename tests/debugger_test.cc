#include "debugger/debugger_fscript.h"
#include "debugger/debugger_global.h"
#include "debugger/debugger_server.h"
#include "debugger/debugger_message.h"
#include "debugger/debugger_server_tcp.h"
#include "debugger/debugger_client_tcp.h"

#include "tkc/fscript.h"
#include "tkc/thread.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

#define DEBUGGER_TCP_PORT 8080

static void* fscript_thread_entry(void* args) {
  value_t v;
  fscript_t* fscript = (fscript_t*)args;
  fscript_exec(fscript, &v);
  fscript_destroy(fscript);

  return NULL;
}

static ret_t on_debugger_client_event(void* ctx, event_t* e) {
  str_t* str = (str_t*)ctx;
  switch (e->type) {
    case DEBUGGER_RESP_MSG_BREAKED: {
      debugger_breaked_event_t* event = debugger_breaked_event_cast(e);
      str_append(str, "breaked");
      str_append(str, "(");
      str_append_int(str, event->line);
      str_append(str, ")");
      break;
    }
    case DEBUGGER_RESP_MSG_LOG: {
      debugger_log_event_t* event = debugger_log_event_cast(e);
      str_append(str, "log");
      str_append(str, "(");
      str_append_int(str, event->line);
      str_append(str, ",\"");
      str_append(str, event->message);
      str_append(str, "\")");
      break;
    }
    case DEBUGGER_RESP_MSG_ERROR: {
      debugger_error_event_t* event = debugger_error_event_cast(e);
      str_append(str, "error");
      str_append(str, "(");
      str_append_int(str, event->line);
      str_append(str, ",\"");
      str_append(str, event->message);
      str_append(str, "\")");
      break;
    }
    case DEBUGGER_RESP_MSG_COMPLETED: {
      str_append(str, "completed()");
    }
    default:
      break;
  }

  return RET_OK;
}

TEST(Debugger, event1) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  debugger_t* debugger = debugger_server_find_debugger(fscript->code_id);
  ASSERT_EQ(debugger_is_paused(debugger), TRUE);
  ASSERT_EQ(debugger_is_paused(client), TRUE);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str, "breaked(0)log(0,\"1\")log(1,\"2\")log(2,\"3\")completed()");
  str_reset(&str);
}

#if 1
TEST(Debugger, event2) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 2), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  debugger_t* debugger = debugger_server_find_debugger(fscript->code_id);
  ASSERT_EQ(debugger_is_paused(debugger), TRUE);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();

  ASSERT_STREQ(str.str, "log(0,\"1\")log(1,\"2\")breaked(2)log(2,\"3\")completed()");
  str_reset(&str);
}

TEST(Debugger, basic) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  debugger_t* debugger = debugger_server_find_debugger(fscript->code_id);
  ASSERT_EQ(debugger_is_paused(debugger), TRUE);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, basic1) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 1), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 2), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  ASSERT_EQ(debugger_next(client), RET_OK);
  ASSERT_EQ(debugger_next(client), RET_OK);
  ASSERT_EQ(debugger_remove_break_point(client, 2), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, local) {
  const char* code =
      "var aaa=111\nvar bbb =222;\nvar "
      "ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  tk_object_t* local = debugger_get_local(client, 0);
  ASSERT_EQ(tk_object_get_prop_int(local, "aaa", 0), 111);
  ASSERT_EQ(tk_object_get_prop_int(local, "bbb", 0), 222);
  ASSERT_STREQ(tk_object_get_prop_str(local, "ccc"), "abc");

  TK_OBJECT_UNREF(local);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, self) {
  const char* code =
      "aaa=111\nbbb "
      "=222;\nccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  tk_object_t* self = debugger_get_self(client);
  ASSERT_EQ(tk_object_get_prop_int(self, "aaa", 0), 111);
  ASSERT_EQ(tk_object_get_prop_int(self, "bbb", 0), 222);
  ASSERT_STREQ(tk_object_get_prop_str(self, "ccc"), "abc");
  TK_OBJECT_UNREF(self);

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, global) {
  const char* code =
      "global.aaa=111\nglobal.bbb "
      "=222;\nglobal.ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  tk_object_t* global = debugger_get_global(client);
  ASSERT_EQ(tk_object_get_prop_int(global, "aaa", 0), 111);
  ASSERT_EQ(tk_object_get_prop_int(global, "bbb", 0), 222);
  ASSERT_STREQ(tk_object_get_prop_str(global, "ccc"), "abc");
  TK_OBJECT_UNREF(global);

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, callstack) {
  const char* code =
      "global.aaa=111\nglobal.bbb "
      "=222;\nglobal.ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, code) {
  const char* code =
      "global.aaa=111\nglobal.bbb "
      "=222;\nglobal.ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_init(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data;
  data.data = (void*)"print(1)";
  data.size = strlen((char*)data.data);
  ASSERT_EQ(debugger_update_code(client, &data), RET_OK);

  data.data = NULL;
  ASSERT_EQ(debugger_get_code(client, &data), RET_OK);
  ASSERT_STREQ((char*)data.data, "print(1)");

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}
#endif
