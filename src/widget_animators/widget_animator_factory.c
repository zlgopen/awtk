/**
 * File:   widget_animator_factory.c
 * Author: AWTK Develop Team
 * Brief:  widget animator factory
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-10-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/enums.h"
#include "tkc/utils.h"
#include "tkc/func_call_parser.h"
#include "base/widget_animator_factory.h"

#ifndef WITHOUT_WIDGET_ANIMATOR

#include "widget_animators/widget_animator_prop.h"
#include "widget_animators/widget_animator_move.h"
#include "widget_animators/widget_animator_scale.h"

typedef struct _move_params_t {
  xy_t x_to;
  xy_t y_to;
  xy_t x_from;
  xy_t y_from;
} move_params_t;

typedef struct _scale_params_t {
  float_t x_to;
  float_t y_to;
  float_t x_from;
  float_t y_from;
} scale_params_t;

typedef struct _prop_params_t {
  float_t to;
  float_t from;
} prop_params_t;

typedef struct _animator_params_t {
  char type[TK_NAME_LEN + 1];
  char name[TK_NAME_LEN + 1];

  widget_t* widget;
  union {
    move_params_t move;
    scale_params_t scale;
    prop_params_t prop;
  } u;

  uint32_t delay;
  int32_t easing;
  uint32_t duration;
  int32_t yoyo_times;
  int32_t repeat_times;

  float_t time_scale;
  bool_t auto_start;
  bool_t auto_destroy;

} animator_params_t;

typedef struct _widget_animator_parser_t {
  func_call_parser_t base;

  animator_params_t params;
} widget_animator_parser_t;

static ret_t parser_on_name(func_call_parser_t* parser, const char* func_name) {
  value_t v;
  widget_animator_parser_t* p = (widget_animator_parser_t*)parser;
  widget_t* widget = p->params.widget;
  const char* type = p->params.type;

  tk_strncpy(p->params.type, func_name, TK_NAME_LEN);
  if (tk_str_eq(type, "move")) {
    move_params_t* params = &p->params.u.move;

    params->x_from = widget->x;
    params->y_from = widget->y;
    params->x_to = widget->x;
    params->y_to = widget->y;
  } else if (tk_str_eq(type, "scale")) {
    float_t scale_x = 1.0f;
    float_t scale_y = 1.0f;
    scale_params_t* params = &p->params.u.scale;

    if (widget_get_prop(widget, WIDGET_PROP_SCALE_X, &v) == RET_OK) {
      scale_x = value_float(&v);
    }
    if (widget_get_prop(widget, WIDGET_PROP_SCALE_Y, &v) == RET_OK) {
      scale_y = value_float(&v);
    }

    params->x_from = scale_x;
    params->y_from = scale_y;
    params->x_to = scale_x;
    params->y_to = scale_y;
  } else {
    float_t value = 0;
    const char* prop_name = p->params.type;
    prop_params_t* params = &p->params.u.prop;

    if (widget_get_prop(widget, prop_name, &v) == RET_OK) {
      value = value_float(&v);
    }

    params->from = value;
    params->to = value;
  }

  return RET_OK;
}

static ret_t parser_on_param(func_call_parser_t* parser, const char* name, const char* value) {
  widget_animator_parser_t* p = (widget_animator_parser_t*)parser;
  const char* type = p->params.type;

  if (tk_str_eq(type, "move")) {
    move_params_t* move = &p->params.u.move;
    if (tk_str_eq(name, "x_from")) {
      move->x_from = tk_atoi(value);
      return RET_OK;
    } else if (tk_str_eq(name, "y_from")) {
      move->y_from = tk_atoi(value);
      return RET_OK;
    } else if (tk_str_eq(name, "x_to")) {
      move->x_to = tk_atoi(value);
      return RET_OK;
    } else if (tk_str_eq(name, "y_to")) {
      move->y_to = tk_atoi(value);
      return RET_OK;
    }
  } else if (tk_str_eq(type, "scale")) {
    scale_params_t* scale = &p->params.u.scale;
    if (tk_str_eq(name, "x_from")) {
      scale->x_from = tk_atof(value);
      return RET_OK;
    } else if (tk_str_eq(name, "y_from")) {
      scale->y_from = tk_atof(value);
      return RET_OK;
    } else if (tk_str_eq(name, "x_to")) {
      scale->x_to = tk_atof(value);
      return RET_OK;
    } else if (tk_str_eq(name, "y_to")) {
      scale->y_to = tk_atof(value);
      return RET_OK;
    }
  } else {
    prop_params_t* v = &p->params.u.prop;
    if (tk_str_eq(name, "from")) {
      v->from = tk_atof(value);
      return RET_OK;
    } else if (tk_str_eq(name, "to")) {
      v->to = tk_atof(value);
      return RET_OK;
    }
  }

  switch (name[0]) {
    case 'y': /*yoyo_times*/
    {
      p->params.yoyo_times = tk_atoi(value);
      break;
    }
    case 't': /*time_scale*/
    {
      p->params.time_scale = tk_atof(value);
      break;
    }
    case 'n': /*name*/
    {
      tk_strncpy(p->params.name, value, TK_NAME_LEN);
      break;
    }
    case 'r': /*repeat_times*/
    {
      p->params.repeat_times = tk_atoi(value);
      break;
    }
    case 'd': /*duration|delay*/
    {
      if (tk_str_eq(name, "delay")) {
        p->params.delay = tk_atoi(value);
      } else if (tk_str_eq(name, "duration")) {
        p->params.duration = tk_atoi(value);
      }
      break;
    }
    case 'a': /*auto_start|auto_destroy*/
    {
      if (tk_str_eq(name, "auto_start")) {
        p->params.auto_start = tk_atob(value);
      } else if (tk_str_eq(name, "auto_destroy")) {
        p->params.auto_destroy = tk_atob(value);
      }
      break;
    }
    case 'e': {
      const key_type_value_t* easing = easing_type_find(value);
      p->params.easing = easing != NULL ? (int32_t)(easing->value) : -1;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t widget_animator_parser_parse(widget_animator_parser_t* parser, const char* str,
                                          widget_t* widget) {
  memset(parser, 0x00, sizeof(*parser));
  func_call_parser_init(&(parser->base), str, strlen(str));

  parser->params.delay = 0;
  parser->params.duration = 500;
  parser->params.yoyo_times = -1;
  parser->params.repeat_times = -1;
  parser->params.auto_start = TRUE;
  parser->params.auto_destroy = TRUE;
  parser->params.time_scale = 1;
  parser->params.easing = EASING_SIN_INOUT;

  parser->params.widget = widget;
  parser->base.on_name = parser_on_name;
  parser->base.on_param = parser_on_param;
  parser->base.on_done = NULL;

  return func_call_parser_parse(&(parser->base));
}

widget_animator_t* widget_animator_create(widget_t* widget, const char* params) {
  uint32_t delay = 0;
  uint32_t easing = 0;
  uint32_t duration = 0;
  const char* type = NULL;
  widget_animator_t* wa = NULL;
  widget_animator_parser_t parser;
  return_value_if_fail(params != NULL && widget != NULL, NULL);

  widget_animator_parser_parse(&parser, params, widget);

  delay = parser.params.delay;
  easing = parser.params.easing;
  duration = parser.params.duration;

  type = parser.params.type;
  if (tk_str_eq(type, "move")) {
    move_params_t* move = &parser.params.u.move;
    wa = widget_animator_move_create(widget, duration, delay, (easing_type_t)easing);

    return_value_if_fail(wa != NULL, NULL);
    widget_animator_move_set_params(wa, move->x_from, move->y_from, move->x_to, move->y_to);
  } else if (tk_str_eq(type, "scale")) {
    scale_params_t* scale = &parser.params.u.scale;
    wa = widget_animator_scale_create(widget, duration, delay, (easing_type_t)easing);

    return_value_if_fail(wa != NULL, NULL);
    widget_animator_scale_set_params(wa, scale->x_from, scale->y_from, scale->x_to, scale->y_to);
  } else {
    const char* prop_name = parser.params.type;
    prop_params_t* param = &parser.params.u.prop;

    wa = widget_animator_prop_create(widget, duration, delay, (easing_type_t)easing, prop_name);
    return_value_if_fail(wa != NULL, NULL);
    widget_animator_prop_set_params(wa, param->from, param->to);
  }

  func_call_parser_deinit(&(parser.base));

  if (wa != NULL) {
    if (parser.params.yoyo_times >= 0) {
      widget_animator_set_yoyo(wa, parser.params.yoyo_times);
    }

    if (parser.params.repeat_times >= 0) {
      widget_animator_set_repeat(wa, parser.params.repeat_times);
    }

    if (parser.params.auto_start) {
      widget_animator_start(wa);
    }

    widget_animator_set_time_scale(wa, parser.params.time_scale);
    widget_animator_set_destroy_when_done(wa, parser.params.auto_destroy);
    if (parser.params.name[0]) {
      widget_animator_set_name(wa, parser.params.name);
    } else {
      widget_animator_set_name(wa, parser.params.type);
    }
  }

  return wa;
}
#endif /*WITHOUT_WIDGET_ANIMATOR*/
