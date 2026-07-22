/**
 * File:   xml_parse.c
 * Author: AWTK Develop Team
 * Brief:  simple xml parser
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/darray.h"
#include "xml/xml_parser.h"
#ifndef tk_isspace
#define tk_isspace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#endif
#ifndef tk_isalpha
#define tk_isalpha(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#endif
#ifndef tk_isdigit
#define tk_isdigit(c) (c >= '0' && c <= '9')
#endif

#define XML_PARSER_REPORT_ERROR_NEVER 0
#define XML_PARSER_REPORT_ERROR_ONCE 1
#define XML_PARSER_REPORT_ERROR_ALWAYS 2

struct _XmlParser {
  const char* read_ptr;
  const char* end;
  const char* start; /* XML数据起始位置，用于计算位置 */
  int attrs_nr;
  int attrs_capacity;
  char** attrs;

  char* buffer;
  int buffer_used;
  int capacity;

  XmlBuilder* builder;
  str_t text;
  bool_t trim_text;

  /* 错误追踪相关字段 */
  int line;              /* 当前行号(从1开始) */
  int col;               /* 当前列号(从1开始) */
  int last_line_start;   /* 当前行第一个字符相对 start 的偏移 */
  bool_t has_error;      /* 是否已遇到错误 */
  int report_error_mode; /* 遇到错误时报错的模式 */
  darray_t* tag_stack;   /* 标签名称栈，用于检测未闭合标签*/
};

static const char* strtrim(char* str);
static void xml_parser_parse_start_tag(XmlParser* parser);
static void xml_parser_parse_end_tag(XmlParser* parser);
static void xml_parser_parse_comment(XmlParser* parser);
static void xml_parser_parse_doctype(XmlParser* parser);
static void xml_parser_parse_pi(XmlParser* parser);
static void xml_parser_parse_text(XmlParser* parser);
static void xml_parser_reset_buffer(XmlParser* parser);
static void xml_parser_update_position(XmlParser* parser);
static void xml_parser_handle_newline(XmlParser* parser);
static bool_t xml_parser_should_report_error(XmlParser* parser);
static bool_t xml_parser_is_valid_tag_name(const char* name, int length);
static bool_t xml_parser_is_valid_attr_name(const char* name, int length);
static void xml_parser_push_tag(XmlParser* parser, const char* tag);
static void xml_parser_pop_tag(XmlParser* parser, const char* tag);
static void xml_parser_check_unclosed_tags(XmlParser* parser);
static void xml_parser_check_duplicate_attrs(XmlParser* parser, const char* name, int len);

XmlParser* xml_parser_create(void) {
  XmlParser* parser = TKMEM_ZALLOC(XmlParser);
  return_value_if_fail(parser != NULL, NULL);

  parser->trim_text = TRUE;
  str_init(&(parser->text), 100);
  parser->attrs = (char**)TKMEM_REALLOCT(char*, parser->attrs, MAX_ATTR_KEY_VALUE_NR + 1);
  parser->attrs_capacity = MAX_ATTR_KEY_VALUE_NR;

  parser->line = 1;
  parser->col = 1;
  parser->last_line_start = 0;
  parser->has_error = FALSE;
  parser->report_error_mode = XML_PARSER_REPORT_ERROR_ALWAYS;
  parser->start = NULL;
  parser->tag_stack = darray_create(32, default_destroy, NULL);

  return parser;
}

void xml_parser_set_trim_text(XmlParser* parser, bool_t trim_text) {
  return_if_fail(parser != NULL);

  parser->trim_text = trim_text;

  return;
}

void xml_parser_set_report_error_mode(XmlParser* parser, const char* mode) {
  return_if_fail(parser != NULL);

  if (tk_str_ieq(mode, "never")) {
    parser->report_error_mode = XML_PARSER_REPORT_ERROR_NEVER;
  } else if (tk_str_ieq(mode, "once")) {
    parser->report_error_mode = XML_PARSER_REPORT_ERROR_ONCE;
  } else {
    parser->report_error_mode = XML_PARSER_REPORT_ERROR_ALWAYS;
  }

  return;
}

/**
 * 判断是否应该报告错误
 */
