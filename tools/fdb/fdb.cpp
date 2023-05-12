/**
 * File:   fdb.c
 * Author: AWTK Develop Team
 * Brief:  fscript remote debugger
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2022-01-20 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc.h"
#include "debugger/debugger.h"
#include "debugger/debugger_lldb.h"
#include "debugger/debugger_client_tcp.h"

#ifndef WIN32
#include <readline/history.h>
#include <readline/readline.h>

static char* command_generator(const char* text, int state);
static char** command_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (start == 0) {
    matches = rl_completion_matches(text, command_generator);
  }

  return (matches);
}
static ret_t aw_read_line_init(void) {
  rl_readline_name = (char*)"fdb";
  rl_attempted_completion_function = command_completion;

  return RET_OK;
}

static char* aw_read_line(const char* tips) {
  return readline(tips);
}

static ret_t aw_read_line_free(char* line) {
  free(line);
  return RET_OK;
}

static ret_t aw_read_line_add_history(char* line) {
  add_history(line);
  return RET_OK;
}

#else
static char s_line[1024];
static ret_t aw_read_line_init(void) {
  return RET_OK;
}

static char* aw_read_line(const char* tips) {
  printf("%s", tips);
  fflush(stdout);
  fgets(s_line, sizeof(s_line) - 1, stdin);

  return s_line;
}

static ret_t aw_read_line_free(char* line) {
  return RET_OK;
}

static ret_t aw_read_line_add_history(char* line) {
  return RET_OK;
}
#endif /**/

typedef struct _app_info_t {
  tk_object_t* obj;
  bool_t completed;
  debugger_t* debugger;
  int32_t current_line;
  const char* current_func;
} app_info_t;

#include "tkc/data_reader_mem.h"
#include "conf_io/conf_ubjson.h"
#include "tkc/time_now.h"

#define KNRM "\x1B[0m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KMAG "\x1B[35m"

typedef ret_t (*cmd_line_func_t)(app_info_t* app, tokenizer_t* tokenizer);

typedef struct _cmd_entry_t {
  const char* name;
  const char* alias;
  const char* desc;
  const char* help;
  cmd_line_func_t func;
} cmd_entry_t;

#define SHOW_CODE_LINES 10

static ret_t fdb_show_code(app_info_t* app, bool_t all) {
  tokenizer_t t;
  int32_t line = 0;
  binary_data_t code = {0, NULL};
  int32_t start = 0;
  int32_t end = 0;
  bool_t meet_return = FALSE;

  if (debugger_get_code(app->debugger, &code) != RET_OK) {
    return RET_FAIL;
  }

  end = app->current_line + SHOW_CODE_LINES;
  start = tk_max_int(app->current_line - SHOW_CODE_LINES, 0);
  tokenizer_init_ex(&t, (const char*)(code.data), code.size, "", "\r\n");

  while (tokenizer_has_more(&t)) {
    const char* text = tokenizer_next(&t);
    if (*text == '\r') {
      line++;
      meet_return = TRUE;
      continue;
    } else if (*text == '\n') {
      if (meet_return) {
        meet_return = FALSE;
      } else {
        line++;
      }
      continue;
    }

    if (line >= start && line <= end) {
      if (line == app->current_line) {
        log_debug(KGRN "%d: =>%s" KNRM, line, text);
      } else {
        log_debug("%d:   %s", line, text);
      }
      log_debug("\n");
    }

    if (line > end) {
      break;
    }
  }

  tokenizer_deinit(&t);
  log_debug("\n");

  return RET_OK;
}

static ret_t fdb_show_threads(const char* title, tk_object_t* obj) {
  log_debug("%s:\n", title);
  log_debug("---------------------\n");
  if (obj != NULL) {
    uint32_t i = 0;
    int32_t id = 0;
    const char* name = NULL;
    char path[MAX_PATH + 1] = {0};
    uint32_t n = tk_object_get_prop_uint32(obj, "body.threads.#size", 0);

    for (i = 0; i < n; i++) {
      tk_snprintf(path, sizeof(path) - 1, "body.threads.[%d].name", i);
      name = tk_object_get_prop_str(obj, path);

      tk_snprintf(path, sizeof(path) - 1, "body.threads.[%d].id", i);
      id = tk_object_get_prop_int32(obj, path, 0);

      log_debug("[%d]: %s\n", id, name);
    }
  }

  return RET_OK;
}

