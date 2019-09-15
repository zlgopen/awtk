/**
 * File:   ubjson_parser.c
 * Author: AWTK Develop Team
 * Brief:  ubjson parser
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/buffer.h"
#include "ubjson/ubjson_parser.h"

typedef enum _parse_state_t {
  STATE_NONE,
  STATE_KEY,
  STATE_VALUE,
  STATE_ARRAY,
} parse_state_t;

typedef struct _ubjson_parser_t {
  void* ctx;
  str_t temp;
  rbuffer_t rb;
  object_t* obj;
  object_t* root;
  parse_state_t state;
  ubjson_reader_t reader;
} ubjson_parser_t;

#define PROP_PARENT "__parent__"

static ubjson_parser_t* ubjson_parser_init(ubjson_parser_t* parser, void* data, uint32_t size) {
  str_t* temp = &(parser->temp);
  rbuffer_t* rb = &(parser->rb);
  ubjson_reader_t* reader = &(parser->reader);

  str_init(temp, 64);
  rbuffer_init(rb, data, size);
  parser->state = STATE_NONE;
  ubjson_reader_init(reader, (ubjson_read_callback_t)rbuffer_read_binary, rb);

  return parser;
}

static ret_t ubjson_parser_deinit(ubjson_parser_t* parser) {
  str_t* temp = &(parser->temp);
  ubjson_reader_t* reader = &(parser->reader);

  str_reset(temp);
  ubjson_reader_reset(reader);

  return RET_OK;
}

static ret_t ubjson_parser_read(ubjson_parser_t* parser, value_t* v) {
  ubjson_reader_t* reader = &(parser->reader);

  return ubjson_reader_read(reader, v);
}

static ret_t ubjson_parser_on_value(ubjson_parser_t* parser, value_t* v) {
  str_t* str = &(parser->temp);

  switch (parser->state) {
    case STATE_NONE: {
      if (v->type == VALUE_TYPE_TOKEN && value_token(v) == UBJSON_MARKER_OBJECT_BEGIN) {
        parser->obj = object_default_create();
        return_value_if_fail(parser->obj != NULL, RET_OOM);

        parser->state = STATE_KEY;
        parser->root = parser->obj;
      } else {
        assert(!"invalid format");
        return RET_BAD_PARAMS;
      }
      break;
    }
    case STATE_KEY: {
      if (v->type == VALUE_TYPE_TOKEN) {
        if (value_token(v) == UBJSON_MARKER_OBJECT_END) {
          object_t* parent = (object_t*)object_get_prop_pointer(parser->obj, PROP_PARENT);
          parser->obj = parent;
        } else {
          assert(!"invalid format");
          return RET_BAD_PARAMS;
        }
      } else if (v->type == VALUE_TYPE_STRING) {
        return_value_if_fail(str_set(str, value_str(v)) == RET_OK, RET_OOM);
        parser->state = STATE_VALUE;
      } else {
        assert(!"invalid format");
        return RET_BAD_PARAMS;
      }
      break;
    }
    case STATE_VALUE: {
      if (v->type == VALUE_TYPE_TOKEN) {
        if (value_token(v) == UBJSON_MARKER_OBJECT_BEGIN) {
          object_t* obj = object_default_create();
          return_value_if_fail(obj != NULL, RET_OOM);

          parser->state = STATE_KEY;
          object_set_prop_object(parser->obj, str->str, obj);
          object_set_prop_pointer(obj, PROP_PARENT, parser->obj);
          parser->obj = obj;

          object_unref(obj);
        } else {
          assert(!"invalid format");
          return RET_BAD_PARAMS;
        }
      } else {
        parser->state = STATE_KEY;
        object_set_prop(parser->obj, str->str, v);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

ret_t ubjson_do_parse(ubjson_parser_t* parser) {
  value_t v;

  while (TRUE) {
    if (ubjson_parser_read(parser, &v) != RET_OK) {
      break;
    }

    ubjson_parser_on_value(parser, &v);
  }

  return RET_OK;
}

object_t* ubjson_parse(void* data, uint32_t size) {
  ubjson_parser_t parser;
  return_value_if_fail(data != NULL && size > 0, NULL);

  ubjson_parser_init(&parser, data, size);
  ubjson_do_parse(&parser);
  ubjson_parser_deinit(&parser);

  return parser.root;
}
