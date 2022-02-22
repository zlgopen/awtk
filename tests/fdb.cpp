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

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/thread.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "tkc/tokenizer.h"
#include "tkc/object_default.h"
#include "tkc/socket_helper.h"
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
  int32_t break_at_line;
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

static ret_t code_get_line(const char* code, uint32_t size, int32_t offset, str_t* str) {
  if (offset >= size) {
    return RET_FAIL;
  }

  for (; offset < size; offset++) {
    char c = code[offset];
    str_append_char(str, c);
    if (c == '\n') {
      break;
    }
  }

  return RET_OK;
}

static ret_t debugger_show_code(app_info_t* app, bool_t all) {
  str_t str;
  int32_t offset = 0;
  int32_t line = 0;
  binary_data_t code = {0, NULL};
  if (debugger_get_code(app->debugger, &code) != RET_OK) {
    return RET_FAIL;
  }

  str_init(&str, 100);
  while (code_get_line((const char*)(code.data), code.size, offset, &str) == RET_OK) {
    if ((tk_abs((line - app->break_at_line)) < (SHOW_CODE_LINES / 2)) || all) {
      if (line == app->break_at_line) {
        log_debug(KGRN "%d: =>%s" KNRM, line, str.str);
      } else {
        log_debug("%d:   %s", line, str.str);
      }
    }
    line++;
    offset += str.size;
    str_clear(&str);
  }
  log_debug("\n");
  str_reset(&str);
  return RET_OK;
}

static ret_t dump_object_prop(void* ctx, const void* data) {
  char buff[64];
  named_value_t* nv = (named_value_t*)data;
  value_str_ex(&(nv->value), buff, sizeof(buff));
  log_debug(" %s=%s\n", nv->name, buff);

  return RET_OK;
}

static ret_t dump_object(const char* title, tk_object_t* obj) {
  log_debug("%s:\n", title);
  log_debug("---------------------\n");
  tk_object_foreach_prop(obj, dump_object_prop, NULL);

  return RET_OK;
}

