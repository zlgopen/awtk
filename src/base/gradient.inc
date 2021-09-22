
/**
 * File: gradient.c
 * Author: AWTK Develop Team
 * Brief:  vector graphic gradient
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-07-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "base/gradient.h"
#include "tkc/color_parser.h"

gradient_t* gradient_init(gradient_t* gradient) {
  return_value_if_fail(gradient != NULL, NULL);
  memset(gradient, 0x00, sizeof(gradient_t));

  return gradient;
}

gradient_t* gradient_init_simple(gradient_t* gradient, uint32_t color) {
  color_t c;
  return_value_if_fail(gradient != NULL, NULL);

  c.color = color;
  gradient_init(gradient);
  gradient->type = GRADIENT_LINEAR;
  gradient_add_stop(gradient, c, 0); 

  return gradient;
}

static ret_t gradient_fix_stops(gradient_t* gradient) {
  uint32_t i = 0;
  if (gradient->nr > 1) {
    float nr = gradient->nr - 1;
    for (i = 0; i < gradient->nr; i++) {
      gradient_stop_t* iter = gradient->stops + i;
      if (iter->offset < 0) {
        iter->offset = (float)i / nr;
      }
    }
  }

  return RET_OK;
}

static gradient_t* gradient_parse_stops(gradient_t* gradient, tokenizer_t* t) {
  color_t color;
  float offset = 0;
  const char* token = NULL;
  const char* p = t->str + t->cursor;

  while (*p == ' ' || *p == '(' || *p == ',') {
    p++;
  }

  while (*p && *p != ')') {
    if (tk_str_start_with(p, "rgb")) {
      color = color_parse(p);
      p = strchr(p, ')');
      return_value_if_fail(p != NULL, NULL);
      p++;
    } else {
      t->cursor = p - t->str;
      token = tokenizer_next_until(t, " ,)");
      color = color_parse(token);
    }

    /*skip color*/
    while (*p && *p != ')' && *p != ' ' && *p != ',') {
      p++;
    }

    if (*p == ' ') {
      p++;
      offset = tk_atof(p) / 100;
    } else {
      offset = -1;
    }
    gradient_add_stop(gradient, color, offset);

    /*skip offset*/
    while (*p && *p != ')' && *p != ',') {
      p++;
    }

    /*skip seperator*/
    while (*p == ' ' || *p == ',') {
      p++;
    }
  }

  gradient_fix_stops(gradient);

  return gradient;
}

static gradient_t* gradient_parse_linear(gradient_t* gradient, tokenizer_t* t) {
  int32_t degree = 0;
  const char* token = NULL;
  uint32_t cursor = t->cursor;
  token = tokenizer_next_until(t, ",)");
  return_value_if_fail(token != NULL, NULL);

  gradient_set_type(gradient, GRADIENT_LINEAR);
  if (isdigit(*token)) {
    if (strstr(token, "deg") != NULL) {
    /*
     * linear-gradient(0deg, blue, green 40%, red);
     */
      degree = tk_atoi(token);
    } else {
      log_debug("not support %s\n", token);
      return NULL;
    }
  } else if (*token == 't') {
    /**
     *linear-gradient(to left top, blue, red);
     */
    if (strstr(token, "left") != NULL) {
      if (strstr(token, "top") != NULL) {
        degree = 315;
      } else if (strstr(token, "bottom") != NULL) {
        degree = 225;
      } else {
        degree = 270;
      }
    } else if (strstr(token, "right") != NULL) {
      if (strstr(token, "top") != NULL) {
        degree = 45;
      } else if (strstr(token, "bottom") != NULL) {
        degree = 135;
      } else {
        degree = 90;
      }
    } else if (strstr(token, "top") != NULL) {
      degree = 0;
    } else if (strstr(token, "bottom") != NULL) {
      degree = 180;
    }
  } else {
    /*it is color*/
    t->cursor = cursor;
  }
  gradient_set_linear_degree(gradient, degree);

  return gradient_parse_stops(gradient, t);
}