static ret_t fdb_show_variables(const char* title, tk_object_t* obj) {
  log_debug("%s:\n", title);
  log_debug("---------------------\n");
  if (obj != NULL) {
    uint32_t i = 0;
    const char* name = NULL;
    const char* type = NULL;
    const char* value = NULL;
    char path[MAX_PATH + 1] = {0};
    uint32_t n = tk_object_get_prop_uint32(obj, "body.variables.#size", 0);

    for (i = 0; i < n; i++) {
      tk_snprintf(path, sizeof(path) - 1, "body.variables.[%d].evaluateName", i);
      name = tk_object_get_prop_str(obj, path);
      tk_snprintf(path, sizeof(path) - 1, "body.variables.[%d].value", i);
      value = tk_object_get_prop_str(obj, path);

      tk_snprintf(path, sizeof(path) - 1, "body.variables.[%d].type", i);
      type = tk_object_get_prop_str(obj, path);

      log_debug("[%2d] %s (%s): %s\n", i, name, type, value);
    }
  }

  return RET_OK;
}

static ret_t func_launch(app_info_t* app, tokenizer_t* tokenizer) {
  int argc = 0;
  char* argv[32];
  char exec[MAX_PATH + 1] = {0};
  char work_dir[MAX_PATH + 1] = {0};
  const char* program = tokenizer_next(tokenizer);

  if (program == NULL) {
    return RET_BAD_PARAMS;
  }

  if (tk_str_start_with(program, STR_SCHEMA_PID) || tk_str_start_with(program, STR_SCHEMA_WASM)) {
    return debugger_launch_app(app->debugger, program, work_dir, argc, argv);
  } else if (tk_str_eq(program, "wasm")) {
    return debugger_launch_app(app->debugger, "wasm://127.0.0.1:1234", work_dir, argc, argv);
  }

  while (tokenizer_has_more(tokenizer)) {
    argv[argc++] = strdup(tokenizer_next(tokenizer));
  }

  path_abs(program, exec, sizeof(exec) - 1);
  path_cwd(work_dir);
  debugger_launch_app(app->debugger, exec, work_dir, argc, argv);

  while (argc > 0) {
    TKMEM_FREE(argv[--argc]);
  }

  return RET_OK;
}

static ret_t fdb_on_debugger_events(void* ctx, event_t* e) {
  str_t astr;
  str_t* str = &astr;
  app_info_t* app = (app_info_t*)ctx;

  str_init(str, 100);
  switch (e->type) {
    case DEBUGGER_RESP_MSG_FRAME_CHANGED: {
      debugger_frame_changed_event_t* event = debugger_frame_changed_event_cast(e);
      app->current_func = event->func;
      app->current_line = event->line;
      break;
    }
    case DEBUGGER_RESP_MSG_BREAKED: {
      debugger_breaked_event_t* event = debugger_breaked_event_cast(e);
      app->current_line = event->line;
      break;
    }
    case DEBUGGER_RESP_MSG_LOG: {
      debugger_log_event_t* event = debugger_log_event_cast(e);
      str_append(str, ">");
      str_append_int(str, event->line);
      str_append(str, ":");
      str_append(str, event->message);
      break;
    }
    case DEBUGGER_RESP_MSG_ERROR: {
      debugger_error_event_t* event = debugger_error_event_cast(e);
      str_append(str, ">>");
      str_append_int(str, event->line);
      str_append(str, ":");
      str_append(str, event->message);
      break;
    }
    case DEBUGGER_RESP_MSG_COMPLETED: {
      str_append(str, "completed()");
      app->completed = TRUE;
    }
    default:
      break;
  }

  log_debug("%s\n", str->str);
  str_reset(str);

  return RET_OK;
}

