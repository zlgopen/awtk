#include "widgets/edit_pattern.h"

#define EDIT_DATE_SEP '/'
#define EDIT_DATE_SEP_NR 2
#define EDIT_DATE_DEFVAL "2020/01/01"

static uint32_t edit_date_part_get_max_len(uint32_t index) {
  if (index == 0) {
    /*year*/
    return 4;
  } else {
    /*month/day*/
    return 2;
  }
}

static bool_t edit_date_part_value_is_valid(uint32_t index, int32_t v) {
  if (index == 0) {
    /*year*/
    return v < 9000;
  } else if (index == 1) {
    /*month*/
    return v > 0 && v <= 12;
  } else {
    /*day*/
    return v > 0 && v <= 31;
  }
}

static bool_t edit_date_part_len_is_valid(uint32_t index, uint32_t len) {
  if (index == 0) {
    /*year*/
    return len == 4;
  } else {
    /*month/day*/
    return len == 2;
  }
}

static wchar_t* edit_date_fix_part(uint32_t index, int32_t v, wchar_t* start, wchar_t* end) {
  uint32_t len = end - start;
  const wchar_t* fix_value = NULL;
  if (index == 0) {
    if (len == 0) {
      fix_value = L"2020";
    }
  } else if (index == 1) {
    if (len == 0 || v > 12) {
      fix_value = L"01";
    }
  } else if (index == 2) {
    if (len == 0 || v > 31) {
      fix_value = L"01";
    }
  }

  if (fix_value != NULL) {
    wcscpy(start, fix_value);
    end = start + wcslen(fix_value);
    *end = 0;
  }
  return end;
}

static ret_t edit_date_inc_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, 1, EDIT_DATE_SEP);
}

static ret_t edit_date_dec_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, -1, EDIT_DATE_SEP);
}

static ret_t edit_date_pre_input(widget_t* widget, uint32_t key) {
  return edit_pre_input_with_sep(widget, key, EDIT_DATE_SEP);
}

static ret_t edit_date_fix_ex(widget_t* widget, bool_t strict) {
  return edit_pattern_fix_ex(widget, strict, EDIT_DATE_DEFVAL, EDIT_DATE_SEP, EDIT_DATE_SEP_NR,
                             edit_date_part_get_max_len, edit_date_fix_part);
}

static bool_t edit_date_is_valid_char(widget_t* widget, wchar_t c) {
  edit_pattern_fix_ex(widget, FALSE, EDIT_DATE_DEFVAL, EDIT_DATE_SEP, EDIT_DATE_SEP_NR,
                      edit_date_part_get_max_len, edit_date_fix_part);

  return edit_pattern_is_valid_char(widget, c, EDIT_DATE_SEP, edit_date_part_get_max_len);
}

static bool_t edit_date_is_valid(widget_t* widget) {
  return edit_pattern_is_valid(widget, EDIT_DATE_SEP, EDIT_DATE_SEP_NR, edit_date_part_len_is_valid,
                               edit_date_part_value_is_valid);
}

static ret_t edit_date_fix(widget_t* widget) {
  return edit_date_fix_ex(widget, TRUE);
}
