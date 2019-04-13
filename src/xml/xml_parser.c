/**
 * File:   xml_parse.c
 * Author: AWTK Develop Team
 * Brief:  simple xml parser
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

  int attrs_nr;
  char* attrs[2 * MAX_ATTR_NR + 1];

  char* buffer;
  int buffer_used;
  int capacity;

  XmlBuilder* builder;
};

static const char* strtrim(char* str);
static void xml_parser_parse_entity(XmlParser* thiz);
static void xml_parser_parse_start_tag(XmlParser* thiz);
static void xml_parser_parse_end_tag(XmlParser* thiz);
static void xml_parser_parse_comment(XmlParser* thiz);
static void xml_parser_parse_doctype(XmlParser* thiz);
static void xml_parser_parse_pi(XmlParser* thiz);
static void xml_parser_parse_text(XmlParser* thiz);
static void xml_parser_reset_buffer(XmlParser* thiz);

XmlParser* xml_parser_create(void) {
  return TKMEM_ZALLOC(XmlParser);
}

void xml_parser_set_builder(XmlParser* thiz, XmlBuilder* builder) {
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

  for (; *thiz->read_ptr != '\0' && (thiz->read_ptr - xml) < length; thiz->read_ptr++, i++) {
    char c = thiz->read_ptr[0];

    switch (state) {
      case STAT_NONE: {
        if (c == '<') {
          xml_parser_reset_buffer(thiz);
          state = STAT_AFTER_LT;
        } else if (!isspace(c)) {
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

static int xml_parser_strdup(XmlParser* thiz, const char* start, int length) {
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
  strtrim(thiz->buffer + offset);
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
  for (; *thiz->read_ptr != '\0' && thiz->attrs_nr < MAX_ATTR_NR; thiz->read_ptr++) {
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
              tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start));
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
              tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start));
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
          tag_name = tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start));
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
      tag_name = thiz->buffer + xml_parser_strdup(thiz, start, thiz->read_ptr - start);
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
          tag_name = tk_pointer_from_int(xml_parser_strdup(thiz, start, thiz->read_ptr - start));
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

static void xml_parser_parse_text(XmlParser* thiz) {
  const char* start = thiz->read_ptr - 1;
  for (; *thiz->read_ptr != '\0'; thiz->read_ptr++) {
    char c = *thiz->read_ptr;

    if (c == '<') {
      if (thiz->read_ptr > start) {
        xml_builder_on_text(thiz->builder, start, thiz->read_ptr - start);
      }
      thiz->read_ptr--;
      return;
    } else if (c == '&') {
      xml_parser_parse_entity(thiz);
    }
  }

  if (thiz->read_ptr > start) {
    xml_builder_on_text(thiz->builder, start, thiz->read_ptr - start);
  }

  return;
}

static void xml_parser_parse_entity(XmlParser* thiz) {
  /*TODO*/
  (void)thiz;
  return;
}

void xml_parser_destroy(XmlParser* thiz) {
  if (thiz != NULL) {
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
