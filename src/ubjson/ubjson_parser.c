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

typedef enum _parse_state_t { STATE_NONE, STATE_KEY, STATE_VALUE, STATE_ARRAY } parse_state_t;

typedef struct _ubjson_parser_t {
  str_t temp;
  rbuffer_t rb;
  object_t* obj;
  object_t* root;
  parse_state_t state;
  ubjson_reader_t reader;

  void* ctx;
  ubjson_on_key_value_t on_key_value;
} ubjson_parser_t;

#define PROP_PARENT "__parent__"

static ret_t rbuffer_read_binary_if_has_more(rbuffer_t* rbuffer, void* data, uint32_t size) {
  if (rbuffer_has_more(rbuffer)) {
    return rbuffer_read_binary(rbuffer, data, size);
  }

  return RET_DONE;
}

static ubjson_parser_t* ubjson_parser_init(ubjson_parser_t* parser, void* data, uint32_t size,
                                           ubjson_on_key_value_t on_key_value, void* ctx) {
  str_t* temp = &(parser->temp);
  rbuffer_t* rb = &(parser->rb);
  ubjson_reader_t* reader = &(parser->reader);

  parser->ctx = ctx;
  parser->on_key_value = on_key_value;

  str_init(temp, 64);
  rbuffer_init(rb, data, size);
  parser->state = STATE_NONE;
  ubjson_reader_init(reader, (ubjson_read_callback_t)rbuffer_read_binary_if_has_more, rb);

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

static ret_t ubjson_on_key_value_object(void* ctx, const char* key, value_t* v) {
  ret_t ret = RET_OK;
  ubjson_parser_t* parser = (ubjson_parser_t*)ctx;

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_BEGIN) {
      if (key == NULL) {
        parser->obj = object_default_create();
        return_value_if_fail(parser->obj != NULL, RET_OOM);
        if(parser->root != NULL) {
          parser->root = parser->obj;
        }
      } else {
        object_t* obj = object_default_create();
        return_value_if_fail(obj != NULL, RET_OOM);

        object_set_prop_object(parser->obj, key, obj);
        object_set_prop_pointer(obj, PROP_PARENT, parser->obj);
        parser->obj = obj;
        object_unref(obj);
      }
    } else if (token == UBJSON_MARKER_OBJECT_END) {
      object_t* parent = (object_t*)object_get_prop_pointer(parser->obj, PROP_PARENT);
      parser->obj = parent;
    } else {
      assert(!"not supported");
      ret = RET_NOT_IMPL;
    }
  } else {
    ret = object_set_prop(parser->obj, key, v);
  }

  return ret;
}