static bool_t xml_parser_should_report_error(XmlParser* parser) {
  if (parser == NULL) {
    return FALSE;
  }

  /* NEVER模式：永远不报告错误 */
  if (parser->report_error_mode == XML_PARSER_REPORT_ERROR_NEVER) {
    return FALSE;
  }

  /* ALWAYS模式：总是报告错误 */
  if (parser->report_error_mode == XML_PARSER_REPORT_ERROR_ALWAYS) {
    return TRUE;
  }

  /* ONCE模式：只报告第一个错误 */
  return !parser->has_error;
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
  parser->start = xml;
  parser->line = 1;
  parser->col = 1;
  parser->last_line_start = 0;
  parser->has_error = FALSE;

  /* 清空标签栈 */
  if (parser->tag_stack != NULL) {
    darray_clear(parser->tag_stack);
  }

  for (; *parser->read_ptr != '\0' && (parser->read_ptr - xml) < length; parser->read_ptr++, i++) {
    char c = parser->read_ptr[0];

    /* 处理换行符 */
    if (c == '\n') {
      xml_parser_handle_newline(parser);
    } else if (c == '\r') {
      /* 处理 \r\n 或单独的 \r */
      if (parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
    } else {
      xml_parser_update_position(parser);
    }

    switch (state) {
      case STAT_NONE: {
        if (c == '<') {
          xml_parser_reset_buffer(parser);
          state = STAT_AFTER_LT;
        } else if (!(parser->trim_text) || !tk_isspace(c)) {
          state = STAT_TEXT;
        }
        break;
      }
      case STAT_AFTER_LT: {
        while (tk_isspace(c)) {
          parser->read_ptr++;
          c = parser->read_ptr[0];
        }

        if (c == '?') {
          state = STAT_PROCESS_INSTRUCTION;
        } else if (c == '/') {
          state = STAT_END_TAG;
        } else if (c == '!') {
          state = STAT_PRE_COMMENT1;
        } else if (tk_isalpha(c) || c == '_' || c == ':') {
          state = STAT_START_TAG;
        } else {
          // 检查前一个字符，避免重复报错
          if (*(parser->read_ptr - 1) == '<') {
            xml_parser_report_error_more(
                parser, "invalid character after '<': expected tag name, '/' or '?', but got '%c'",
                c);
          }
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
          // 检查前一个字符，避免重复报错
          if (*(parser->read_ptr - 1) == '!') {
            xml_parser_report_error_more(
                parser,
                "expected '-' for comment or 'D' for DOCTYPE or '[' for CDATA after '<!', "
                "but got '%c'",
                c);
          }
        }
        break;
      }
      case STAT_PRE_COMMENT2: {
        if (c == '-') {
          state = STAT_COMMENT;
        } else {
          // 检查前一个字符，避免重复报错
          if (*(parser->read_ptr - 1) == '-') {
            xml_parser_report_error_more(
                parser, "expected second '-' for comment start '<!--', but got '%c'", c);
          }
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

  /* 检查未完成的状态 */
  if (state == STAT_AFTER_LT) {
    xml_parser_report_error(parser, "unexpected end of file: incomplete tag after '<'");
  }

  /* 检查未闭合的标签 */
  xml_parser_check_unclosed_tags(parser);

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
    STAT_PRE_EQUAL,
    STAT_PRE_VALUE,
    STAT_VALUE,
    STAT_END,
  } state = STAT_PRE_KEY;

  char value_end = '\"';
  const char* start = parser->read_ptr;
  const char* key_start = NULL;
  int key_len = 0;

  parser->attrs_nr = 0;
  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    if (parser->attrs_capacity <= parser->attrs_nr) {
      parser->attrs =
          (char**)TKMEM_REALLOCT(char*, parser->attrs, (parser->attrs_capacity << 1) + 1);
      parser->attrs_capacity = parser->attrs_capacity << 1;
    }
    char c = *parser->read_ptr;

    /* 处理换行 */
    if (c == '\n' || c == '\r') {
      if (c == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
      continue;
    } else {
      xml_parser_update_position(parser);
    }

    switch (state) {
      case STAT_PRE_KEY: {
        if (c == end_char || c == '>') {
          state = STAT_END;
        } else if (!tk_isspace(c)) {
          state = STAT_KEY;
          key_start = parser->read_ptr;
        }
        break;
      }
      case STAT_KEY: {
        if (tk_isspace(c) || c == '=' || c == end_char || c == '>') {
          key_len = parser->read_ptr - key_start;

          /* 验证属性名称，非法属性名称不添加到属性列表 */
          if (key_len > 0) {
            if (!xml_parser_is_valid_attr_name(key_start, key_len)) {
              char* name = tk_strndup(key_start, key_len);
              if (name != NULL) {
                xml_parser_report_error_more(parser, "invalid attribute name: '%s'", name);
                TKMEM_FREE(name);
              } else {
                xml_parser_report_error(parser, "invalid attribute name");
              }
            } else {
              xml_parser_check_duplicate_attrs(parser, key_start, key_len);
            }
          }

          if (c == '=') {
            state = STAT_PRE_VALUE;
          } else if (c == end_char || c == '>') {
            /* 属性没有值 */
            char* name = key_len > 0 ? tk_strndup(key_start, key_len) : NULL;
            if (name != NULL) {
              xml_parser_report_error_more(parser, "attribute missing value: '%s'", name);
              TKMEM_FREE(name);
            } else {
              xml_parser_report_error(parser, "attribute missing value");
            }

            state = STAT_END;
          } else {
            state = STAT_PRE_EQUAL;
          }
        }
        break;
      }
      case STAT_PRE_EQUAL: {
        if (c == '=') {
          state = STAT_PRE_VALUE;
        } else if (!tk_isspace(c)) {
          /* 缺少等号，下一个字符直接是属性名或值 */
          xml_parser_report_error(parser, "attribute missing '=' separator");

          if (c == '\"' || c == '\'') {
            /* 直接跳到值解析 */
            state = STAT_PRE_VALUE;
            parser->read_ptr--; /* 回退让下一个状态处理引号 */
          } else {
            /* 看作新的属性名 */
            state = STAT_PRE_KEY;
            parser->read_ptr--;
          }
        }
        break;
      }
      case STAT_PRE_VALUE: {
        if (c == '\"' || c == '\'') {
          state = STAT_VALUE;
          value_end = c;
          start = parser->read_ptr + 1;
        } else if (!tk_isspace(c)) {
          /* 属性值没有用引号包裹 */
          xml_parser_report_error(parser, "attribute value must be quoted");

          /* 尝试继续解析，把当前位置当作值开始 */
          state = STAT_VALUE;
          value_end = ' '; /* 用空格作为结束标记 */
          start = parser->read_ptr;
        }
        break;
      }
      case STAT_VALUE: {
        if (c == value_end || (value_end == ' ' && (tk_isspace(c) || c == end_char || c == '>'))) {
          parser->attrs[parser->attrs_nr++] =
              tk_pointer_from_int(xml_parser_strdup(parser, key_start, key_len, TRUE));
          parser->attrs[parser->attrs_nr++] = tk_pointer_from_int(
              xml_parser_strdup(parser, start, parser->read_ptr - start, FALSE));
          state = STAT_PRE_KEY;

          if (c == end_char || c == '>') {
            /* 如果是空格结束符遇到结束字符，回退让外层处理 */
            if (value_end == ' ') {
              parser->read_ptr--;
            }
          }
        }
        break;
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
  bool_t is_self_closing = FALSE;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    switch (state) {
      case STAT_NAME: {
        if (tk_isspace(c) || c == '>' || c == '/') {
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

  /* 验证标签名称是否合法 */
  if (!xml_parser_is_valid_tag_name(tag_name, strlen(tag_name))) {
    xml_parser_report_error_more(parser, "invalid tag name: '%s'", tag_name);
  }

  xml_builder_on_start(parser->builder, tag_name, (const char**)parser->attrs);

  if (parser->read_ptr[0] == '/') {
    is_self_closing = TRUE;
    xml_builder_on_end(parser->builder, tag_name);
  }

  /* 将非自闭合标签压入栈 */
  if (!is_self_closing) {
    xml_parser_push_tag(parser, tag_name);
  }

  for (; *parser->read_ptr != '>' && *parser->read_ptr != '\0'; parser->read_ptr++) {
    /* 处理换行 */
    if (*parser->read_ptr == '\n' || *parser->read_ptr == '\r') {
      if (*parser->read_ptr == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
    } else {
      xml_parser_update_position(parser);
    }
  }

  return;
}

static void xml_parser_parse_end_tag(XmlParser* parser) {
  char* tag_name = NULL;
  const char* start = parser->read_ptr;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    /* 处理换行 */
    if (c == '\n' || c == '\r') {
      if (c == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
    } else if (c == '>') {
      int tag_len = parser->read_ptr - start;

      /* 验证结束标签内容不为空 */
      if (tag_len <= 0) {
        xml_parser_report_error(parser, "empty end tag: '</>'");
        break;
      }

      /* 去除可能的空白 */
      while (tag_len > 0 && tk_isspace(start[tag_len - 1])) {
        tag_len--;
      }

      tag_name = parser->buffer + xml_parser_strdup(parser, start, tag_len, TRUE);

      /* 验证标签名称 */
      if (!xml_parser_is_valid_tag_name(tag_name, strlen(tag_name))) {
        xml_parser_report_error_more(parser, "invalid end tag name: '%s'", tag_name);
      }

      /* 检查标签栈匹配 */
      xml_parser_pop_tag(parser, tag_name);

      xml_builder_on_end(parser->builder, tag_name);
      break;
    } else {
      xml_parser_update_position(parser);
    }
  }

  return;
}

static void xml_parser_parse_doctype(XmlParser* parser) {
  int doctype_line = parser->line;
  int doctype_col = parser->col;
  bool_t found_end = FALSE;

  /* 向后回退到 'D' 位置 */
  parser->read_ptr--;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    /* 处理换行 */
    if (c == '\n' || c == '\r') {
      if (c == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
      continue;
    } else if (c == '>') {
      found_end = TRUE;
      break;
    } else {
      xml_parser_update_position(parser);
    }
  }

  /* 检查是否找到DOCTYPE结束 */
  if (!found_end) {
    parser->line = doctype_line;
    parser->col = doctype_col;
    xml_parser_report_error(parser, "unclosed DOCTYPE declaration: missing '>'");
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
  int comment_line = parser->line;
  int comment_col = parser->col;
  bool_t found_end = FALSE;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    /* 处理换行 */
    if (c == '\n' || c == '\r') {
      if (c == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
      continue;
    } else {
      xml_parser_update_position(parser);
    }

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
          found_end = TRUE;
          return;
        } else if (c == '-') {
          /* 保持STAT_MINUS2状态，处理 '--->' 等情况 */
          ;
        } else {
          /* '--' 后面不是 '>'，可能是注释内容中的 '--' */
          state = STAT_COMMENT;
        }
        break;
      }
      default:
        break;
    }
  }

  /* 检查是否找到注释结束 */
  if (!found_end) {
    parser->line = comment_line;
    parser->col = comment_col;
    xml_parser_report_error(parser, "unclosed comment: missing '-->'");
  }

  return;
}

static void xml_parser_parse_pi(XmlParser* parser) {
  enum _State { STAT_NAME, STAT_ATTR, STAT_END } state = STAT_NAME;

  char* tag_name = NULL;
  const char* start = parser->read_ptr;
  int pi_line = parser->line;
  int pi_col = parser->col;
  bool_t found_end = FALSE;

  for (; *parser->read_ptr != '\0'; parser->read_ptr++) {
    char c = *parser->read_ptr;

    /* 处理换行 */
    if (c == '\n' || c == '\r') {
      if (c == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
      continue;
    } else {
      xml_parser_update_position(parser);
    }

    switch (state) {
      case STAT_NAME: {
        if (tk_isspace(c) || c == '?' || c == '>') {
          tag_name =
              tk_pointer_from_int(xml_parser_strdup(parser, start, parser->read_ptr - start, TRUE));

          /* 验证处理指令名称 */
          if (parser->read_ptr - start > 0 &&
              !xml_parser_is_valid_tag_name(start, parser->read_ptr - start)) {
            char* name = tk_strndup(start, parser->read_ptr - start);
            if (name != NULL) {
              xml_parser_report_error_more(parser, "invalid processing instruction name: '%s'",
                                           name);
              TKMEM_FREE(name);
            } else {
              xml_parser_report_error(parser, "invalid processing instruction name");
            }
          }

          state = tk_isspace(c) ? STAT_ATTR : STAT_END;
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

  /* 检查处理指令结束 */
  if (*parser->read_ptr == '?') {
    parser->read_ptr++;
    xml_parser_update_position(parser);
    if (*parser->read_ptr == '>') {
      found_end = TRUE;
    } else {
      xml_parser_report_error(parser, "invalid processing instruction end: expected '>' after '?'");
    }
  }

  tag_name = parser->buffer + tk_pointer_to_int(tag_name);
  xml_builder_on_pi(parser->builder, tag_name, (const char**)parser->attrs);

  for (; *parser->read_ptr != '>' && *parser->read_ptr != '\0'; parser->read_ptr++) {
    if (*parser->read_ptr == '\n' || *parser->read_ptr == '\r') {
      if (*parser->read_ptr == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
    } else {
      xml_parser_update_position(parser);
    }
  }

  if (!found_end) {
    parser->line = pi_line;
    parser->col = pi_col;
    xml_parser_report_error(parser, "unclosed processing instruction: missing '?>'");
  }

  return;
}

static void xml_parser_on_text(XmlParser* parser) {
  if (parser->text.size > 0) {
    char* start = parser->text.str;
    char* end = parser->text.str + parser->text.size - 1;

    if (parser->trim_text) {
      while (tk_isspace(*start) && *start) {
        start++;
      }

      while (tk_isspace(*end) && end > start) {
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

    /* 处理换行 */
    if (c == '\n' || c == '\r') {
      if (c == '\r' && parser->read_ptr[1] == '\n') {
        parser->read_ptr++;
      }
      xml_parser_handle_newline(parser);
      str_append_char(s, '\n');
      continue;
    } else {
      xml_parser_update_position(parser);
    }

    if (c == '<') {
      if (tk_str_start_with(parser->read_ptr, "<![CDATA[")) {
        const char* start = parser->read_ptr + 9;
        int cdata_line = parser->line;
        int cdata_col = parser->col;
        parser->read_ptr = strstr(start, "]]>");
        if (parser->read_ptr != NULL) {
          str_append_with_len(s, start, parser->read_ptr - start);
          parser->read_ptr += 2;
        } else {
          parser->line = cdata_line;
          parser->col = cdata_col;
          xml_parser_report_error(parser, "unclosed CDATA section: missing ']]>'");
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
    TKMEM_FREE(parser->attrs);

    /* 释放标签栈 */
    darray_destroy(parser->tag_stack);

    TKMEM_FREE(parser);
  }

  return;
}

static const char* strtrim(char* str) {
  char* p = NULL;

  p = str + strlen(str) - 1;

  while (p != str && tk_isspace(*p)) {
    *p = '\0';
    p--;
  }

  p = str;
  while (*p != '\0' && tk_isspace(*p)) p++;

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

/**
 * 更新解析器的行号和列号
 * 每次移动read_ptr后调用此函数
 */
static void xml_parser_update_position(XmlParser* parser) {
  if (parser->start == NULL || parser->read_ptr == NULL) {
    return;
  }

  const char* p = parser->start + parser->last_line_start;
  const char* current = parser->read_ptr;

  /* 计算当前位置的列号 */
  parser->col = (int)(current - p) + 1;
}

/**
 * 处理换行符，更新行号
 */
static void xml_parser_handle_newline(XmlParser* parser) {
  if (parser->start == NULL) {
    return;
  }

  parser->line++;
  /* +1 使其指向新行第一个字符(而非换行符本身)，保证 update_position 的列号正确 */
  parser->last_line_start = (int)(parser->read_ptr - parser->start) + 1;
  parser->col = 1;
}

void xml_parser_report_error(XmlParser* parser, const char* message) {
  if (parser == NULL || parser->builder == NULL || message == NULL) {
    return;
  }

  if (xml_parser_should_report_error(parser)) {
    parser->has_error = TRUE;
    xml_builder_on_error(parser->builder, parser->line, parser->col, message);
  }
}

void xml_parser_report_error_more(XmlParser* parser, const char* format, ...) {
  if (parser == NULL || parser->builder == NULL || format == NULL) {
    return;
  }

  if (xml_parser_should_report_error(parser)) {
    char message[128] = {0};
    va_list va;

    va_start(va, format);
    tk_vsnprintf(message, sizeof(message), format, va);
    va_end(va);

    parser->has_error = TRUE;
    xml_builder_on_error(parser->builder, parser->line, parser->col, message);
  }
}

void xml_parser_report_error_more_ex(XmlParser* parser, int line, int col, const char* format,
                                     ...) {
  if (parser == NULL || parser->builder == NULL || format == NULL) {
    return;
  }

  if (xml_parser_should_report_error(parser)) {
    char message[128] = {0};
    va_list va;

    va_start(va, format);
    tk_vsnprintf(message, sizeof(message), format, va);
    va_end(va);

    parser->has_error = TRUE;
    xml_builder_on_error(parser->builder, line, col, message);
  }
}

/**
 * 检查标签名称是否合法
 * XML标签名必须以字母或下划线开头，后面可以是字母、数字、下划线、连字符、点号
 */
static bool_t xml_parser_is_valid_tag_name(const char* name, int length) {
  int i;

  if (name == NULL || length <= 0) {
    return FALSE;
  }

  /* 第一个字符必须是字母或下划线 */
  if (!tk_isalpha(name[0]) && name[0] != '_') {
    return FALSE;
  }

  /* 后续字符检查 */
  for (i = 1; i < length; i++) {
    char c = name[i];
    if (!tk_isalpha(c) && !tk_isdigit(c) && c != '_' && c != '-' && c != '.' && c != ':') {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * 检查属性名称是否合法
 */
static bool_t xml_parser_is_valid_attr_name(const char* name, int length) {
  return xml_parser_is_valid_tag_name(name, length);
}

/**
 * 将标签压入栈
 */
static void xml_parser_push_tag(XmlParser* parser, const char* tag) {
  if (parser == NULL || tag == NULL || parser->tag_stack == NULL) {
    return;
  }

  /* 复制标签名称并压栈 */
  char* tag_copy = tk_strdup(tag);
  if (tag_copy == NULL) {
    xml_parser_report_error(parser, "failed to allocate memory for tag name");
    return;
  }

  darray_push(parser->tag_stack, tag_copy);

  return;
}

/**
 * 从栈中弹出标签并验证匹配
 */
static void xml_parser_pop_tag(XmlParser* parser, const char* tag) {
  if (parser == NULL || tag == NULL || parser->tag_stack == NULL) {
    return;
  }

  if (parser->tag_stack->size <= 0) {
    xml_parser_report_error(parser, "extra end tag");
    return;
  }

  char* expected_tag = (char*)darray_tail(parser->tag_stack);
  if (!tk_str_eq(expected_tag, tag)) {
    xml_parser_report_error_more(parser, "mismatched end tag: expected '</%s>' but found '</%s>'",
                                 expected_tag, tag);
  }

  darray_pop(parser->tag_stack);
  return;
}

/**
 * 检查是否有未闭合的标签
 */
static void xml_parser_check_unclosed_tags(XmlParser* parser) {
  if (parser == NULL || parser->tag_stack == NULL) {
    return;
  }

  while (parser->tag_stack->size > 0) {
    char* unclosed_tag = (char*)darray_tail(parser->tag_stack);
    xml_parser_report_error_more(parser, "unclosed tag: '%s' was never closed", unclosed_tag);
    darray_pop(parser->tag_stack);
  }
}

static void xml_parser_check_duplicate_attrs(XmlParser* parser, const char* key_start, int len) {
  int i;

  if (parser == NULL || parser->attrs_nr <= 0) {
    return;
  }

  for (i = 0; i < parser->attrs_nr; i += 2) {
    const char* stored = parser->buffer + tk_pointer_to_int(parser->attrs[i]);
    if (strlen(stored) == (size_t)len && tk_str_eq_with_len(stored, key_start, len)) {
      const char* p = parser->start + parser->last_line_start;
      /* 计算 key_start 的列号 */
      int col = (int)(key_start - p) + 1;
      xml_parser_report_error_more_ex(parser, parser->line, col, "duplicate attribute: '%s'", stored);
      return;
    }
  }

  return;
}

void xml_parser_parse_file(XmlParser* parser, const char* filename) {
  char* buff = read_text_file(filename);
  return_if_fail(buff != NULL);
  xml_parser_parse(parser, buff, strlen(buff));
  TKMEM_FREE(buff);

  return;
}
