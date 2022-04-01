/**
 * File:   url.c
 * Author: AWTK Develop Team
 * Brief:  URL parser 
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/url.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"

static url_t* url_parse(url_t* url, const char* surl);

url_t* url_create(const char* surl) {
  url_t* url = TKMEM_ZALLOC(url_t);
  return_value_if_fail(url != NULL, NULL);

  if (surl != NULL) {
    return url_parse(url, surl);
  }

  return url;
}

typedef enum _state_t {
  STATE_START = 0,
  STATE_SCHEMA,
  STATE_HOST,
  STATE_PORT,
  STATE_PATH,
  STATE_KEY,
  STATE_VALUE
} state_t;

#define STR_FILE "file"
#define NUM_DEFAULT_PORT 80
#define STR_SCHEMA_DEFAULT "http"

static url_t* url_fix(url_t* url) {
  if (url->port == 0) {
    if (tk_str_eq(url->schema, "http")) {
      url->port = 80;
    } else if (tk_str_eq(url->schema, "https")) {
      url->port = 443;
    } else if (tk_str_eq(url->schema, "ftp")) {
      url->port = 21;
    }
  }
  return url;
}

static url_t* url_parse(url_t* url, const char* surl) {
  str_t str;
  const char* p = surl;
  const char* start = surl;
  state_t state = STATE_START;
  return_value_if_fail(str_init(&str, 100) != NULL, url);

  while (TRUE) {
    switch (state) {
      case STATE_START: {
        if (!tk_isspace(*p)) {
          if (*p == '/' || *p == '\\' || strncmp(p, "./", 2) == 0 || strncmp(p, ".\\", 2) == 0) {
            goto_error_if_fail(url_set_schema(url, STR_FILE) == RET_OK);
            state = STATE_PATH;
          } else {
            state = STATE_SCHEMA;
          }
          start = p;
        }
        break;
      }
      case STATE_SCHEMA: {
        if (*p == ':' || *p == '/' || *p == '\0') {
          goto_error_if_fail(str_set_with_len(&str, start, p - start) == RET_OK);

          if (strncmp(p, "://", 3) == 0) {
            /*http://zlg.cn*/
            p += 2;
            start = p + 1;
            state = STATE_HOST;
            goto_error_if_fail(url_set_schema(url, str.str) == RET_OK);
          } else if (strncmp(p, ":/", 2) == 0 || strncmp(p, ":\\", 2) == 0) {
            /*c:/a/b/c */
            goto_error_if_fail(url_set_schema(url, STR_FILE) == RET_OK);
            goto_error_if_fail(str.size == 1);
            state = STATE_PATH;
          } else if (*p == ':') {
            /*zlg.cn:80*/
            goto_error_if_fail(url_set_schema(url, STR_SCHEMA_DEFAULT) == RET_OK);
            goto_error_if_fail(url_set_host(url, str.str) == RET_OK);
            state = STATE_PORT;
            start = p + 1;
          } else if (*p == '/') {
            /*zlg.cn:80*/
            goto_error_if_fail(url_set_schema(url, STR_SCHEMA_DEFAULT) == RET_OK);
            goto_error_if_fail(url_set_port(url, NUM_DEFAULT_PORT) == RET_OK);
            goto_error_if_fail(url_set_host(url, str.str) == RET_OK);
            state = STATE_PATH;
            start = p + 1;
          } else {
            /*zlg.cn*/
            goto_error_if_fail(url_set_schema(url, STR_SCHEMA_DEFAULT) == RET_OK);
            goto_error_if_fail(url_set_port(url, NUM_DEFAULT_PORT) == RET_OK);
            goto_error_if_fail(url_set_host(url, str.str) == RET_OK);
            start = p + 1;
          }

          if (tk_str_eq(url->schema, STR_FILE)) {
            state = STATE_PATH;
          }
        }
        break;
      }
      case STATE_HOST: {
        if (*p == ':' || *p == '?' || *p == '/' || *p == '\0') {
          goto_error_if_fail(str_set_with_len(&str, start, p - start) == RET_OK);
          goto_error_if_fail(url_set_host(url, str.str) == RET_OK);

          switch (*p) {
            case ':': {
              state = STATE_PORT;
              break;
            }
            case '?': {
              state = STATE_KEY;
              break;
            }
            case '/': {
              p--;
              state = STATE_PATH;
              break;
            }
            default:
              break;
          }
          start = p + 1;
        }
        break;
      }
      case STATE_PORT: {
        if (*p == '@' || *p == '?' || *p == '/' || *p == '\0') {
          goto_error_if_fail(str_set_with_len(&str, start, p - start) == RET_OK);

          switch (*p) {
            case '@': {
              /*http://username:password@example.com*/
              goto_error_if_fail(url_set_user_name(url, url->host) == RET_OK);
              goto_error_if_fail(url_set_password(url, str.str) == RET_OK);
              TKMEM_FREE(url->host);
              state = STATE_HOST;
              break;
            }
            case '?': {
              /*https://www.zlg.cn:80?key=value*/
              state = STATE_KEY;
              url_set_port(url, tk_atoi(str.str));
              break;
            }
            case '/': {
              /*https://www.zlg.cn:80/awtk*/
              state = STATE_PATH;
              url_set_port(url, tk_atoi(str.str));
              break;
            }
            default: {
              /*https://www.zlg.cn:80*/
              url_set_port(url, tk_atoi(str.str));
              break;
            }
          }
          start = p + 1;
        }
        break;
      }
      case STATE_PATH: {
        if (*p == '?' || *p == '\0') {
          goto_error_if_fail(str_set_with_len(&str, start, p - start) == RET_OK);
          goto_error_if_fail(url_set_path(url, str.str) == RET_OK);
          state = STATE_KEY;
          start = p + 1;
        }
        break;
      }
      case STATE_KEY: {
        if (*p == '=' || *p == '\0') {
          goto_error_if_fail(str_set_with_len(&str, start, p - start) == RET_OK);
          state = STATE_VALUE;
          start = p + 1;
        }
        break;
      }
      case STATE_VALUE: {
        if (*p == '&' || *p == '\0') {
          const char* name = str.str;
          const char* value = str.str + str.size + 1;
          str.size++;
          goto_error_if_fail(str_append_with_len(&str, start, p - start) == RET_OK);
          goto_error_if_fail(url_set_param(url, name, value) == RET_OK);
          state = STATE_KEY;
          start = p + 1;
        }
        break;
      }
    }

    if (*p) {
      p++;
    } else {
      break;
    }
  }
  str_reset(&str);

  return url_fix(url);
