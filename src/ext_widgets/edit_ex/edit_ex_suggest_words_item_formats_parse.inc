/**
 * File:   edit_ex_suggest_words_item_formats_parse.inc
 * Author: AWTK Develop Team
 * Brief:  edit_ex输入建议词的项格式解析
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-02-18 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_EDIT_EX_SUGGEST_WORDS_ITEM_FORMATS_PARSE_INC
#define TK_EDIT_EX_SUGGEST_WORDS_ITEM_FORMATS_PARSE_INC

#include "tkc/types_def.h"

BEGIN_C_DECLS

#include "ext_widgets/edit_ex/edit_ex.h"

#include "tkc/darray.h"
#include "tkc/tokenizer.h"
#include "base/widget_factory.h"
#include "widgets/label.h"
#include "widgets/view.h"
#include "widgets/combo_box_item.h"

/**
 * @const EDIT_EX_SUGGEST_WORDS_ITEM_FORMAT_PREFIX
 * 属性名前加上该前缀表示模型中的属性需要格式化。
 */
#define EDIT_EX_SUGGEST_WORDS_ITEM_FORMAT_PREFIX "@edit_ex$"

static int model_items_format_name_compare(widget_t* iter, const char* name) {
  const char* iter_name = widget_get_prop_str(iter, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, NULL);
  return tk_str_cmp(iter_name, name);
}

inline static widget_t* edit_ex_suggest_words_model_items_find(darray_t* model_items,
                                                               const char* name) {
  if (TK_STR_IS_EMPTY(name)) {
    name = "[0]";
  }
  if (tk_str_indexable(name)) {
    int32_t index = tk_atoi(name + 1);
    return (widget_t*)darray_get(model_items, index);
  } else {
    return (widget_t*)darray_find_ex(model_items, (tk_compare_t)model_items_format_name_compare,
                                     (void*)name);
  }
}

inline static widget_t* edit_ex_suggest_words_create_item(edit_ex_t* edit_ex, widget_t* parent) {
  widget_t* ret = NULL;
  return_value_if_fail(edit_ex != NULL, NULL);

  ret = combo_box_item_create(parent, 0, 0, 0, 0);
  return_value_if_fail(ret != NULL, NULL);

  widget_set_children_layout(ret, "default(r=1,c=0)");
  widget_set_sync_state_to_children(ret, TRUE);

  return ret;
}

inline static widget_t* edit_ex_suggest_words_create_separator(edit_ex_t* edit_ex,
                                                               widget_t* parent) {
  widget_t* ret = NULL;
  const char* style = NULL;
  char param[128] = {'\0'};
  return_value_if_fail(edit_ex != NULL && edit_ex->suggest_words_ui_props != NULL, NULL);

  ret = view_create(parent, 0, 0, 1, 0);
  widget_set_name(ret, "@separator");

  tk_snprintf(param, sizeof(param) - 1, "default(w=%d,h=100%%)", ret->w);
  widget_set_self_layout(ret, param);

  style = tk_object_get_prop_str(edit_ex->suggest_words_ui_props,
                                 EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW
                                 "." EDIT_EX_PROP_SUGGEST_WORDS_ITEM_SEPARATE_STYLE);
  if (style != NULL) {
    widget_use_style(ret, style);
  }
  return ret;
}

inline static widget_t* edit_ex_suggest_words_create_separator_view(edit_ex_t* edit_ex,
                                                                    widget_t* parent) {
  widget_t* ret = view_create(parent, 0, 0, 0, 0);
  (void)edit_ex;
  widget_set_name(ret, "@separator_view");
  /* 透明的 view */
  widget_set_style_color(ret, STYLE_ID_BG_COLOR, color_init(0, 0, 0, 0).color);
  widget_set_style_color(ret, STYLE_ID_BORDER_COLOR, color_init(0, 0, 0, 0).color);
  widget_set_children_layout(ret, "default(r=1,c=0)");
  return ret;
}

inline static const char* edit_ex_suggest_words_find_pair_char(const char* str, char rep_char,
                                                               char pair_char) {
  const char* ret = str;
  uint32_t need_num = 1;

  do {
    if ('\0' == *ret) {
      return NULL;
    }
    if (rep_char == *ret) {
      ++need_num;
    } else if (pair_char == *ret) {
      --need_num;
    }
    if (0 == need_num) {
      break;
    }
  } while (++ret);

  return ret;
}

