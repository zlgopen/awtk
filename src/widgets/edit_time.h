#include "widgets/edit_pattern.h"

#define EDIT_TIME_SEP ':'
#define EDIT_TIME_SEP_NR 1
#define EDIT_TIME_DEFVAL "00:00"

static uint32_t edit_time_part_get_max_len(uint32_t index) {
  return 2;
}

static bool_t edit_time_part_value_is_valid(uint32_t index, int32_t v) {
  if (index == 0) {
    /*hour*/
    return v < 24;
  } else if (index == 1) {
    /*min*/
    return v < 60;
  }

  return FALSE;
}

static bool_t edit_time_part_len_is_valid(uint32_t index, uint32_t len) {
  return len == 2;
}

static wchar_t* edit_time_fix_part(uint32_t index, int32_t v, wchar_t* start, wchar_t* end) {
  uint32_t len = end - start;
  const wchar_t* fix_value = NULL;
  if (index == 0) {
    if (len == 0 || v > 23) {
      fix_value = L"00";
    }
  } else if (index == 1) {
    if (len == 0 || v > 59) {
      fix_value = L"00";
    }
  }

  if (fix_value != NULL) {
    wcscpy(start, fix_value);
    end = start + wcslen(fix_value);
    *end = 0;
  }

  return end;
}

static ret_t edit_time_inc_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, 1, EDIT_TIME_SEP);
}

static ret_t edit_time_dec_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, -1, EDIT_TIME_SEP);
}

static ret_t edit_time_pre_input(widget_t* widget, uint32_t key) {
  return edit_pre_input_with_sep(widget, key, EDIT_TIME_SEP);
}

static ret_t edit_time_fix_ex(widget_t* widget, bool_t strict) {
  return edit_pattern_fix_ex(widget, strict, EDIT_TIME_DEFVAL, EDIT_TIME_SEP, EDIT_TIME_SEP_NR,
                             edit_time_part_get_max_len, edit_time_fix_part);
}

static bool_t edit_time_is_valid_char(widget_t* widget, wchar_t c) {
  edit_pattern_fix_ex(widget, FALSE, EDIT_TIME_DEFVAL, EDIT_TIME_SEP, EDIT_TIME_SEP_NR,
                      edit_time_part_get_max_len, edit_time_fix_part);

  return edit_pattern_is_valid_char(widget, c, EDIT_TIME_SEP, edit_time_part_get_max_len);
}

static bool_t edit_time_is_valid(widget_t* widget) {
  return edit_pattern_is_valid(widget, EDIT_TIME_SEP, EDIT_TIME_SEP_NR, edit_time_part_len_is_valid,
                               edit_time_part_value_is_valid);
}

static ret_t edit_time_fix(widget_t* widget) {
  return edit_time_fix_ex(widget, TRUE);
}