error:

  str_reset(&str);
  url_destroy(url);

  return NULL;
}

ret_t url_set_schema(url_t* url, const char* schema) {
  return_value_if_fail(url != NULL && schema != NULL, RET_BAD_PARAMS);

  url->schema = tk_str_copy(url->schema, schema);

  return RET_OK;
}

ret_t url_set_user_name(url_t* url, const char* user_name) {
  return_value_if_fail(url != NULL && user_name != NULL, RET_BAD_PARAMS);

  url->user_name = tk_str_copy(url->user_name, user_name);

  return RET_OK;
}

ret_t url_set_password(url_t* url, const char* password) {
  return_value_if_fail(url != NULL && password != NULL, RET_BAD_PARAMS);

  url->password = tk_str_copy(url->password, password);

  return RET_OK;
}

ret_t url_set_host(url_t* url, const char* host) {
  return_value_if_fail(url != NULL && host != NULL, RET_BAD_PARAMS);

  url->host = tk_str_copy(url->host, host);

  return RET_OK;
}

ret_t url_set_port(url_t* url, int32_t port) {
  return_value_if_fail(url != NULL, RET_BAD_PARAMS);

  url->port = port;

  return RET_OK;
}

ret_t url_set_path(url_t* url, const char* path) {
  return_value_if_fail(url != NULL && path != NULL, RET_BAD_PARAMS);

  url->path = tk_str_copy(url->path, path);

  return RET_OK;
}

ret_t url_set_param(url_t* url, const char* name, const char* value) {
  return_value_if_fail(url != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);

  if (url->params == NULL) {
    url->params = object_default_create();
  }
  return_value_if_fail(url->params != NULL, RET_OOM);

  return tk_object_set_prop_str(url->params, name, value);
}

const char* url_get_param(url_t* url, const char* name) {
  return_value_if_fail(url != NULL && name != NULL, NULL);

  if (url->params == NULL) {
    return NULL;
  }

  return tk_object_get_prop_str(url->params, name);
}

ret_t url_destroy(url_t* url) {
  return_value_if_fail(url != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(url->schema);
  TKMEM_FREE(url->host);
  TKMEM_FREE(url->user_name);
  TKMEM_FREE(url->password);
  TKMEM_FREE(url->path);
  TK_OBJECT_UNREF(url->params);

  TKMEM_FREE(url);

  return RET_OK;
}
