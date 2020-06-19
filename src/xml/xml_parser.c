/**
 * File:   xml_parse.c
 * Author: AWTK Develop Team
 * Brief:  simple xml parser
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/str.h"
#include "tkc/utils.h"
#include "xml/xml_parser.h"
#ifndef isspace
#define isspace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#endif
#ifndef isalpha
#define isalpha(c) ((c >= 'a' && c <= 'z') || (c >= 'A' || c <= 'Z'))
#endif

struct _XmlParser {
  const char* read_ptr;
  const char* end;
  int attrs_nr;
  char* attrs[MAX_ATTR_KEY_VALUE_NR + 1];

  char* buffer;
  int buffer_used;
  int capacity;

  XmlBuilder* builder;
  str_t text;
  bool_t trim_text;
};

static const char* strtrim(char* str);
static void xml_parser_parse_start_tag(XmlParser* thiz);
static void xml_parser_parse_end_tag(XmlParser* thiz);
static void xml_parser_parse_comment(XmlParser* thiz);
static void xml_parser_parse_doctype(XmlParser* thiz);
static void xml_parser_parse_pi(XmlParser* thiz);
static void xml_parser_parse_text(XmlParser* thiz);
static void xml_parser_reset_buffer(XmlParser* thiz);

XmlParser* xml_parser_create(void) {
  XmlParser* thiz = TKMEM_ZALLOC(XmlParser);
  return_value_if_fail(thiz != NULL, NULL);

  thiz->trim_text = TRUE;
  str_init(&(thiz->text), 100);

  return thiz;
}

void xml_parser_set_trim_text(XmlParser* thiz, bool_t trim_text) {
  return_if_fail(thiz != NULL);

  thiz->trim_text = trim_text;

  return;
}

void xml_parser_set_builder(XmlParser* thiz, XmlBuilder* builder) {
  return_if_fail(thiz != NULL);

  thiz->builder = builder;

  return;
}

void xml_parser_parse(XmlParser* thiz, const char* xml, int length) {
  int i = 0;
  enum _State {
    STAT_NONE,
    STAT_AFTER_LT,
    STAT_START_TAG,
    STAT_END_TAG,
    STAT_TEXT,
    STAT_PRE_COMMENT1,
    STAT_PRE_COMMENT2,
    STAT_COMMENT,
    STAT_DOCTYPE,
    STAT_PROCESS_INSTRUCTION,
  } state = STAT_NONE;

  thiz->read_ptr = xml;
  thiz->end = xml + length;

  for (; *thiz->read_ptr != '\0' && (thiz->read_ptr - xml) < length; thiz->read_ptr++, i++) {
    char c = thiz->read_ptr[0];

    switch (state) {
      case STAT_NONE: {
        if (c == '<') {
          xml_parser_reset_buffer(thiz);
          state = STAT_AFTER_LT;
        } else if (!(thiz->trim_text) || !isspace(c)) {
          state = STAT_TEXT;
        }
        break;
      }
      case STAT_AFTER_LT: {
        if (c == '?') {
          state = STAT_PROCESS_INSTRUCTION;
        } else if (c == '/') {
          state = STAT_END_TAG;
        } else if (c == '!') {
          state = STAT_PRE_COMMENT1;
        } else if (isalpha(c) || c == '_') {
          state = STAT_START_TAG;
        } else {
          xml_builder_on_error(thiz->builder, 0, 0, "unexpected char");
        }
        break;
      }
      case STAT_START_TAG: {
        xml_parser_parse_start_tag(thiz);
        state = STAT_NONE;
        break;
      }
      case STAT_END_TAG: {
        xml_parser_parse_end_tag(thiz);
        state = STAT_NONE;
        break;
      }
      case STAT_PROCESS_INSTRUCTION: {
        xml_parser_parse_pi(thiz);
        state = STAT_NONE;
        break;
      }
      case STAT_TEXT: {
        xml_parser_parse_text(thiz);
        state = STAT_NONE;
        break;
      }
      case STAT_PRE_COMMENT1: {
        if (c == '-') {
          state = STAT_PRE_COMMENT2;
        } else if (c == 'D' || c == 'd') {
          state = STAT_DOCTYPE;
        } else if (c == '[' && tk_str_start_with(thiz->read_ptr, "[CDATA[")) {
          thiz->read_ptr--;
          xml_parser_parse_text(thiz);
          state = STAT_NONE;
        } else {
          xml_builder_on_error(thiz->builder, 0, 0, "expected \'-\'");
        }
        break;
      }
      case STAT_PRE_COMMENT2: {
        if (c == '-') {
          state = STAT_COMMENT;
        } else {
          xml_builder_on_error(thiz->builder, 0, 0, "expected \'-\'");
        }
      }
      case STAT_COMMENT: {
        xml_parser_parse_comment(thiz);
        state = STAT_NONE;
        break;
      }
      case STAT_DOCTYPE: {
        xml_parser_parse_doctype(thiz);
        state = STAT_NONE;
        break;
      }
      default:
        break;
    }

    if (*thiz->read_ptr == '\0') {
      break;
    }
  }

  return;
}

static void xml_parser_reset_buffer(XmlParser* thiz) {
  thiz->buffer_used = 0;
  thiz->attrs_nr = 0;
  thiz->attrs[0] = NULL;

  return;
}

static int xml_parser_strdup(XmlParser* thiz, const char* start, int length, bool_t trim) {
  int offset = -1;

  if ((thiz->buffer_used + length) >= thiz->capacity) {
    int new_capacity = thiz->capacity + (thiz->capacity >> 1) + length + 32;
    char* buffer = (char*)TKMEM_REALLOCT(char, thiz->buffer, new_capacity);
    if (buffer != NULL) {
      thiz->buffer = buffer;
      thiz->capacity = new_capacity;
    }
  }

  if ((thiz->buffer_used + length) >= thiz->capacity) {
    return offset;
  }

  offset = thiz->buffer_used;
  strncpy(thiz->buffer + offset, start, length);
  thiz->buffer[offset + length] = '\0';
  if (trim) {
    strtrim(thiz->buffer + offset);
  }
  thiz->buffer_used += length + 1;

  return offset;
}

static void xml_parser_parse_attrs(XmlParser* thiz, char end_char) {
  int i = 0;
  enum _State {
    STAT_PRE_KEY,
    STAT_KEY,
    STAT_PRE_VALUE,
    STAT_VALUE,
    STAT_END,
  } state = STAT_PRE_KEY;

  char value_end = '\"';
  const char* start = thiz->read_ptr;

  thiz->attrs_nr = 0;
  for (; *thiz->read_ptr != '\0' && thiz->attrs_nr < MAX_ATTR_KEY_VALUE_NR; thiz->read_ptr++) {
    char c = *thiz->read_ptr;

    switch (state) {
      case STAT_PRE_KEY: {
        if (c == end_char || c == '>') {
          state = STAT_END;
        } else if (!isspace(c)) {
          state = STAT_KEY;
          start = thiz->read_ptr;
        }
      }
      case STAT_KEY: {
        if (c == '=') {
          thiz->attrs[thiz->attrs_nr++] =
              tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start, TRUE));
          state = STAT_PRE_VALUE;
        }

        break;
      }
      case STAT_PRE_VALUE: {
        if (c == '\"' || c == '\'') {
          state = STAT_VALUE;
          value_end = c;
          start = thiz->read_ptr + 1;
        }
        break;
      }
      case STAT_VALUE: {
        if (c == value_end) {
          thiz->attrs[thiz->attrs_nr++] =
              tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start, FALSE));
          state = STAT_PRE_KEY;
        }
      }
      default:
        break;
    }

    if (state == STAT_END) {
      break;
    }
  }

  for (i = 0; i < thiz->attrs_nr; i++) {
    thiz->attrs[i] = thiz->buffer + tk_pointer_to_int(thiz->attrs[i]);
  }
  thiz->attrs[thiz->attrs_nr] = NULL;

  return;
}

static void xml_parser_parse_start_tag(XmlParser* thiz) {
  enum _State {
    STAT_NAME,
    STAT_ATTR,
    STAT_END,
  } state = STAT_NAME;

  char* tag_name = NULL;
  const char* start = thiz->read_ptr - 1;

  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    char c = *thiz->read_ptr;

    switch (state) {
      case STAT_NAME: {
        if (isspace(c) || c == '>' || c == '/') {
          tag_name =
              tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start, TRUE));
          state = (c != '>' && c != '/') ? STAT_ATTR : STAT_END;
        }
        break;
      }
      case STAT_ATTR: {
        xml_parser_parse_attrs(thiz, '/');
        state = STAT_END;

        break;
      }
      default:
        break;
    }

    if (state == STAT_END) {
      break;
    }
  }

  tag_name = thiz->buffer + tk_pointer_to_int(tag_name);
  xml_builder_on_start(thiz->builder, tag_name, (const char**)thiz->attrs);

  if (thiz->read_ptr[0] == '/') {
    xml_builder_on_end(thiz->builder, tag_name);
  }

  for (; *thiz->read_ptr != '>' && *thiz->read_ptr != '\0'; thiz->read_ptr++)
    ;

  return;
}

static void xml_parser_parse_end_tag(XmlParser* thiz) {
  char* tag_name = NULL;
  const char* start = thiz->read_ptr;
  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    if (*thiz->read_ptr == '>') {
      tag_name = thiz->buffer + xml_parser_strdup(thiz, start, thiz->read_ptr - start, TRUE);
      xml_builder_on_end(thiz->builder, tag_name);

      break;
    }
  }

  return;
}

static void xml_parser_parse_doctype(XmlParser* thiz) {
  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    char c = *thiz->read_ptr;
    if (c == '>') {
      break;
    }
  }

  return;
}

static void xml_parser_parse_comment(XmlParser* thiz) {
  enum _State {
    STAT_COMMENT,
    STAT_MINUS1,
    STAT_MINUS2,
  } state = STAT_COMMENT;

  const char* start = ++thiz->read_ptr;
  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    char c = *thiz->read_ptr;

    switch (state) {
      case STAT_COMMENT: {
        if (c == '-') {
          state = STAT_MINUS1;
        }
        break;
      }
      case STAT_MINUS1: {
        if (c == '-') {
          state = STAT_MINUS2;
        } else {
          state = STAT_COMMENT;
        }
        break;
      }
      case STAT_MINUS2: {
        if (c == '>') {
          xml_builder_on_comment(thiz->builder, start, thiz->read_ptr - start - 2);
          return;
        }
      }
      default:
        break;
    }
  }

  return;
}

static void xml_parser_parse_pi(XmlParser* thiz) {
  enum _State { STAT_NAME, STAT_ATTR, STAT_END } state = STAT_NAME;

  char* tag_name = NULL;
  const char* start = thiz->read_ptr;

  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    char c = *thiz->read_ptr;

    switch (state) {
      case STAT_NAME: {
        if (isspace(c) || c == '>') {
          tag_name =
              tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start, TRUE));
          state = c != '>' ? STAT_ATTR : STAT_END;
        }

        break;
      }
      case STAT_ATTR: {
        xml_parser_parse_attrs(thiz, '?');
        state = STAT_END;
        break;
      }
      default:
        break;
    }

    if (state == STAT_END) {
      break;
    }
  }

  tag_name = thiz->buffer + tk_pointer_to_int(tag_name);
  xml_builder_on_pi(thiz->builder, tag_name, (const char**)thiz->attrs);

  for (; *thiz->read_ptr != '>' && *thiz->read_ptr != '\0'; thiz->read_ptr++)
    ;

  return;
}

static void xml_parser_on_text(XmlParser* thiz) {
  if (thiz->text.size > 0) {
    char* start = thiz->text.str;
    char* end = thiz->text.str + thiz->text.size - 1;

    if (thiz->trim_text) {
      while (isspace(*start) && *start) {
        start++;
      }

      while (isspace(*end) && end > start) {
        *end = '\0';
        end--;
      }
    }

    if (end >= start) {
      xml_builder_on_text(thiz->builder, start, end - start + 1);
    }
  }
}

static void xml_parser_parse_text(XmlParser* thiz) {
  str_t* s = &(thiz->text);

  s->size = 0;
  s->str[0] = '\0';
  thiz->read_ptr--;

  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    char c = *thiz->read_ptr;

    if (c == '<') {
      if (tk_str_start_with(thiz->read_ptr, "<![CDATA[")) {
        const char* start = thiz->read_ptr + 9;
        thiz->read_ptr = strstr(start, "]]>");
        if (thiz->read_ptr != NULL) {
          str_append_with_len(s, start, thiz->read_ptr - start);
          thiz->read_ptr += 2;
        } else {
          log_warn("invalid cdata\n");
          thiz->read_ptr = thiz->end;
        }
      } else {
        thiz->read_ptr--;
        break;
      }
    } else {
      str_append_char(s, c);
    }
  }

  xml_parser_on_text(thiz);

  return;
}

void xml_parser_destroy(XmlParser* thiz) {
  if (thiz != NULL) {
    str_reset(&(thiz->text));
    TKMEM_FREE(thiz->buffer);
    TKMEM_FREE(thiz);
  }

  return;
}

static const char* strtrim(char* str) {
  char* p = NULL;

  p = str + strlen(str) - 1;

  while (p != str && isspace(*p)) {
    *p = '\0';
    p--;
  }

  p = str;
  while (*p != '\0' && isspace(*p)) p++;

  if (p != str) {
    char* s = p;
    char* d = str;
    while (*s != '\0') {
      *d = *s;
      d++;
      s++;
    }
    *d = '\0';
  }

  return str;
}

static char* read_text_file(const char* filename) {
  uint32_t size = 0;

  return (char*)file_read(filename, &size);
}

void xml_parser_parse_file(XmlParser* thiz, const char* filename) {
  char* buff = read_text_file(filename);
  return_if_fail(buff != NULL);
  xml_parser_parse(thiz, buff, strlen(buff));
  TKMEM_FREE(buff);

  return;
}