inline static const char* edit_ex_suggest_words_set_str_by_slice(str_t* str, const char* start,
                                                                 const char* end,
                                                                 const char* trim_text) {
  return_value_if_fail(str != NULL, NULL);
  return_value_if_fail(start != NULL && end != NULL && end >= start, NULL);

  if (TK_STR_IS_NOT_EMPTY(trim_text)) {
    const char* p = start;
    while (p < end && strchr(trim_text, *p) != NULL) {
      p++;
    }
    start = p;

    p = end - 1;
    while (p + 1 > start && strchr(trim_text, *p) != NULL) {
      p--;
    }
    end = p + 1;
  }

  return_value_if_fail(RET_OK == str_set_with_len(str, start, end - start), NULL);

  return str->str;
}

inline static widget_t* edit_ex_suggest_words_widget_create_by_type(widget_t* parent,
                                                                    const char* type) {
  widget_t* ret = NULL;
  if (TK_STR_IS_NOT_EMPTY(type)) {
    ret = widget_factory_create_widget(widget_factory(), type, parent, 0, 0, 0, 0);
  } else { /* 默认类型为label */
    ret = label_create(parent, 0, 0, 0, 0);
  }
  return ret;
}

static ret_t edit_ex_suggest_words_widget_props_format_parse(edit_ex_t* edit_ex, const char* format,
                                                             widget_t* widget) {
  ret_t ret = RET_OK;
  const char* prop_format = NULL;
  str_t name;
  str_t value;
  tokenizer_t t;
  tokenizer_init(&t, format, UINT32_MAX, ",");

  str_init(&name, 0);
  str_init(&value, 0);

  for (prop_format = tokenizer_next(&t); prop_format != NULL; prop_format = tokenizer_next(&t)) {
    const char* iter_eq = strchr(prop_format, '=');
    break_if_fail_ex(iter_eq != NULL, ret = RET_BAD_PARAMS);

    break_if_fail_ex(
        NULL != edit_ex_suggest_words_set_str_by_slice(&name, prop_format, iter_eq, " \r\n"),
        ret = RET_FAIL);
    str_set(&value, iter_eq + 1);
    str_trim(&value, " \r\n");
    if (name.size > 0 && value.size > 0) {
      if ('$' == value.str[0]) {
        str_insert(&name, 0, EDIT_EX_SUGGEST_WORDS_ITEM_FORMAT_PREFIX);
        str_remove(&value, 0, 1); /* 去掉 $ */
      }
      widget_set_prop_str(widget, name.str, value.str);
    }
  }

  str_reset(&value);
  str_reset(&name);

  tokenizer_deinit(&t);

  return ret;
}

