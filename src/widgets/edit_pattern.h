#include "widgets/edit.h"

#ifndef EDIT_PATTERN_H
#define EDIT_PATTERN_H

typedef uint32_t (*pattern_part_get_max_len_t)(uint32_t index);
typedef bool_t (*pattern_part_len_is_valid_t)(uint32_t index, uint32_t len);
typedef bool_t (*pattern_part_value_is_valid_t)(uint32_t index, int32_t value);
typedef wchar_t* (*pattern_part_fix_t)(uint32_t index, int32_t value, wchar_t* start, wchar_t* end);

static bool_t edit_pattern_is_valid(widget_t* widget, wchar_t sep, uint32_t sep_nr,
                                    pattern_part_len_is_valid_t is_valid_len,
                                    pattern_part_value_is_valid_t is_valid_value) {
  uint32_t i = 0;
  const wchar_t* ps = NULL;
  const wchar_t* pe = NULL;
  wstr_t* text = &(widget->text);

  if (tk_wstr_count_c(text->str, sep) != sep_nr) {
    return FALSE;
  }

  ps = text->str;
  pe = wcs_chr(ps, sep);

  do {
    int v = 0;
    int len = pe - ps;
    if (!is_valid_len(i, len)) {
      return FALSE;
    }

    v = tk_watoi_n(ps, pe - ps);
    if (!(is_valid_value(i, v))) {
      return FALSE;
    }

    if (i >= sep_nr) {
      break;
    }

    i++;
    ps = pe + 1;
    pe = wcs_chr(ps, sep);
    if (pe == NULL) {
      pe = ps + wcslen(ps);
    }
  } while (TRUE);

  return TRUE;
}

static ret_t edit_pattern_fix_ex(widget_t* widget, bool_t strict, const char* defval, wchar_t sep,
                                 uint32_t sep_nr, pattern_part_get_max_len_t get_part_max_len,
                                 pattern_part_fix_t fix) {
  uint32_t i = 0;
  wchar_t str[32];
  wchar_t* p = str;
  wchar_t* pd = str;
  const wchar_t* ps = NULL;
  const wchar_t* pe = NULL;
  wstr_t* text = &(widget->text);

  memset(str, 0x00, sizeof(str));
  if (tk_wstr_count_c(text->str, sep) != sep_nr) {
    widget_set_text_utf8(widget, defval);
    edit_set_cursor(widget, 0);

    return RET_OK;
  }

  ps = text->str;
  pe = wcs_chr(ps, sep);

  do {
    int v = 0;
    while (ps != pe && (p - pd) < get_part_max_len(i)) {
      *p++ = *ps++;
    }

    if (strict) {
      v = tk_watoi_n(pd, p - pd);
      p = fix(i, v, pd, p);
    }

    if (i == sep_nr) {
      *p = 0;
      break;
    } else {
      *p++ = sep;
    }

    i++;
    pd = p;
    ps = pe + 1;
    pe = wcs_chr(ps, sep);
    if (pe == NULL) {
      pe = ps + wcslen(ps);
    }
  } while (TRUE);

  widget_set_text(widget, str);

  return RET_OK;
}

static bool_t edit_pattern_is_valid_char(widget_t* widget, wchar_t c, wchar_t sep,
                                         pattern_part_get_max_len_t get_part_max_len) {
  if ((c >= '0' && c <= '9') || c == sep) {
    uint32_t cursor = 0;
    text_edit_state_t state;
    edit_t* edit = EDIT(widget);
    wstr_t* text = &(widget->text);
    text_edit_get_state(edit->model, &state);

    cursor = state.cursor <= text->size ? state.cursor : text->size;

    if (c == sep) {
      if (text->str[cursor] == sep) {
        /*如果时sep，而且当前字符也时sep，光标往后移动一个位置*/
        text_edit_set_cursor(edit->model, cursor + 1);
      }
      return FALSE;
    }

    if (text->str[cursor] == sep || text->str[cursor] == 0) {
      int len = 0;
      uint32_t i = 0;
      uint32_t index = 0;
      const wchar_t* pe = text->str + cursor;
      const wchar_t* ps = pe - 1;

      for (i = 0; i < cursor; i++) {
        if (text->str[i] == sep) {
          index++;
        }
      }

      /*找到当前part的第一个字符*/
      while (ps > text->str && *ps != sep) {
        ps--;
      }

      if (*ps == sep) {
        ps++;
      }

      len = pe - ps;
      /*part 字符超长*/
      if (len >= get_part_max_len(index)) {
        if (text->str[cursor] == 0) {
          return FALSE;
        } else if (text->str[cursor] == sep) {
          /*输入到下一个part*/
          if (text->str[cursor] == sep) {
            text_edit_set_cursor(edit->model, cursor + 1);
          } else {
            text_edit_set_select(edit->model, cursor + 1, cursor + 2);
          }
        }
      } else {
        /*正常输入*/
        text_edit_set_cursor(edit->model, cursor);
      }
    } else {
      /*覆盖下一个字符*/
      text_edit_set_select(edit->model, cursor, cursor + 1);
    }

    return TRUE;
  }

  return FALSE;
}

#endif /*EDIT_PATTERN_H*/