static ret_t func_local(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = debugger_get_local(app->debugger, 0);
  dump_object("local vars", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_self(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = debugger_get_self(app->debugger);
  dump_object("member vars", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_global(app_info_t* app, tokenizer_t* tokenizer) {
  tk_object_t* obj = debugger_get_global(app->debugger);
  dump_object("global vars", obj);
  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_next(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_next(app->debugger);
  sleep_ms(300);
  return debugger_show_code(app, FALSE);
}

static ret_t func_continue(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_continue(app->debugger);
  sleep_ms(300);
  return debugger_show_code(app, FALSE);
}

static ret_t func_set_break(app_info_t* app, tokenizer_t* tokenizer) {
  int32_t line = tokenizer_next_int(tokenizer, 0);
  log_debug("set break at line: %d\n", line);
  return debugger_set_break_point(app->debugger, line);
}

static ret_t func_list_break(app_info_t* app, tokenizer_t* tokenizer) {
  binary_data_t data = {0, NULL};

  debugger_get_break_points(app->debugger, &data);
  log_debug("break points:\n%s", (char*)data.data);

  return RET_OK;
}

static ret_t func_list_debuggers(app_info_t* app, tokenizer_t* tokenizer) {
  binary_data_t data = {0, NULL};

  debugger_get_debuggers(app->debugger, &data);
  log_debug("debuggers:\n%s", (char*)data.data);

  return RET_OK;
}

static ret_t func_backtrace(app_info_t* app, tokenizer_t* tokenizer) {
  binary_data_t data = {0, NULL};

  debugger_get_callstack(app->debugger, &data);
  log_debug("callstack:\n%s", (char*)data.data);

  return RET_OK;
}

static ret_t func_remove_break(app_info_t* app, tokenizer_t* tokenizer) {
  int32_t line = tokenizer_next_int(tokenizer, -1);
  if (line >= 0) {
    log_debug("remove break at line: %d\n", line);
    return debugger_remove_break_point(app->debugger, line);
  } else {
    log_debug("remove all break points\n");
    return debugger_clear_break_points(app->debugger);
  }
}

static ret_t func_step_in(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_in(app->debugger);
  sleep_ms(300);
  return debugger_show_code(app, FALSE);
}

static ret_t func_step_out(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_out(app->debugger);
  sleep_ms(300);
  return debugger_show_code(app, FALSE);
}

static ret_t func_step_over(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_step_over(app->debugger);
  sleep_ms(300);
  return debugger_show_code(app, FALSE);
}

static ret_t func_get_code(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_show_code(app, TRUE);
  return RET_OK;
}

static ret_t func_quit(app_info_t* app, tokenizer_t* tokenizer) {
  debugger_deinit(app->debugger);
  return RET_STOP;
}

static ret_t func_help(app_info_t* app, tokenizer_t* tokenizer);
static const cmd_entry_t s_cmds[] = {
    {"next", "n", "run next line code", "next", func_next},
    {"step_over", "u", "step over", "step over", func_step_over},
    {"step_in", "s", "step in", "step in script function", func_step_in},
    {"step_out", "so", "step out", "step out script fuction", func_step_out},
    {"continue", "c", "continue", "continue", func_continue},
    {"get_code", "l", "get_code", "show source code", func_get_code},
    {"break", "b", "break", "set break point at line", func_set_break},
    {"list_break", "lb", "list break", "list break points", func_list_break},
    {"list_debuggers", "ld", "list debuggers", "list debuggers", func_list_debuggers},
    {"delete_break", "d", "delete break", "remove break point at line", func_remove_break},
    {"local", "local", "local", "show local variables", func_local},
    {"self", "self", "self", "show member variables", func_self},
    {"global", "global", "global", "show global variables", func_global},
    {"backtrace", "bt", "backtrace", "show backtrace", func_backtrace},
    {"quit", "q", "Quit loop", "quit", func_quit},
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

static ret_t func_help(app_info_t* app, tokenizer_t* tokenizer) {
  uint32_t i = 0;
  printf(KMAG "================================================\n" KNRM);
  while (s_cmds[i].name != NULL) {
    const cmd_entry_t* iter = s_cmds + i;
    printf(KYEL "%u: %s(%s)\n" KNRM, i, iter->desc, iter->alias);
    printf("------------------------------\n");
    printf(" # %s\n", iter->help);
    printf(KGRN "------------------------------------------------\n" KNRM);
    i++;
  }
  printf(KMAG "================================================\n" KNRM);

  return RET_OK;
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

static ret_t aw_flow_shell_exec(app_info_t* app, const char* line) {
  tokenizer_t t;
  ret_t ret = RET_OK;
  const char* name = NULL;
  cmd_line_func_t func = NULL;

  return_value_if_fail(app != NULL && app->debugger != NULL && line != NULL, RET_BAD_PARAMS);
  tokenizer_init(&t, line, strlen(line), " \t\r\n");

  name = tokenizer_next(&t);
  if (name == NULL || *name == '\0') {
    return RET_OK;
  }

  func = (cmd_line_func_t)object_get_prop_pointer(app->obj, name);
  if (func == NULL) {
    func = func_help;
  }

  ret = func(app, &t);
  if (ret == RET_BAD_PARAMS) {
    func_help(app, &t);
  }

  return ret;
}

static ret_t on_debugger_client_event(void* ctx, event_t* e) {
  str_t astr;
  str_t* str = &astr;
  app_info_t* app = (app_info_t*)ctx;

  str_init(str, 100);
  switch (e->type) {
    case DEBUGGER_RESP_MSG_BREAKED: {
      debugger_breaked_event_t* event = debugger_breaked_event_cast(e);
      str_append(str, "breaked");
      str_append(str, "(");
      str_append_int(str, event->line);
      str_append(str, ")");
      app->break_at_line = event->line;
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

ret_t aw_flow_shell_run(debugger_t* debugger) {
  app_info_t app;
  tk_object_t* obj = object_default_create();
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  app.obj = obj;
  app.break_at_line = 0xffff;
  app.debugger = debugger;
  app.completed = FALSE;

  aw_read_line_init();
  register_functions(obj);

  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &app);
  emitter_on(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &app);

  while (app.debugger != NULL) {
    char* line = aw_read_line(KMAG "[fdb] # " KNRM);
    if (line == NULL) {
      break;
    }

    if (aw_flow_shell_exec(&app, line) == RET_STOP) {
      aw_read_line_free(line);
      break;
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

int main(int argc, char* argv[]) {
  debugger_t* client = NULL;
  const char* host = argc == 2 ? argv[1] : "localhost";
  const char* code_id = argc == 3 ? argv[2] : DEBUGGER_DEFAULT_CODE_ID;

  platform_prepare();
  tk_socket_init();

  log_debug("Usage: %s [host] [code_id]\n", argv[0]);
  log_debug("host=%s code_id=%s\n", host, code_id);

  client = debugger_client_tcp_create(host, DEBUGGER_TCP_PORT);
  if (client == NULL) {
    log_debug("connect to \"%s\" %d failed!\n", host, DEBUGGER_TCP_PORT);
    exit(0);
  }

  debugger_attach(client, DEBUGGER_LANG_FSCRIPT, code_id);
  debugger_set_break_point(client, DEBUGGER_START_LINE);

  aw_flow_shell_run(client);

  TK_OBJECT_UNREF(client);
  tk_socket_deinit();

  return 0;
}
