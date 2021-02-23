/**
 * File:   xml_parse.c
 * Author: AWTK Develop Team
 * Brief:  simple xml parser
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
static void xml_parser_parse_start_tag(XmlParser* parser);
static void xml_parser_parse_end_tag(XmlParser* parser);
static void xml_parser_parse_comment(XmlParser* parser);
static void xml_parser_parse_doctype(XmlParser* parser);
static void xml_parser_parse_pi(XmlParser* parser);
static void xml_parser_parse_text(XmlParser* parser);
static void xml_parser_reset_buffer(XmlParser* parser);

XmlParser* xml_parser_create(void) {
  XmlParser* parser = TKMEM_ZALLOC(XmlParser);
  return_value_if_fail(parser != NULL, NULL);

  parser->trim_text = TRUE;
  str_init(&(parser->text), 100);

  return parser;
}

void xml_parser_set_trim_text(XmlParser* parser, bool_t trim_text) {
  return_if_fail(parser != NULL);

  parser->trim_text = trim_text;

  return;
}

void xml_parser_set_builder(XmlParser* parser, XmlBuilder* builder) {
  return_if_fail(parser != NULL);

  parser->builder = builder;

  return;
}

void xml_parser_parse(XmlParser* parser, const char* xml, int length) {
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

  parser->read_ptr = xml;
  parser->end = xml + length;

  for (; *parser->read_ptr != '\0' && (parser->read_ptr - xml) < length; parser->read_ptr++, i++) {
    char c = parser->read_ptr[0];

    switch (state) {
      case STAT_NONE: {
        if (c == '<') {
          xml_parser_reset_buffer(parser);
          state = STAT_AFTER_LT;
        } else if (!(parser->trim_text) || !isspace(c)) {
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
          xml_builder_on_error(parser->builder, 0, 0, "unexpected char");
        }
        break;
      }
      case STAT_START_TAG: {
        xml_parser_parse_start_tag(parser);
        state = STAT_NONE;
        break;
      }
      case STAT_END_TAG: {
        xml_parser_parse_end_tag(parser);
        state = STAT_NONE;
        break;
      }
      case STAT_PROCESS_INSTRUCTION: {
        xml_parser_parse_pi(parser);
        state = STAT_NONE;
        break;
      }
      case STAT_TEXT: {
        xml_parser_parse_text(parser);
        state = STAT_NONE;
        break;
      }
      case STAT_PRE_COMMENT1: {
        if (c == '-') {
          state = STAT_PRE_COMMENT2;
        } else if (c == 'D' || c == 'd') {
          state = STAT_DOCTYPE;
        } else if (c == '[' && tk_str_start_with(parser->read_ptr, "[CDATA[")) {
          parser->read_ptr--;
          xml_parser_parse_text(parser);
          state = STAT_NONE;
        } else {
          xml_builder_on_error(parser->builder, 0, 0, "expected \'-\'");
        }
        break;
      }
      case STAT_PRE_COMMENT2: {
        if (c == '-') {
          state = STAT_COMMENT;
        } else {
          xml_builder_on_error(parser->builder, 0, 0, "expected \'-\'");
        }
      }
      case STAT_COMMENT: {
        xml_parser_parse_comment(parser);
        state = STAT_NONE;
        break;
      }
      case STAT_DOCTYPE: {
        xml_parser_parse_doctype(parser);
        state = STAT_NONE;
        break;
      }
      default:
        break;
    }

    if (*parser->read_ptr == '\0') {
      break;
    }
  }

  return;
}

static void xml_parser_reset_buffer(XmlParser* parser) {
  parser->buffer_used = 0;
  parser->attrs_nr = 0;
  parser->attrs[0] = NULL;

  return;
}

static int xml_parser_strdup(XmlParser* parser, const char* start, int length, bool_t trim) {
  int offset = -1;

  if ((parser->buffer_used + length) >= parser->capacity) {
    int new_capacity = parser->capacity + (parser->capacity >> 1) + length + 32;
    char* buffer = (char*)TKMEM_REALLOCT(char, parser->buffer, new_capacity);
    if (buffer != NULL) {
      parser->buffer = buffer;
      parser->capacity = new_capacity;
    }
  }

  if ((parser->buffer_used + length) >= parser->capacity) {
    return offset;
  }

  offset = parser->buffer_used;
  strncpy(parser->buffer + offset, start, length);
  parser->buffer[offset + length] = '\0';
  if (trim) {
    strtrim(parser->buffer + offset);
  }
  parser->buffer_used += length + 1;

  return offset;
}

static void xml_parser_parse_attrs(XmlParser* parser, char end_char) {
  int i = 0;
  enum _State {
    STAT_PRE_KEY,
    STAT_KEY,
    STAT_PRE_VALUE,
    STAT_VALUE,
    STAT_END,
  } state = STAT_PRE_KEY;

  char value_end = '\"';
  const char* start = parser->read_ptr;

  parser->attrs_nr = 0;
  for (; *parser->read_ptr != '\0' && parser->attrs_nr < MAX_ATTR_KEY_VALUE_NR;
       parser->read_ptr++) {
    char c = *parser->read_ptr;

    switch (state) {
      case STAT_PRE_KEY: {
        if (c == end_char || c == '>') {
          state = STAT_END;
        } else if (!isspace(c)) {
          state = STAT_KEY;
          start = parser->read_ptr;
        }
      }
      case STAT_KEY: {
        if (c == '=') {
          parser->attrs[parser->attrs_nr++] =
              tk_pointer_from_int(xml_parser_strdup(parser, start, parser->read_ptr - start, TRUE));
          state = STAT_PRE_VALUE;
        }

        break;
      }
      case STAT_PRE_VALUE: {
        if (c == '\"' || c == '\'') {
          state = STAT_VALUE;
          value_end = c;
          start = parser->read_ptr + 1;
        }
        break;
      }
      case STAT_VALUE: {
        if (c == value_end) {
          parser->attrs[parser->attrs_nr++] = tk_pointer_from_int(
              xml_parser_strdup(parser, start, parser->read_ptr - start, FALSE));
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

  for (i = 0; i < parser->attrs_nr; i++) {
    parser->attrs[i] = parser->buffer + tk_pointer_to_int(parser->attrs[i]);
  }
  parser->attrs[parser->attrs_nr] = NULL;

  return;
}

static void xml_parser_parse_start_tag(XmlParser* parser) {
  enum _State {
    STAT_NAME,
    STAT_ATTR,
    STAT_END,
  } state = STAT_NAME;

  char* tag_name = NULL;
  const char* start = parser->read_ptr - 1;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    switch (state) {
      case STAT_NAME: {
        if (isspace(c) || c == '>' || c == '/') {
          tag_name =
              tk_pointer_from_int(xml_parser_strdup(parser, start, parser->read_ptr - start, TRUE));
          state = (c != '>' && c != '/') ? STAT_ATTR : STAT_END;
        }
        break;
      }
      case STAT_ATTR: {
        xml_parser_parse_attrs(parser, '/');
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

  tag_name = parser->buffer + tk_pointer_to_int(tag_name);
  xml_builder_on_start(parser->builder, tag_name, (const char**)parser->attrs);

  if (parser->read_ptr[0] == '/') {
    xml_builder_on_end(parser->builder, tag_name);
  }

  for (; *parser->read_ptr != '>' && *parser->read_ptr != '\0'; parser->read_ptr++)
    ;

  return;
}

static void xml_parser_parse_end_tag(XmlParser* parser) {
  char* tag_name = NULL;
  const char* start = parser->read_ptr;
  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    if (*parser->read_ptr == '>') {
      tag_name = parser->buffer + xml_parser_strdup(parser, start, parser->read_ptr - start, TRUE);
      xml_builder_on_end(parser->builder, tag_name);

      break;
    }
  }

  return;
}

static void xml_parser_parse_doctype(XmlParser* parser) {
  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;
    if (c == '>') {
      break;
    }
  }

  return;
}

static void xml_parser_parse_comment(XmlParser* parser) {
  enum _State {
    STAT_COMMENT,
    STAT_MINUS1,
    STAT_MINUS2,
  } state = STAT_COMMENT;

  const char* start = ++parser->read_ptr;
  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

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
          xml_builder_on_comment(parser->builder, start, parser->read_ptr - start - 2);
          return;
        }
      }
      default:
        break;
    }
  }

  return;
}

static void xml_parser_parse_pi(XmlParser* parser) {
  enum _State { STAT_NAME, STAT_ATTR, STAT_END } state = STAT_NAME;

  char* tag_name = NULL;
  const char* start = parser->read_ptr;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    switch (state) {
      case STAT_NAME: {
        if (isspace(c) || c == '>') {
          tag_name =
              tk_pointer_from_int(xml_parser_strdup(parser, start, parser->read_ptr - start, TRUE));
          state = c != '>' ? STAT_ATTR : STAT_END;
        }

        break;
      }
      case STAT_ATTR: {
        xml_parser_parse_attrs(parser, '?');
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

  tag_name = parser->buffer + tk_pointer_to_int(tag_name);
  xml_builder_on_pi(parser->builder, tag_name, (const char**)parser->attrs);

  for (; *parser->read_ptr != '>' && *parser->read_ptr != '\0'; parser->read_ptr++)
    ;

  return;
}

static void xml_parser_on_text(XmlParser* parser) {
  if (parser->text.size > 0) {
    char* start = parser->text.str;
    char* end = parser->text.str + parser->text.size - 1;

    if (parser->trim_text) {
      while (isspace(*start) && *start) {
        start++;
      }

      while (isspace(*end) && end > start) {
        *end = '\0';
        end--;
      }
    }

    if (end >= start) {
      xml_builder_on_text(parser->builder, start, end - start + 1);
    } else if (start == parser->text.str + parser->text.size) {
      /* support <![CDATA[]]> grammar */
      xml_builder_on_text(parser->builder, start, 0);
    }
  }
}