static ret_t func_target(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_t* debugger = NULL;
  const char* target = tokenizer_next(tokenizer);
  const char* host = "localhost";

  if (target == NULL) {
    return RET_BAD_PARAMS;
  }

  TK_OBJECT_UNREF(app->debugger);
  if (tk_str_eq(target, "lldb")) {
    debugger = debugger_lldb_create(host, DEBUGGER_TCP_PORT);
  } else {
    const char* code_id = DEBUGGER_DEFAULT_CODE_ID;

    debugger = debugger_client_tcp_create(host, DEBUGGER_TCP_PORT);
    if (debugger == NULL) {
      log_debug("connect to \"%s\" %d failed!\n", host, DEBUGGER_TCP_PORT);
      exit(0);
    }
    debugger_attach(debugger, DEBUGGER_LANG_FSCRIPT, code_id);
    debugger_set_break_point(debugger, DEBUGGER_START_LINE);
  }

  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_BREAKED, fdb_on_debugger_events, app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_FRAME_CHANGED, fdb_on_debugger_events, app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_LOG, fdb_on_debugger_events, app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_ERROR, fdb_on_debugger_events, app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED, fdb_on_debugger_events, app);

  app->debugger = debugger;

  return RET_OK;
}

static ret_t func_local(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = NULL;
  int32_t index = tokenizer_next_int(tokenizer, -1);

  if (index < 0) {
    index = app->debugger->current_frame_index;
  }
  obj = debugger_get_local(app->debugger, index);
  fdb_show_variables("local vars", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_print(app_info_t* app, tokenizer_t* tokenizer) {
  const char* name = tokenizer->str + tokenizer->cursor;
  tk_object_t* obj = debugger_get_var(app->debugger, name);
  fdb_show_variables("var", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_self(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = debugger_get_self(app->debugger);
  fdb_show_variables("member vars", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_threads(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = debugger_get_threads(app->debugger);
  fdb_show_threads("threads:", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_global(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = debugger_get_global(app->debugger);
  fdb_show_variables("global vars", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_pause(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_pause(app->debugger);
  return RET_OK;
}

static ret_t func_stop(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_stop(app->debugger);
  return RET_OK;
}

static ret_t func_restart(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_restart(app->debugger);
  sleep_ms(300);
  return fdb_show_code(app, FALSE);
}

static ret_t func_set_break(app_info_t* app, tokenizer_t* tokenizer) {
  if (app->debugger->vt->set_break_point_ex != NULL) {
    const char* position = tokenizer_next(tokenizer);
    return debugger_set_break_point_ex(app->debugger, position);
  } else if (app->debugger->vt->set_break_point != NULL) {
    int32_t line = tokenizer_next_int(tokenizer, 0);
    return debugger_set_break_point(app->debugger, line);
  }

  return RET_BAD_PARAMS;
}

static ret_t fdb_show_break_points(app_info_t* app) {
  int32_t i = 0;
  tokenizer_t t;
  binary_data_t data = {0, NULL};
  debugger_get_break_points(app->debugger, &data);

  tokenizer_init(&t, (char*)data.data, data.size, "\n");
  log_debug("breakpoints:\n---------------------------\n");
  while (tokenizer_has_more(&t)) {
    const char* bp = tokenizer_next(&t);
    log_debug(" [%d] %s\n", i, bp);
    i++;
  }
  tokenizer_deinit(&t);

  return RET_OK;
}

static ret_t func_list_break(app_info_t* app, tokenizer_t* tokenizer) {
  fdb_show_break_points(app);

  return RET_OK;
}

static ret_t func_list_debuggers(app_info_t* app, tokenizer_t* tokenizer) {
  binary_data_t data = {0, NULL};

  debugger_get_debuggers(app->debugger, &data);
  log_debug("debuggers:\n%s", (char*)data.data);

  return RET_OK;
}

static ret_t fdb_show_callstack(app_info_t* app) {
  int32_t i = 0;
  tokenizer_t t;
  binary_data_t data = {0, NULL};
  debugger_get_callstack(app->debugger, &data);
  return_value_if_fail(data.data != NULL, RET_BAD_PARAMS);

  tokenizer_init(&t, (char*)data.data, data.size, "\n");
  log_debug("callstack:\n---------------------------\n");
  while (tokenizer_has_more(&t)) {
    const char* func = tokenizer_next(&t);
    if (i == app->debugger->current_frame_index) {
      log_debug(KGRN "=> [%d] %s\n" KNRM, i, func);
    } else {
      log_debug("   [%d] %s\n", i, func);
    }
    i++;
  }
  tokenizer_deinit(&t);

  return RET_OK;
}

static ret_t func_backtrace(app_info_t* app, tokenizer_t* tokenizer) {
  fdb_show_callstack(app);

  return RET_OK;
}

static ret_t func_remove_break(app_info_t* app, tokenizer_t* tokenizer) {
  const char* bp = tokenizer_next(tokenizer);
  if (bp != NULL) {
    log_debug("remove break at line: %s\n", bp);
    return debugger_remove_break_point_ex(app->debugger, bp);
  } else {
    log_debug("remove all break points\n");
    return debugger_clear_break_points(app->debugger);
  }
}

static ret_t func_step_in(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_in(app->debugger);
  sleep_ms(300);
  debugger_dispatch_messages(app->debugger);
  return fdb_show_code(app, FALSE);
}

static ret_t func_next(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_over(app->debugger);
  sleep_ms(300);
  debugger_dispatch_messages(app->debugger);
  return fdb_show_code(app, FALSE);
}

static ret_t func_continue(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_continue(app->debugger);
  sleep_ms(300);
  debugger_dispatch_messages(app->debugger);
  return fdb_show_code(app, FALSE);
}

static ret_t func_flush(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_dispatch_messages(app->debugger);
  sleep_ms(300);
  return RET_OK;
}

static ret_t func_step_out(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_out(app->debugger);
  sleep_ms(300);
  debugger_dispatch_messages(app->debugger);
  return fdb_show_code(app, FALSE);
}

static ret_t func_step_loop_over(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_loop_over(app->debugger);
  sleep_ms(300);
  debugger_dispatch_messages(app->debugger);
  return fdb_show_code(app, FALSE);
}

static ret_t func_frame(app_info_t* app, tokenizer_t* tokenizer) {
  int32_t index = tokenizer_next_int(tokenizer, -1);
  if (index >= 0) {
    debugger_set_current_frame(app->debugger, index);
    return fdb_show_code(app, FALSE);
  } else {
    return RET_BAD_PARAMS;
  }
}

static ret_t func_get_code(app_info_t* app, tokenizer_t* tokenizer) {
  fdb_show_code(app, FALSE);
  return RET_OK;
}

static ret_t func_quit(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_deinit(app->debugger);
  return RET_STOP;
}

static ret_t func_help(app_info_t* app, tokenizer_t* tokenizer);
static const cmd_entry_t s_cmds[] = {
    {"target", "t", "select target", "t [default|lldb]", func_target},
    {"run", "r", "run a program", "r program arg1 arg2...", func_launch},
    {"print", "p", "show a var, support path(eg: a.b[1].name).", "p name", func_print},
    {"frame", "f", "select current frame", "f index", func_frame},
    {"flush", "fl", "flush socket", "fl", func_flush},
    {"next", "n", "run next line code", "n", func_next},
    {"pause", "ps", "pause the running program", "pause", func_pause},
    {"stop", "stop", "stop the running program", "stop", func_stop},
    {"step_loop_over", "u", "step loop over", "u", func_step_loop_over},
    {"step_in", "s", "step in function", "s", func_step_in},
    {"step_out", "so", "step out function", "so", func_step_out},
    {"continue", "c", "continue", "c", func_continue},
    {"get_code", "l", "show source code", "l", func_get_code},
    {"break", "b", "set break point", "b func\n   b filename:line", func_set_break},
    {"list_break", "lb", "list breakpoints", "lb", func_list_break},
    {"list_debuggers", "ld", "list debuggers", "ld", func_list_debuggers},
    {"delete_break", "d", "delete breakpoints", "d position", func_remove_break},
    {"local", "local", "show local variables", "local", func_local},
    {"self", "self", "show member variables", "self", func_self},
    {"global", "global", "show global variables", "global", func_global},
    {"threads", "threads", "show threads", "threads", func_threads},
    {"backtrace", "bt", "show backtrace", "bt", func_backtrace},
    {"quit", "q", "Quit debugger", "q", func_quit},
    {"restart", "rs", "restart app", "rs", func_restart},
    {NULL, NULL, NULL}};

static char* command_generator(const char* text, int state) {
  static int list_index, len;

  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  while (s_cmds[list_index].name != NULL) {
    const cmd_entry_t* iter = s_cmds + list_index;
    list_index++;

    if (iter->name == NULL) {
      break;
    }

    if (strncmp(iter->name, text, len) == 0) {
      return strdup(iter->name);
    }

    if (strncmp(iter->help, text, len) == 0) {
      return strdup(iter->help);
    }
  }

  return ((char*)NULL);
}

static ret_t fdb_show_help(app_info_t* app, const char* cmd) {
  uint32_t i = 0;
  printf(KMAG "================================================\n" KNRM);
  while (s_cmds[i].name != NULL) {
    const cmd_entry_t* iter = s_cmds + i;
    if (cmd == NULL || tk_str_eq(iter->name, cmd) || tk_str_eq(iter->alias, cmd)) {
      printf(KYEL "%u: %s(%s)\n" KNRM, i, iter->desc, iter->alias);
      printf("------------------------------\n");
      printf(" # %s\n", iter->help);
      printf(KGRN "------------------------------------------------\n" KNRM);
    }
    i++;
  }
  printf(KMAG "================================================\n" KNRM);

  return RET_OK;
}

static ret_t func_help(app_info_t* app, tokenizer_t* tokenizer) {
  return fdb_show_help(app, NULL);
}

static ret_t register_functions(object_t* obj) {
  uint32_t i = 0;
  while (s_cmds[i].name != NULL) {
    const cmd_entry_t* iter = s_cmds + i;
    tk_object_set_prop_pointer(obj, iter->alias, (void*)(iter->func));
    tk_object_set_prop_pointer(obj, iter->name, (void*)(iter->func));
    i++;
  }
  return RET_OK;
}

static ret_t fdb_shell_exec(app_info_t* app, const char* line) {
  tokenizer_t t;
  ret_t ret = RET_OK;
  const char* name = NULL;
  cmd_line_func_t func = NULL;

  return_value_if_fail(app != NULL && line != NULL, RET_BAD_PARAMS);
  tokenizer_init(&t, line, strlen(line), " \t\r\n");

  name = tokenizer_next(&t);
  if (name == NULL || *name == '\0') {
    return RET_OK;
  }

  func = (cmd_line_func_t)object_get_prop_pointer(app->obj, name);
  if (func == NULL) {
    func = func_help;
  }

  if (app->debugger == NULL) {
    if (!tk_str_eq(name, "target") && !tk_str_eq(name, "t") && !tk_str_eq(name, "quit") &&
        !tk_str_eq(name, "q") && !tk_str_eq(name, "help") && !tk_str_eq(name, "h")) {
      printf("Please use target command to choose debug target.\n");
      return RET_OK;
    }
  }

  ret = func(app, &t);
  if (ret == RET_BAD_PARAMS) {
    fdb_show_help(app, name);
  }

  return ret;
}

ret_t fdb_shell_run(void) {
  app_info_t app;
  tk_object_t* obj = object_default_create();

  memset(&app, 0x00, sizeof(app));
  app.obj = obj;
  app.debugger = NULL;
  app.completed = FALSE;
  app.current_line = 0xffff;

  aw_read_line_init();
  register_functions(obj);

  while (TRUE) {
    char* line = aw_read_line(KMAG "[fdb] # " KNRM);
    if (line == NULL) {
      break;
    }

    if (app.debugger != NULL) {
      debugger_dispatch_messages(app.debugger);
    }
    if (fdb_shell_exec(&app, line) == RET_STOP) {
      aw_read_line_free(line);
      break;
    }
    if (app.debugger != NULL) {
      debugger_dispatch_messages(app.debugger);
    }

    aw_read_line_add_history(line);
    aw_read_line_free(line);

    if (app.completed) {
      break;
    }
  }

  app.debugger = NULL;
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"

int main(int argc, char* argv[]) {
  platform_prepare();
  tk_socket_init();

  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);

  fdb_shell_run();

  tk_socket_deinit();

  return 0;
}