static gradient_t* gradient_parse_radial(gradient_t* gradient, tokenizer_t* t) {
  const char* token = NULL;
  uint32_t cursor = t->cursor;
  token = tokenizer_next_until(t, ",)");
  return_value_if_fail(token != NULL, NULL);

  gradient_set_type(gradient, GRADIENT_LINEAR);
  if (tk_str_start_with(token, "circle")) {
    /*
     * radial-gradient(circle, #333, #333 50%, #eee 75%, #333 75%);
     */
  } else if (tk_str_start_with(token, "ellipse") || tk_str_start_with(token, "closest") ||
             tk_str_start_with(token, "farthest")) {
    /*
     * radial-gradient(ellipse at top, #e66465, transparent),
     * radial-gradient(closest-side, #3f87a6, #ebf8e1, #f69d3c);
     */
    log_debug("not support type:%s\n", token);
    return NULL;
  } else {
    /*it is color*/
    t->cursor = cursor;
  }

  return gradient_parse_stops(gradient, t);
}

static gradient_t* gradient_parse_str(gradient_t* gradient, const char* str) {
  tokenizer_t tokenizer;
  const char* token = NULL;
  tokenizer_t* t = tokenizer_init(&tokenizer, str, strlen(str), ":");
  token = tokenizer_next_until(t, "(");
  if (*token == 'l') {
    /*
     * linear-gradient(0deg, blue, green 40%, red);
     */
    t->cursor++;
    gradient_parse_linear(gradient, t);
  } else if (*token == 'r') {
    /*
     * radial-gradient(circle, rgba(2,0,36,1) 0%, rgba(63,95,131,1) 58%, rgba(0,212,255,1) 100%);
     */
    t->cursor++;
    gradient_parse_radial(gradient, t);
  }
  tokenizer_deinit(t);

  return gradient;
}

ret_t gradient_to_str(gradient_t* gradient, str_t* str) {
  uint32_t i = 0;
  char offset[32];
  char color[TK_COLOR_HEX_LEN + 1];
  return_value_if_fail(gradient != NULL && str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + 128) == RET_OK, RET_OOM);

  if (gradient->type == GRADIENT_LINEAR) {
    str_append(str, "linear-gradient(");
    str_append_int(str, gradient->degree);
    str_append(str, "deg");
  } else if (gradient->type == GRADIENT_RADIAL) {
    str_append(str, "radial-gradient(circle");
  }

  for (i = 0; i < gradient->nr; i++) {
    gradient_stop_t* iter = gradient->stops + i;
    color_hex_str(iter->color, color);
    tk_snprintf(offset, sizeof(offset) - 1, "%d", (int)(iter->offset * 100));

    str_append_more(str, ", ", color, " ", offset, "%", NULL);
  }
  str_append(str, ")");

  return RET_OK;
}

gradient_t* gradient_init_from_str(gradient_t* gradient, const char* str) {
  return_value_if_fail(str != NULL, NULL);
  return_value_if_fail(gradient_init(gradient) != NULL, NULL);

  return gradient_parse_str(gradient, str);
}

static gradient_t* gradient_parse_binary(gradient_t* gradient, const uint8_t* data, uint32_t size) {
  color_t c;
  rbuffer_t rb;
  uint32_t i = 0;
  uint8_t nr = 0;
  float offset = 0;
  uint32_t color = 0;
  uint8_t type = 0;
  uint16_t degree = 0;
  uint32_t correct_size = 0;
  rbuffer_init(&rb, data, size);
  rbuffer_read_uint16(&rb, &degree);
  rbuffer_read_uint8(&rb, &type);
  rbuffer_read_uint8(&rb, &nr);
  correct_size = sizeof(uint32_t) + nr * (sizeof(float) + sizeof(uint32_t));
  return_value_if_fail(size >= correct_size, NULL);
  return_value_if_fail(nr < TK_GRADIENT_MAX_STOP_NR, NULL);

  gradient->nr = 0;
  gradient->degree = degree;
  gradient->type = (gradient_type_t)type;

  for (i = 0; i < nr; i++) {
    rbuffer_read_float(&rb, &offset);
    rbuffer_read_uint32(&rb, &color);

    c.color = color;
    gradient_add_stop(gradient, c, offset);
  }

  return gradient;
}

