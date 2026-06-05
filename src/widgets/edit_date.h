#include "widgets/edit_pattern.h"

#define EDIT_DATE_SEP '/'
#define EDIT_DATE_SEP_NR 2
#define EDIT_DATE_DEFVAL "2020/01/01"

/*
  必须确保 edit_pattern 对日期文本按住严格的 年->月->日 的顺序进行处理，
  否则校验 日 时会使用错误的 年月
*/
typedef struct _edit_date_ctx_t {
  int32_t year;
  int32_t month;
} edit_date_ctx_t;

static int32_t edit_date_get_days_in_month(int32_t year, int32_t month) {
  static const uint8_t days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
    return 29;
  }
  if (month >= 1 && month <= 12) {
    return days[month - 1];
  }
  return 31;
}

static uint32_t edit_date_part_get_max_len(uint32_t index) {
  if (index == 0) {
    /*year*/
    return 4;
  } else {
    /*month/day*/
    return 2;
  }
}

static bool_t edit_date_part_value_is_valid_ctx(uint32_t index, int32_t v, void* ctx) {
  edit_date_ctx_t* date_ctx = (edit_date_ctx_t*)ctx;
  if (index == 0) {
    /*year*/
    date_ctx->year = v;
    return v < 9000;
  } else if (index == 1) {
    /*month*/
    date_ctx->month = v;
    return v > 0 && v <= 12;
  } else {
    /*day*/
    return v > 0 && v <= edit_date_get_days_in_month(date_ctx->year, date_ctx->month);
  }
}

static bool_t edit_date_part_len_is_valid(uint32_t index, uint32_t len) {
  if (index == 0) {
    /*year*/
    return len == 4;
  } else {
    /*month/day*/
    return len > 0 && len <= 2;
  }
}

static wchar_t* edit_date_fix_part_ctx(uint32_t index, int32_t v, wchar_t* start, wchar_t* end, void* ctx) {
  edit_date_ctx_t* date_ctx = (edit_date_ctx_t*)ctx;
  uint32_t len = end - start;
  const wchar_t* fix_value = NULL;
  if (index == 0) {
    if (len == 0) {
      date_ctx->year = 2020;
      fix_value = L"2020";
    } else {
      date_ctx->year = v;
    }
  } else if (index == 1) {
    if (len == 0 || v > 12 || v < 1) {
      date_ctx->month = 1;
      fix_value = L"01";
    } else {
      date_ctx->month = v;
    }
  } else if (index == 2) {
    int32_t max_day = edit_date_get_days_in_month(date_ctx->year, date_ctx->month);
    if (len == 0 || v > max_day || v < 1) {
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
  edit_date_ctx_t date_ctx = {2020, 1};
  return edit_pattern_fix_ctx(widget, strict, EDIT_DATE_DEFVAL, EDIT_DATE_SEP, EDIT_DATE_SEP_NR,
                              edit_date_part_get_max_len, edit_date_fix_part_ctx, &date_ctx);
}

static bool_t edit_date_is_valid_char(widget_t* widget, wchar_t c) {
  edit_date_ctx_t date_ctx = {2020, 1};
  edit_pattern_fix_ctx(widget, FALSE, EDIT_DATE_DEFVAL, EDIT_DATE_SEP, EDIT_DATE_SEP_NR,
                       edit_date_part_get_max_len, edit_date_fix_part_ctx, &date_ctx);

  return edit_pattern_is_valid_char(widget, c, EDIT_DATE_SEP, edit_date_part_get_max_len);
}

static bool_t edit_date_is_valid(widget_t* widget) {
  edit_date_ctx_t date_ctx = {2020, 1};
  return edit_pattern_is_valid_ctx(widget, EDIT_DATE_SEP, EDIT_DATE_SEP_NR, edit_date_part_len_is_valid,
                                   edit_date_part_value_is_valid_ctx, &date_ctx);
}

static ret_t edit_date_fix(widget_t* widget) {
  return edit_date_fix_ex(widget, TRUE);
}

static ret_t edit_date_pre_delete(widget_t* widget, delete_type_t delete_type) {
  return edit_pre_delete_with_sep(widget, delete_type, EDIT_DATE_SEP);
}
