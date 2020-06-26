#include "widgets/edit_pattern.h"

#define EDIT_IPV4_SEP '.'
#define EDIT_IPV4_SEP_NR 3
#define EDIT_IPV4_MIN_PART_SIZE 1
#define EDIT_IPV4_MAX_PART_SIZE 3
#define EDIT_IPV4_DEFVAL "0.0.0.0"

static uint32_t edit_ipv4_part_get_max_len(uint32_t index) {
  return EDIT_IPV4_MAX_PART_SIZE;
}

static bool_t edit_ipv4_part_value_is_valid(uint32_t index, int32_t v) {
  return ((v) >= 0 && (v) <= 255);
}

static bool_t edit_ipv4_part_len_is_valid(uint32_t index, uint32_t len) {
  return ((len) >= EDIT_IPV4_MIN_PART_SIZE && (len) <= EDIT_IPV4_MAX_PART_SIZE);
}

static wchar_t* edit_ipv4_fix_part(uint32_t index, int32_t v, wchar_t* start, wchar_t* end) {
  if ((v) > 255) {
    return end - 1;
  }

  if (start == end) {
    *start = '0';
    end++;
    *end = '\0';
  }

  return end;
}

static ret_t edit_ipv4_inc_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, 1, EDIT_IPV4_SEP);
}

static ret_t edit_ipv4_dec_value(widget_t* widget) {
  return edit_add_value_with_sep(widget, -1, EDIT_IPV4_SEP);
}

static ret_t edit_ipv4_pre_input(widget_t* widget, uint32_t key) {
  return edit_pre_input_with_sep(widget, key, EDIT_IPV4_SEP);
}

static ret_t edit_ipv4_fix_ex(widget_t* widget, bool_t strict) {
  return edit_pattern_fix_ex(widget, strict, EDIT_IPV4_DEFVAL, EDIT_IPV4_SEP, EDIT_IPV4_SEP_NR,
                             edit_ipv4_part_get_max_len, edit_ipv4_fix_part);
}

static bool_t edit_ipv4_is_valid_char(widget_t* widget, wchar_t c) {
  edit_pattern_fix_ex(widget, FALSE, EDIT_IPV4_DEFVAL, EDIT_IPV4_SEP, EDIT_IPV4_SEP_NR,
                      edit_ipv4_part_get_max_len, edit_ipv4_fix_part);

  return edit_pattern_is_valid_char(widget, c, EDIT_IPV4_SEP, edit_ipv4_part_get_max_len);
}

static bool_t edit_ipv4_is_valid(widget_t* widget) {
  return edit_pattern_is_valid(widget, EDIT_IPV4_SEP, EDIT_IPV4_SEP_NR, edit_ipv4_part_len_is_valid,
                               edit_ipv4_part_value_is_valid);
}

static ret_t edit_ipv4_fix(widget_t* widget) {
  return edit_ipv4_fix_ex(widget, TRUE);
}