static ret_t edit_ex_suggest_words_item_format_parse(edit_ex_t* edit_ex, const char* item_format,
                                                     widget_t* parent) {
  ret_t ret = RET_OK;
  const char* p = item_format;
  const char* last_p = item_format;
  widget_t* curr_item = NULL;
  widget_t* curr_parent = parent;
  str_t slice;
  str_init(&slice, 0);

  while (*p != '\0') {
    switch (*p) {
      case '(': {
        if (NULL == curr_item) {
          const char* type = edit_ex_suggest_words_set_str_by_slice(&slice, last_p, p, " \r\n");
          goto_error_if_fail_ex(type != NULL, ret = RET_FAIL);
          curr_item = edit_ex_suggest_words_widget_create_by_type(curr_parent, type);
          goto_error_if_fail_ex(curr_item != NULL, ret = RET_FAIL);
        }

        last_p = ++p;
        p = edit_ex_suggest_words_find_pair_char(p, '(', ')');
        goto_error_if_fail_ex(p != NULL, ret = RET_FAIL);

        {
          const char* props_format =
              edit_ex_suggest_words_set_str_by_slice(&slice, last_p, p, " \r\n");
          goto_error_if_fail_ex(props_format != NULL, ret = RET_FAIL);
          ret = edit_ex_suggest_words_widget_props_format_parse(edit_ex, props_format, curr_item);
          goto_error_if_fail(RET_OK == ret);
        }

        if (curr_parent != parent) { /* 分隔线布局 */
          const char* layout = self_layouter_to_string(curr_item->self_layout);
          if (layout != NULL) {
            widget_set_self_layout(curr_parent, layout);
          } else {
            int32_t w = widget_get_prop_int(curr_item, WIDGET_PROP_W, 0);
            widget_set_prop_int(curr_parent, WIDGET_PROP_W, w);
          }
          widget_set_self_layout_params(curr_item, NULL, NULL, "-1", NULL); /* 减去分隔线宽度 */
        }

        last_p = ++p;
      } break;
      case '[': {
        if (NULL == curr_item) {
          const char* type = edit_ex_suggest_words_set_str_by_slice(&slice, last_p, p, " \r\n");
          goto_error_if_fail_ex(type != NULL, ret = RET_FAIL);
          curr_item = edit_ex_suggest_words_widget_create_by_type(curr_parent, type);
          goto_error_if_fail_ex(curr_item != NULL, ret = RET_FAIL);
        }

        last_p = ++p;
        p = edit_ex_suggest_words_find_pair_char(p, '[', ']');
        goto_error_if_fail_ex(p != NULL, ret = RET_FAIL);

        {
          const char* subitem_format =
              edit_ex_suggest_words_set_str_by_slice(&slice, last_p, p, " \r\n");
          goto_error_if_fail_ex(subitem_format != NULL, ret = RET_FAIL);
          ret = edit_ex_suggest_words_item_format_parse(edit_ex, subitem_format, curr_item);
          goto_error_if_fail(RET_OK == ret);
        }

        last_p = ++p;
      } break;
      case ',': {
        curr_item = NULL;
        curr_parent = parent;

        last_p = ++p;
      } break;
      case '|': {
        curr_item = NULL;
        curr_parent = edit_ex_suggest_words_create_separator_view(edit_ex, parent);
        edit_ex_suggest_words_create_separator(edit_ex, curr_parent);

        last_p = ++p;
      } break;
      default: {
        ++p;
      } break;
    }
  }

error:
  str_reset(&slice);
  return ret;
}

static ret_t edit_ex_suggest_words_item_formats_parse(edit_ex_t* edit_ex, const char* item_formats,
                                                      darray_t* result) {
  const char* item_format = NULL;
  ret_t ret = RET_OK;
  uint32_t i = 0;
  tokenizer_t t;
  str_t str_format;
  return_value_if_fail(item_formats != NULL && result != NULL, RET_BAD_PARAMS);

  tokenizer_init(&t, item_formats, UINT32_MAX, ";");
  str_init(&str_format, 0);

  for (item_format = tokenizer_next(&t), i = 0; item_format != NULL;
       item_format = tokenizer_next(&t), i++) {
    uint32_t len = tk_strlen(item_format);
    const char* brace_left = strchr(item_format, '{');
    if (brace_left != NULL && item_format[len - 1] == '}') {
      widget_t* item = NULL;

      /* 设置 { 前的名称 */
      break_if_fail_ex(NULL != edit_ex_suggest_words_set_str_by_slice(&str_format, item_format,
                                                                      brace_left, " \r\n"),
                       ret = RET_FAIL);

      /* 避免与索引产生冲突 */
      break_if_fail_ex(!tk_str_indexable(str_format.str), ret = RET_FAIL);

      if (0 == str_format.size) {
        str_append_char(&str_format, '[');
        str_append_uint32(&str_format, i);
        str_append_char(&str_format, ']');
      }

      item = edit_ex_suggest_words_create_item(edit_ex, NULL);
      break_if_fail_ex(item != NULL, ret = RET_OOM);

      widget_set_prop_str(item, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, str_format.str);
      widget_set_name(item, str_format.str);

      /* 去掉 {} 和名称 */
      break_if_fail_ex(NULL != edit_ex_suggest_words_set_str_by_slice(
                                   &str_format, brace_left + 1, &item_format[len - 2], " \r\n"),
                       (widget_destroy(item), ret = RET_FAIL));

      ret = edit_ex_suggest_words_item_format_parse(edit_ex, str_format.str, item);
      break_if_fail_ex(RET_OK == ret, widget_destroy(item));

      ret = darray_push(result, item);
      break_if_fail_ex(RET_OK == ret, widget_destroy(item));
    } else {
      log_debug("%s:fail to parse item_format:\"%s\"!\n", __FUNCTION__, item_format);
    }
  }

  str_reset(&str_format);
  tokenizer_deinit(&t);

  return ret;
}

END_C_DECLS

#endif /*TK_EDIT_EX_SUGGEST_WORDS_ITEM_FORMATS_PARSE_INC*/