static ret_t ubjson_parser_on_value(ubjson_parser_t* parser, value_t* v) {
  str_t* str = &(parser->temp);

  switch (parser->state) {
    case STATE_NONE: {
      if (v->type == VALUE_TYPE_TOKEN && value_token(v) == UBJSON_MARKER_OBJECT_BEGIN) {
        parser->on_key_value(parser->ctx, NULL, v);
        parser->state = STATE_KEY;
      } else {
        assert(!"invalid format");
        return RET_BAD_PARAMS;
      }
      break;
    }
    case STATE_KEY: {
      if (v->type == VALUE_TYPE_TOKEN) {
        if (value_token(v) == UBJSON_MARKER_OBJECT_END) {
          parser->on_key_value(parser->ctx, NULL, v);
        } else if (value_token(v) == UBJSON_MARKER_ARRAY_END) {
          parser->on_key_value(parser->ctx, NULL, v);
        } else {
          assert(!"invalid format");
          return RET_BAD_PARAMS;
        }
      } else if (v->type == VALUE_TYPE_STRING) {
        return_value_if_fail(str_set(str, value_str(v)) == RET_OK, RET_OOM);
        parser->state = STATE_VALUE;
      } else if (v->type == VALUE_TYPE_INT8 || v->type == VALUE_TYPE_INT16 ||
                 v->type == VALUE_TYPE_UINT8 || v->type == VALUE_TYPE_INT32) {
        int32_t len = value_int(v);
        ubjson_reader_t* reader = &(parser->reader);

        str_extend(str, len + 1);
        parser->state = STATE_VALUE;
        return_value_if_fail(ubjson_reader_read_data(reader, str->str, len) == RET_OK, RET_FAIL);
        str->str[len] = '\0';
      } else {
        assert(!"invalid format");
        return RET_BAD_PARAMS;
      }
      break;
    }
    case STATE_VALUE: {
      if (v->type == VALUE_TYPE_TOKEN) {
        if (value_token(v) == UBJSON_MARKER_OBJECT_BEGIN) {
          parser->state = STATE_KEY;
          parser->on_key_value(parser->ctx, str->str, v);
        } else if (value_token(v) == UBJSON_MARKER_ARRAY_BEGIN) {
          parser->state = STATE_ARRAY;
          parser->on_key_value(parser->ctx, str->str, v);
        } else {
          assert(!"invalid format");
          return RET_BAD_PARAMS;
        }
      } else {
        parser->state = STATE_KEY;
        parser->on_key_value(parser->ctx, str->str, v);
      }
      break;
    }
    case STATE_ARRAY: {
      if (v->type == VALUE_TYPE_TOKEN) {
        if (value_token(v) == UBJSON_MARKER_ARRAY_END) {
          parser->state = STATE_KEY;
          parser->on_key_value(parser->ctx, NULL, v);
        } else if (value_token(v) == UBJSON_MARKER_OBJECT_BEGIN) {
          parser->state = STATE_KEY;
          parser->on_key_value(parser->ctx, NULL, v);
        }
      } else {
        parser->on_key_value(parser->ctx, NULL, v);
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

object_t* object_from_ubjson(void* data, uint32_t size) {
  ubjson_parser_t parser;
  return_value_if_fail(data != NULL && size > 0, NULL);

  ubjson_parser_init(&parser, data, size, ubjson_on_key_value_object, &parser);
  ubjson_do_parse(&parser);
  ubjson_parser_deinit(&parser);

  return parser.root;
}

static ret_t ubjson_on_key_value_dump(void* ctx, const char* key, value_t* v) {
  int32_t i = 0;
  ret_t ret = RET_OK;
  int32_t* level = (int32_t*)ctx;
  ubjson_parser_t* parser = (ubjson_parser_t*)ctx;

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_END) {
      *level = *level - 1;
    }
  }

  for (i = 0; i < *level; i++) {
    log_debug(" ");
  }

  if (key != NULL) {
    log_debug("\"%s\":", key);
  }

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_BEGIN) {
      log_debug("{\n");
      *level = *level + 1;
    } else if (token == UBJSON_MARKER_OBJECT_END) {
      log_debug("},\n");
    } else if (token == UBJSON_MARKER_ARRAY_BEGIN) {
      log_debug("[");
    } else if (token == UBJSON_MARKER_ARRAY_END) {
      log_debug("],\n");
    } else {
      assert(!"not supported");
      ret = RET_NOT_IMPL;
    }
  } else {
    if (v->type == VALUE_TYPE_BOOL) {
      log_debug("%s", value_bool(v) ? "true" : "false");
    } else if (v->type == VALUE_TYPE_FLOAT) {
      log_debug("%f", value_float32(v));
    } else if (v->type == VALUE_TYPE_DOUBLE) {
      log_debug("%lf", value_double(v));
    } else if (v->type == VALUE_TYPE_STRING) {
      log_debug("\"%s\"", value_str(v));
    } else {
      log_debug("%d", value_int(v));
    }

    if (key != NULL) {
      log_debug(",\n");
    } else {
      log_debug(", ");
    }
  }

  return ret;
}

ret_t ubjson_parse(void* data, uint32_t size, ubjson_on_key_value_t on_key_value, void* ctx) {
  ubjson_parser_t parser;
  return_value_if_fail(data != NULL && size > 0, RET_BAD_PARAMS);

  ubjson_parser_init(&parser, data, size, on_key_value, ctx);
  ubjson_do_parse(&parser);
  ubjson_parser_deinit(&parser);

  return RET_OK;
}

ret_t ubjson_dump(void* data, uint32_t size) {
  int32_t level = 0;

  return ubjson_parse(data, size, ubjson_on_key_value_dump, &level);
}