ret_t gradient_to_binary(gradient_t* gradient, wbuffer_t* wb) {
  uint32_t i = 0;
  uint32_t size = sizeof(uint32_t) + TK_GRADIENT_MAX_STOP_NR * (sizeof(uint32_t) + sizeof(float));
  return_value_if_fail(gradient != NULL && wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wbuffer_extend_capacity(wb, wb->cursor + size) == RET_OK, RET_OOM);

  wbuffer_write_uint16(wb, gradient->degree);
  wbuffer_write_uint8(wb, gradient->type);
  wbuffer_write_uint8(wb, gradient->nr);

  for (i = 0; i < gradient->nr; i++) {
    gradient_stop_t* iter = gradient->stops + i;
    wbuffer_write_float(wb, iter->offset);
    wbuffer_write_uint32(wb, iter->color.color);
  }

  return RET_OK;
}

gradient_t* gradient_init_from_binary(gradient_t* gradient, const uint8_t* data, uint32_t size) {
  uint32_t least_size = sizeof(uint32_t) + sizeof(gradient_stop_t);
  return_value_if_fail(data != NULL && size > least_size, NULL);
  return_value_if_fail(gradient_init(gradient) != NULL, NULL);

  return gradient_parse_binary(gradient, data, size);
}

ret_t gradient_set_type(gradient_t* gradient, gradient_type_t type) {
  return_value_if_fail(gradient != NULL, RET_BAD_PARAMS);
  gradient->type = type;

  return RET_OK;
}

ret_t gradient_set_linear_degree(gradient_t* gradient, uint32_t degree) {
  return_value_if_fail(gradient != NULL, RET_BAD_PARAMS);
  gradient->degree = degree;

  return RET_OK;
}

ret_t gradient_add_stop(gradient_t* gradient, color_t color, float offset) {
  return_value_if_fail(gradient != NULL && gradient->nr < TK_GRADIENT_MAX_STOP_NR, RET_BAD_PARAMS);

  gradient->stops[gradient->nr].color = color;
  gradient->stops[gradient->nr++].offset = offset;

  return RET_OK;
}

gradient_stop_t* gradient_get_stop(gradient_t* gradient, uint32_t index) {
  return_value_if_fail(gradient != NULL, NULL);
  return_value_if_fail(gradient->nr > index, NULL);

  return gradient->stops + index;
}

color_t gradient_get_first_color(gradient_t* gradient) {
  return_value_if_fail(gradient != NULL && gradient->nr > 0, color_init(0xff, 0xff, 0xff, 0xff));

  return gradient->stops[0].color;
}

color_t gradient_get_last_color(gradient_t* gradient) {
  return_value_if_fail(gradient != NULL && gradient->nr > 0, color_init(0xff, 0xff, 0xff, 0xff));

  return gradient->stops[gradient->nr - 1].color;
}

ret_t gradient_deinit(gradient_t* gradient) {
  return_value_if_fail(gradient != NULL, RET_BAD_PARAMS);
  memset(gradient, 0x00, sizeof(gradient_t));

  return RET_OK;
}

color_t gradient_get_color(gradient_t* gradient, float offset) {
  color_t c = color_init(0x00, 0, 0, 0);
  return_value_if_fail(gradient != NULL && gradient->nr > 0, c);
  return_value_if_fail(offset >= 0 && offset <= 1, c);

  if (gradient->nr == 1) {
    return gradient_get_first_color(gradient);
  } else {
    uint32_t i = 0;
    for (i = 0; i < gradient->nr; i++) {
      gradient_stop_t* iter = gradient->stops + i;
      if (iter->offset == offset || (i + 1) == gradient->nr) {
        return iter->color;
      } else if (offset > iter->offset) {
        gradient_stop_t* next = gradient->stops + i + 1;
        if (offset == next->offset) {
          return next->color;
        } else if (offset < next->offset) {
          float range = next->offset - iter->offset;
          float percent = (offset - iter->offset) / range;

          c.rgba.r = (1 - percent) * iter->color.rgba.r + percent * next->color.rgba.r;
          c.rgba.g = (1 - percent) * iter->color.rgba.g + percent * next->color.rgba.g;
          c.rgba.b = (1 - percent) * iter->color.rgba.b + percent * next->color.rgba.b;
          c.rgba.a = (1 - percent) * iter->color.rgba.a + percent * next->color.rgba.a;

          return c;
        }
      }
    }
  }

  return gradient_get_last_color(gradient);
}
