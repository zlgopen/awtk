#include "base/utils.h"
#include "base/enums.h"
#include "base/easing.h"
#include "base/func_call_parser.h"

typedef struct _window_animation_parser_t {
  func_call_parser_t base;
  char name[NAME_LEN + 1];
  int duration;
  int easing;
} window_animation_parser_t;

static ret_t parser_on_name(func_call_parser_t* parser, const char* func_name) {
  window_animation_parser_t* p = (window_animation_parser_t*)parser;

  tk_strncpy(p->name, func_name, NAME_LEN);

  return RET_OK;
}

static ret_t parser_on_param(func_call_parser_t* parser, const char* name, const char* value) {
  window_animation_parser_t* p = (window_animation_parser_t*)parser;

  if (*name == 'd') { /*duration*/
    p->duration = tk_atoi(value);
  } else if (*name == 'e') { /*easing*/
    const key_type_value_t* easing = easing_type_find(value);
    p->easing = easing != NULL ? (int)(easing->value) : -1;
  }

  return RET_OK;
}

static ret_t window_animation_parser_parse(window_animation_parser_t* parser, const char* str) {
  memset(parser, 0x00, sizeof(*parser));
  func_call_parser_init(&(parser->base), str, strlen(str));

  parser->easing = -1;
  parser->base.on_name = parser_on_name;
  parser->base.on_param = parser_on_param;
  parser->base.on_done = NULL;

  return func_call_parser_parse(&(parser->base));
}

static window_animator_t* window_animator_create(const char* type, bool_t open) {
  window_animation_parser_t parser;
  window_animator_t* wa = NULL;

  if (type == NULL || *type == '\0') {
    return NULL;
  }

  window_animation_parser_parse(&parser, type);

  if (tk_str_eq(parser.name, WINDOW_ANIMATOR_CENTER_SCALE)) {
    wa = window_animator_create_scale(open);
  } else if (tk_str_eq(parser.name, WINDOW_ANIMATOR_HTRANSLATE)) {
    wa = window_animator_create_htranslate(open);
  } else if (tk_str_eq(parser.name, WINDOW_ANIMATOR_VTRANSLATE)) {
    wa = window_animator_create_vtranslate(open);
  } else if (tk_str_eq(parser.name, WINDOW_ANIMATOR_BOTTOM_TO_TOP)) {
    wa = window_animator_create_bottom_to_top(open);
  } else if (tk_str_eq(parser.name, WINDOW_ANIMATOR_TOP_TO_BOTTOM)) {
    wa = window_animator_create_top_to_bottom(open);
  } else if (tk_str_eq(parser.name, WINDOW_ANIMATOR_FADE)) {
    wa = window_animator_create_fade(open);
  }

  return_value_if_fail(wa != NULL, NULL);

  if (parser.duration > 0) {
    wa->duration = parser.duration;
  }

  if (parser.easing >= 0) {
    wa->easing = easing_get((easing_type_t)(parser.easing));
  }

  if (strchr(type, '=') != NULL) {
    log_debug("parser.duration:%d parser.easing=%d\n", parser.duration, parser.easing);
  }

  func_call_parser_deinit(&(parser.base));

  return wa;
}

window_animator_t* window_animator_create_for_open(const char* type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, TRUE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, TRUE);

  return wa;
}

window_animator_t* window_animator_create_for_close(const char* type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, FALSE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, FALSE);

  return wa;
}