static void xml_parser_parse_text(XmlParser* parser) {
  str_t* s = &(parser->text);

  s->size = 0;
  s->str[0] = '\0';
  parser->read_ptr--;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    if (c == '<') {
      if (tk_str_start_with(parser->read_ptr, "<![CDATA[")) {
        const char* start = parser->read_ptr + 9;
        parser->read_ptr = strstr(start, "]]>");
        if (parser->read_ptr != NULL) {
          str_append_with_len(s, start, parser->read_ptr - start);
          parser->read_ptr += 2;
        } else {
          log_warn("invalid cdata\n");
          parser->read_ptr = parser->end;
        }
      } else {
        parser->read_ptr--;
        break;
      }
    } else {
      str_append_char(s, c);
    }
  }

  xml_parser_on_text(parser);

  return;
}

void xml_parser_destroy(XmlParser* parser) {
  if (parser != NULL) {
    str_reset(&(parser->text));
    TKMEM_FREE(parser->buffer);
    TKMEM_FREE(parser);
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

void xml_parser_parse_file(XmlParser* parser, const char* filename) {
  char* buff = read_text_file(filename);
  return_if_fail(buff != NULL);
  xml_parser_parse(parser, buff, strlen(buff));
  TKMEM_FREE(buff);

  return;
}
