#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "widgets/edit.h"
#include "base/window.h"
#include "widgets/group_box.h"
#include "common.h"

#define TK_TRUE 1u
#define TK_FALSE 0u

TEST(Edit, int) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_int_limit(b, 1, 100, 5), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(1, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(5, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_INT, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_STEP, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, uint) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(b, INPUT_UINT);

  ASSERT_EQ(edit_set_uint_limit(b, 1, 100, 5), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(1, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(5, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_UINT, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_STEP, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  ASSERT_EQ(edit_set_uint_limit(b, 0, UINT32_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(UINT32_MAX, value_uint32(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_UINT, value_uint32(&v2));

  widget_destroy(b);
}

TEST(Edit, set_limit_sets_input_type) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  // set_xxx_limit 会同时设置 input_type
  ASSERT_EQ(edit_set_int_limit(b, -100, 100, 1), RET_OK);
  ASSERT_EQ(EDIT(b)->input_type, INPUT_INT);

  ASSERT_EQ(edit_set_uint_limit(b, 0, 100, 1), RET_OK);
  ASSERT_EQ(EDIT(b)->input_type, INPUT_UINT);

  ASSERT_EQ(edit_set_hex_limit(b, 0x0, 0xFF, 1), RET_OK);
  ASSERT_EQ(EDIT(b)->input_type, INPUT_HEX);

  ASSERT_EQ(edit_set_float_limit(b, -100, 100, 1), RET_OK);
  ASSERT_EQ(EDIT(b)->input_type, INPUT_FLOAT);

  ASSERT_EQ(edit_set_ufloat_limit(b, -100, 100, 1), RET_OK);
  ASSERT_EQ(EDIT(b)->input_type, INPUT_UFLOAT);
  ASSERT_DOUBLE_EQ(EDIT(b)->min, 0.0); // ufloat 的 min < 0 自动修正为 0

  ASSERT_EQ(edit_set_text_limit(b, 0, 100), RET_OK);
  ASSERT_EQ(EDIT(b)->input_type, INPUT_TEXT);

  widget_destroy(b);
}

TEST(Edit, convert_time_to_text) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_TIME);
  widget_set_text(e, L"99:99");
  ASSERT_FALSE(edit_is_valid_value(e));

  ASSERT_EQ(edit_set_text_limit(e, 0, 10), RET_OK);
  ASSERT_EQ(EDIT(e)->input_type, INPUT_TEXT);

  ASSERT_TRUE(wstr_eq(&(e->text), L"99:99"));
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, convert_between_pattern_types) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  // TIME -> TIME_FULL: "12:34" 对 TIME 合法，对 TIME_FULL 段数不足
  edit_set_input_type(e, INPUT_TIME);
  widget_set_text(e, L"12:34");
  ASSERT_TRUE(edit_is_valid_value(e));

  edit_set_input_type(e, INPUT_TIME_FULL);
  ASSERT_FALSE(edit_is_valid_value(e));
  widget_set_text(e, L"12:34:56");
  ASSERT_TRUE(edit_is_valid_value(e));

  // TIME_FULL -> DATE: 同样的内容对 DATE 非法
  edit_set_input_type(e, INPUT_DATE);
  ASSERT_FALSE(edit_is_valid_value(e));
  widget_set_text(e, L"2020/01/01");
  ASSERT_TRUE(edit_is_valid_value(e));

  // DATE -> IPV4: 同样的内容对 IPV4 非法
  edit_set_input_type(e, INPUT_IPV4);
  ASSERT_FALSE(edit_is_valid_value(e));
  widget_set_text(e, L"192.168.1.1");
  ASSERT_TRUE(edit_is_valid_value(e));

  // IPV4 -> INT: 文本不变，但校验和取值方式都变成 INT
  edit_set_int_limit(e, 0, 1000, 1);
  ASSERT_EQ(EDIT(e)->input_type, INPUT_INT);
  ASSERT_FALSE(edit_is_valid_value(e));
  widget_set_text(e, L"123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), 123);

  widget_destroy(e);
}

TEST(Edit, convert_numeric_to_hex) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  // INT "123" -> HEX: "123" 不是合法 hex(缺 0x 前缀)
  edit_set_int_limit(e, -1000, 1000, 1);
  widget_set_text(e, L"123");
  ASSERT_TRUE(edit_is_valid_value(e));

  edit_set_hex_limit(e, 0, 0xFFFF, 1);
  ASSERT_EQ(EDIT(e)->input_type, INPUT_HEX);
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"0x123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x123u);

  // HEX "0x123" -> UINT: "0x123" 不是合法 uint
  edit_set_uint_limit(e, 0, 1000, 1);
  ASSERT_EQ(EDIT(e)->input_type, INPUT_UINT);
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"291");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 291u);

  // UINT -> FLOAT: 同样的内容对 FLOAT 也合法，但取值方式不同
  edit_set_float_limit(e, 0, 1000, 0.1);
  ASSERT_EQ(EDIT(e)->input_type, INPUT_FLOAT);
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_double(e), 291);

  // FLOAT -> INT: 同样的内容对 INT 格式合法，但值范围不合法
  edit_set_int_limit(e, 0, 100, 1);
  ASSERT_EQ(EDIT(e)->input_type, INPUT_INT);
  ASSERT_FALSE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), 291);

  widget_destroy(e);
}

TEST(Edit, hex_value_without_prefix) {
  value_t v;
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_hex_limit(e, 0, 0xFFFFFFFF, 1);

  // 只有带 0x/0X 前缀的文本合法
  widget_set_text(e, L"123");
  ASSERT_FALSE(edit_is_valid_value(e));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(0x123u, value_uint32(&v));

  widget_set_text(e, L"FF");
  ASSERT_FALSE(edit_is_valid_value(e));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(0xFFu, value_uint32(&v));

  // 前导0不影响解析，但没有前缀仍然非法
  widget_set_text(e, L"000000000000000000000000000FF");
  ASSERT_FALSE(edit_is_valid_value(e));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(0xFFu, value_uint32(&v));

  // 带前缀时前导0合法
  widget_set_text(e, L"0x00000000000000000000000000000000000000000123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(0x123u, value_uint32(&v));

  widget_destroy(e);
}

TEST(Edit, leading_zeros) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  // INT
  edit_set_int_limit(e, 0, 1000, 1);
  widget_set_text(e, L"000000000000000000000000000123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), 123);

  // UINT
  edit_set_uint_limit(e, 0, 1000, 1);
  widget_set_text(e, L"000000000000000000000000000123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 123u);

  // HEX: 必须带 0x/0X 前缀
  edit_set_hex_limit(e, 0, 0xFFFF, 1);
  widget_set_text(e, L"0x000000000000000000000000000123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x123u);

  widget_set_text(e, L"0000x0123");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"000000000000000000000000000123");
  ASSERT_FALSE(edit_is_valid_value(e));

  // 超过范围
  widget_set_text(e, L"0x0000000000000000000000000010000");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, hex_inc_without_prefix) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_hex_limit(e, 0, 0xFFFFFFFF, 1);

  // "123" 按 16 进制是 0x123，inc 后应为 0x124 而不是 0x7C，且保持无前缀风格
  widget_set_text(e, L"123");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"124"));

  widget_set_text(e, L"FF");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"100"));

  widget_set_text(e, L"10");
  edit_set_cursor(e, e->text.size);
  edit_dec(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"F"));

  // 有前缀时保持前缀，大小写跟随原文本
  widget_set_text(e, L"0x123");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0x124"));

  widget_set_text(e, L"0XFF");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0X100"));

  widget_set_text(e, L"x123");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"x124"));

  widget_set_text(e, L"XFF");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"X100"));

  // auto_fix 时只有 0x/0X 保留，其他前缀统一成 0x
  edit_set_auto_fix(e, TRUE);
  widget_set_text(e, L"7");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0x8"));

  widget_set_text(e, L"X7");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0x8"));

  widget_set_text(e, L"x7");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0x8"));

  widget_set_text(e, L"0x7");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0x8"));

  widget_set_text(e, L"0X7");
  edit_set_cursor(e, e->text.size);
  edit_inc(EDIT(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"0X8"));

  widget_destroy(e);
}

static bool_t custom_is_upper_char(widget_t* widget, wchar_t c) {
  edit_t* e = EDIT(widget);
  return c >= L'A' && c <= L'Z' && widget->text.size < e->max;
}

static bool_t custom_is_upper_value(widget_t* widget) {
  uint32_t i = 0;
  wstr_t* text = &(widget->text);
  edit_t* e = EDIT(widget);

  if (text->size == 0 || text->size > e->max || text->size < e->min) {
    return FALSE;
  }

  for (i = 0; i < text->size; i++) {
    if (text->str[i] < L'A' || text->str[i] > L'Z') {
      return FALSE;
    }
  }

  return TRUE;
}

static ret_t custom_fix_value(widget_t* widget) {
  wstr_t* text = &(widget->text);
  edit_t* edit = EDIT(widget);
  uint32_t i = 0;
  uint32_t max = edit->max;
  uint32_t write_pos = 0;

  for (i = 0; i < text->size; i++) {
    if (text->str[i] >= 'A' && text->str[i] <= 'Z') {
      text->str[write_pos++] = text->str[i];
    }
  }
  text->size = write_pos;
  text->str[write_pos] = 0;

  if (text->size > max) {
    text->size = max;
    text->str[max] = 0;
  }

  return RET_OK;
}

TEST(Edit, is_valid_pattern_custom_upper) {
  // 自定义规则: 所以输入必须是大写字母，长度 [2,5]
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_CUSTOM);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 2);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 5);
  edit_set_is_valid_char(e, custom_is_upper_char);

  widget_set_text(e, L"aAAaB");
  ASSERT_STREQ(e->state, WIDGET_STATE_NORMAL);
  ASSERT_TRUE(edit_is_valid_value(e));
  edit_set_is_valid_value(e, custom_is_upper_value);
  ASSERT_STREQ(e->state, WIDGET_STATE_ERROR);
  ASSERT_FALSE(edit_is_valid_value(e));
  edit_set_fix_value(e, custom_fix_value);
  edit_set_auto_fix(e, TRUE);
  ASSERT_TRUE(wstr_eq(&(e->text), L"AAB"));
  ASSERT_TRUE(edit_is_valid_value(e));

  // 小写字母和数字不允许输入
  widget_set_text(e, L"");
  edit_set_auto_fix(e, FALSE);
  ASSERT_NE(edit_input_char(e, L'a'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'1'), RET_OK);
  ASSERT_FALSE(edit_is_valid_value(e));

  // 大写字母可以输入
  ASSERT_EQ(edit_input_char(e, L'A'), RET_OK);
  ASSERT_FALSE(edit_is_valid_value(e)); // 最小长度 2
  ASSERT_EQ(edit_input_char(e, L'B'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'C'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'D'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'E'), RET_OK);
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"ABCDE"));
  ASSERT_NE(edit_input_char(e, L'F'), RET_OK); // 最大长度 5
  ASSERT_TRUE(wstr_eq(&(e->text), L"ABCDE"));

  // 程序设置大写字母内容合法
  widget_set_text(e, L"HELLO");
  ASSERT_TRUE(edit_is_valid_value(e));

  // 程序设置含小写的内容非法
  widget_set_text(e, L"Hello");
  ASSERT_FALSE(edit_is_valid_value(e));

  // 程序设置含数字的内容非法
  widget_set_text(e, L"AB1");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_destroy(e);
}

static bool_t custom_password_is_valid_char(widget_t* widget, wchar_t c) {
  edit_t* e = EDIT(widget);
  // 可打印 ASCII: 数字、大小写字母和特殊符号都可以输入(不含空格)
  return c > 0x20 && c <= 0x7E && widget->text.size < e->max;
}

static bool_t custom_password_is_valid_value(widget_t* widget) {
  uint32_t i = 0;
  bool_t has_lower = FALSE;
  bool_t has_upper = FALSE;
  bool_t has_digit = FALSE;
  wstr_t* text = &(widget->text);
  edit_t* e = EDIT(widget);

  if (text->size == 0 || text->size > e->max || text->size < e->min) {
    return FALSE;
  }

  for (i = 0; i < text->size; i++) {
    wchar_t c = text->str[i];
    if (c >= L'a' && c <= L'z') {
      has_lower = TRUE;
    } else if (c >= L'A' && c <= L'Z') {
      has_upper = TRUE;
    } else if (c >= L'0' && c <= L'9') {
      has_digit = TRUE;
    }
  }

  // 至少包含数字、小写字母、大写字母三种符号
  return has_lower && has_upper && has_digit;
}

TEST(Edit, is_valid_pattern_custom_password) {
  // 自定义规则: 至少包含数字、大写字母、小写字母三种类型的符号, 长度 [4,8]
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_CUSTOM_PASSWORD);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 4);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 8);
  edit_set_is_valid_char(e, custom_password_is_valid_char);

  widget_set_text(e, L"abcd");
  ASSERT_STREQ(e->state, WIDGET_STATE_NORMAL);
  ASSERT_TRUE(edit_is_valid_value(e));
  edit_set_is_valid_value(e, custom_password_is_valid_value);
  ASSERT_STREQ(e->state, WIDGET_STATE_ERROR);
  ASSERT_FALSE(edit_is_valid_value(e));

  ASSERT_EQ(edit_input_char(e, L'#'), RET_OK);

  // 少于三种符号时非法
  widget_set_text(e, L"abcd");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"abcd1234"); // 缺大写
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"ABCD1234"); // 缺小写
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"Abc#"); // 缺数字
  ASSERT_FALSE(edit_is_valid_value(e));

  // 太短
  widget_set_text(e, L"aA1");
  ASSERT_FALSE(edit_is_valid_value(e));

  // 三种都有即合法
  widget_set_text(e, L"aA11");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"Abcd1234");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"Abc1#");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"");
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'B'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_NE(edit_input_char(e, L' '), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'#'), RET_OK);
  ASSERT_TRUE(wstr_eq(&(e->text), L"aB3#"));
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'a'), RET_OK); // 太长
  ASSERT_TRUE(wstr_eq(&(e->text), L"aB3#aaaa"));

  widget_destroy(e);
}

TEST(Edit, clamp_min_max) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  // INPUT_UINT: min 不能 < 0
  edit_set_input_type(b, INPUT_UINT);
  value_set_int(&v1, -10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2));

  // INPUT_UINT: max 不能 < 0
  value_set_int(&v1, -1);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2));

  // INPUT_UINT: max 不能 > UINT32_MAX
  value_set_int64(&v1, UINT32_MAX + 1LL);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(UINT32_MAX, value_uint32(&v2));

  // INPUT_INT: min 不能 < INT32_MIN
  edit_set_input_type(b, INPUT_INT);
  value_set_int64(&v1, (int64_t)INT32_MIN - 1);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(INT32_MIN, value_int(&v2));

  // INPUT_INT: max 不能 > INT32_MAX
  value_set_int64(&v1, (int64_t)INT32_MAX + 1);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(INT32_MAX, value_int(&v2));

  // INPUT_HEX: min 不能 < 0，且用 uint32 表示
  edit_set_input_type(b, INPUT_HEX);
  value_set_int(&v1, -5);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0u, value_uint32(&v2));

  // INPUT_UFLOAT: min 不能 < 0
  edit_set_input_type(b, INPUT_UFLOAT);
  value_set_double(&v1, -10.5);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0.0, value_double(&v2));

  // INPUT_TEXT: min 不能 < 0（长度限制）
  edit_set_input_type(b, INPUT_TEXT);
  value_set_int(&v1, -3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2));

  // INPUT_EMAIL: min 不能 < 0（长度限制）
  edit_set_input_type(b, INPUT_EMAIL);
  value_set_int(&v1, -1);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2));

  // INPUT_PHONE: min 不能 < 0（长度限制）
  edit_set_input_type(b, INPUT_PHONE);
  value_set_int32(&v1, -1);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2));

  // 切换 input_type 时自动 clamp: INT → UINT
  edit_set_int_limit(b, -100, 1000, 1);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(-100, value_int(&v2)); // -100 在 INT32 范围内，clamp 不改变
  edit_set_input_type(b, INPUT_UINT);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_uint32(&v2)); // min=-100 clamped to 0 for UINT

  edit_set_input_type(b, INPUT_INT);
  // 设置 min 时自动修正 max
  edit_set_int_limit(b, 0, 100, 1);
  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(200, value_int(&v2));
  // 设置 max 时自动修正 min
  edit_set_int_limit(b, 0, 100, 1);
  value_set_int(&v1, -100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(-100, value_int(&v2));

  // HEX: max 不能 > UINT32_MAX
  edit_set_input_type(b, INPUT_HEX);
  value_set_int64(&v1, UINT32_MAX + 1LL);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(UINT32_MAX, value_uint32(&v2));

  // HEX: min 用 uint32 读写
  value_set_uint32(&v1, 0x100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0x100u, value_uint32(&v2));

  widget_destroy(b);
}

TEST(Edit, min_eq_max) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  // INT: min == max
  edit_set_input_type(e, INPUT_INT);
  edit_set_int_limit(e, 50, 50, 1);
  edit_set_auto_fix(e, TRUE);

  widget_set_text_utf8(e, "100");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), 100);

  edit_inc(EDIT(e));
  ASSERT_EQ(edit_get_int(e), 101);

  edit_dec(EDIT(e));
  ASSERT_EQ(edit_get_int(e), 100);

  // UINT: min == max
  edit_set_input_type(e, INPUT_UINT);
  edit_set_uint_limit(e, 200, 200, 1);
  edit_set_auto_fix(e, TRUE);

  widget_set_text_utf8(e, "4294967295");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 4294967295u);

  edit_inc(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);

  edit_dec(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 4294967295u);

  // HEX: min == max
  edit_set_hex_limit(e, 0xFF, 0xFF, 1);
  widget_set_text(e, L"1");
  ASSERT_TRUE(wstr_eq(&(e->text), L"0x1"));
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x1u);

  edit_inc(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x2u);

  edit_dec(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x1u);

  // TEXT: min == max
  edit_set_text_limit(e, 5, 5);
  edit_set_auto_fix(e, FALSE);
  widget_set_text(e, L"1234");
  ASSERT_FALSE(edit_is_valid_value(e));
  widget_set_text(e, L"12345");
  ASSERT_TRUE(edit_is_valid_value(e));
  widget_set_text(e, L"123456");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_TRUE(wstr_eq(&(e->text), L"12345")); // edit_set_text_impl 截断字符串不受 auto_fix 控制

  widget_destroy(e);
}

TEST(Edit, auto_fix_uint) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  // uint 大值 auto_fix
  edit_set_input_type(e, INPUT_UINT);
  edit_set_uint_limit(e, 0, UINT32_MAX, 1);
  edit_set_auto_fix(e, TRUE);

  // 超出 max 应 auto_fix 到 max
  widget_set_text_utf8(e, "12345678900");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), UINT32_MAX);

  // 正常范围内的值
  widget_set_text_utf8(e, "100");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 100u);

  // min / max 小范围
  edit_set_uint_limit(e, 300, 400, 1);
  widget_set_text(e, L"0");
  ASSERT_EQ((uint32_t)edit_get_int64(e), 300u);
  widget_set_text(e, L"1000");
  ASSERT_EQ((uint32_t)edit_get_int64(e), 400u);

  // 分开设置 min / max 且 min 大于原来的 max
  value_t v;
  value_set_int64(&v, 500);
  ASSERT_EQ(widget_set_prop(e, WIDGET_PROP_MIN, &v), RET_OK);
  ASSERT_DOUBLE_EQ(EDIT(e)->min, EDIT(e)->max);
  value_set_int64(&v, 600);
  ASSERT_EQ(widget_set_prop(e, WIDGET_PROP_MAX, &v), RET_OK);
  widget_set_text(e, L"0");
  ASSERT_EQ((uint32_t)edit_get_int64(e), 500u);
  widget_set_text(e, L"1000");
  ASSERT_EQ((uint32_t)edit_get_int64(e), 600u);

  // 超越 uint32 范围的 min/max
  value_set_int64(&v, -1000);
  ASSERT_EQ(widget_set_prop(e, WIDGET_PROP_MIN, &v), RET_OK);
  value_set_int64(&v, 1e10);
  ASSERT_EQ(widget_set_prop(e, WIDGET_PROP_MAX, &v), RET_OK);
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_MIN, &v), RET_OK);
  ASSERT_EQ(0, value_int64(&v));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_MAX, &v), RET_OK);
  ASSERT_EQ(UINT32_MAX, value_int64(&v));
  widget_set_text(e, L"-1");
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);
  widget_set_text(e, L"9000000000"); // 9e9
  ASSERT_EQ((uint32_t)edit_get_int64(e), UINT32_MAX);

  edit_set_uint_limit(e, 100, 200, 1);
  widget_set_text(e, L"-1");
  ASSERT_EQ((uint32_t)edit_get_int64(e), 100);
  widget_set_text(e, L"9000000000"); // 9e9
  ASSERT_EQ((uint32_t)edit_get_int64(e), 200);

  widget_destroy(e);
}

TEST(Edit, auto_fix_hex) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_HEX);
  edit_set_hex_limit(e, 0x100, 0xFFFF, 1);
  edit_set_auto_fix(e, TRUE);

  // 低于 min 应 auto_fix 到 min
  widget_set_text(e, L"0x50");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x100u);

  // 超出 max 应 auto_fix 到 max
  widget_set_text(e, L"0x1FFFF");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0xFFFFu);

  // 正常范围内的值
  widget_set_text(e, L"0x0200");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x0200u);

  widget_destroy(e);
}

TEST(Edit, inc_dec_int) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_INT);
  edit_set_int_limit(e, -100, 100, 1);
  edit_set_auto_fix(e, TRUE);

  // 空文本 inc 时使用 min 作为初始值
  edit_set_auto_fix(e, FALSE);
  widget_set_text_utf8(e, ""); // 设置空文本本身可能触发auto_fix
  edit_inc(EDIT(e));
  ASSERT_EQ(edit_get_int(e), -100);
  edit_set_auto_fix(e, TRUE);

  // 空文本 dec 时使用 max 作为初始值
  edit_set_auto_fix(e, FALSE);
  widget_set_text_utf8(e, "");
  edit_dec(EDIT(e));
  ASSERT_EQ(edit_get_int(e), 100);
  edit_set_auto_fix(e, TRUE);

  // inc 到 max 后 auto_fix 到 max
  widget_set_text_utf8(e, "100");
  edit_inc(EDIT(e));
  ASSERT_EQ(edit_get_int(e), 100);

  edit_dec(EDIT(e));
  ASSERT_EQ(edit_get_int(e), 99);
  widget_set_prop_float(e, WIDGET_PROP_STEP, 1.9);
  for (int i = 1; i <= 10; ++i) {
    edit_dec(EDIT(e));
    ASSERT_EQ(edit_get_int(e), 99 - i);
  }

  // dec 到 min 后 auto_fix 到 min
  widget_set_text_utf8(e, "-100");
  edit_dec(EDIT(e));
  ASSERT_EQ(edit_get_int(e), -100);

  widget_destroy(e);
}

TEST(Edit, inc_dec_uint) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  // uint 大值 inc/dec
  edit_set_input_type(e, INPUT_UINT);
  edit_set_uint_limit(e, 0, UINT32_MAX, 1);

  // 空文本 inc 使用 min=0
  edit_set_auto_fix(e, FALSE);
  widget_set_text_utf8(e, ""); // 设置空文本本身可能触发auto_fix
  edit_inc(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);
  edit_set_auto_fix(e, TRUE);

  // 空文本 dec 使用 max
  edit_set_auto_fix(e, FALSE);
  widget_set_text_utf8(e, "");
  edit_dec(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), UINT32_MAX);
  edit_set_auto_fix(e, TRUE);

  // inc 超过 max 时 auto_fix 到 max
  widget_set_text_utf8(e, "4294967295");
  edit_inc(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), UINT32_MAX);

  // dec 到 0 后 auto_fix 到 min=0
  widget_set_text_utf8(e, "0");
  edit_dec(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);

  widget_destroy(e);
}

TEST(Edit, inc_dec_hex) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_HEX);
  edit_set_hex_limit(e, 0, 0xFF, 1);

  // 空文本 inc 使用 min
  edit_set_auto_fix(e, FALSE);
  widget_set_text_utf8(e, ""); // 设置空文本本身可能触发auto_fix
  edit_inc(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);
  edit_set_auto_fix(e, TRUE);

  // 空文本 dec 使用 max
  edit_set_auto_fix(e, FALSE);
  widget_set_text_utf8(e, "");
  edit_dec(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0xFFu);
  edit_set_auto_fix(e, TRUE);

  // inc 超过 max auto_fix 到 max
  widget_set_text(e, L"0XFF");
  edit_inc(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0xFFu);

  // dec 到 min auto_fix 到 min
  widget_set_text(e, L"0X0");
  edit_dec(EDIT(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);

  widget_destroy(e);
}

TEST(Edit, tips) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TIPS, "tips"), RET_OK);
  ASSERT_STREQ(EDIT(b)->tips, "tips");

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TR_TIPS, "tr_tips"), RET_OK);
  ASSERT_STREQ(EDIT(b)->tr_tips, "tr_tips");

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TR_TIPS, ""), RET_OK);
  ASSERT_EQ(EDIT(b)->tr_tips == NULL, true);

  widget_destroy(b);
}

TEST(Edit, set_text) {
  text_edit_state_t state;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  widget_set_text_utf8(b, "hello");

  ASSERT_EQ(text_edit_get_state(EDIT(b)->model, &state), RET_OK);
  ASSERT_EQ(state.cursor, 0u);

  edit_set_cursor(b, b->text.size);
  ASSERT_EQ(text_edit_get_state(EDIT(b)->model, &state), RET_OK);
  ASSERT_EQ(state.cursor, 5u);

  widget_destroy(b);
}

TEST(Edit, set_int) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(edit_set_int(b, -10), RET_OK);
  ASSERT_EQ(edit_get_int(b), -10);

  widget_destroy(b);
}

TEST(Edit, action_text) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_STREQ(EDIT(b)->action_text, "done");
  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_ACTION_TEXT, "next"), RET_OK);
  ASSERT_STREQ(EDIT(b)->action_text, "next");
  ASSERT_STREQ(widget_get_prop_str(b, WIDGET_PROP_ACTION_TEXT, ""), "next");

  widget_destroy(b);
}

TEST(Edit, inputable) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b->vt->inputable, TK_TRUE);
  widget_destroy(b);
}

TEST(Edit, focusable) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b->vt->focusable, TK_TRUE);
  widget_destroy(b);
}

TEST(Edit, text) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_text_limit(b, 0, 100), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_TEXT, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, float) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_float_limit(b, 0, 100, 0.1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0.0f, value_float(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_FLOAT, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100.0f, value_float(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(0.1f - value_float(&v2) < 0.001f, true);

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  widget_destroy(b);
}

TEST(Edit, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_readonly(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->readonly, TK_TRUE);

  ASSERT_EQ(edit_set_cancelable(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->cancelable, TK_TRUE);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_READONLY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_READONLY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  ASSERT_EQ(edit_set_auto_fix(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->auto_fix, TK_TRUE);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_READONLY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_READONLY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_LEFT_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_LEFT_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_RIGHT_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_RIGHT_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_TOP_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_TOP_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_BOTTOM_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_BOTTOM_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, cast) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b, edit_cast(b));
  ASSERT_EQ((edit_t*)NULL, EDIT(NULL));
  widget_destroy(b);
}

TEST(Edit, focus) {
  widget_t* w = window_create(NULL, 0, 0, 400, 400);
  widget_t* g = group_box_create(w, 0, 0, 400, 400);
  widget_t* e = edit_create(g, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_focus(e, TRUE), RET_OK);
  ASSERT_EQ(w->key_target, g);
  ASSERT_EQ(g->key_target, e);

  widget_destroy(w);
}

TEST(Edit, focus_next) {
  widget_t* w = window_create(NULL, 0, 0, 400, 400);
  widget_t* g = group_box_create(w, 0, 0, 400, 400);
  widget_t* e1 = edit_create(g, 10, 20, 30, 40);
  widget_t* e2 = edit_create(g, 10, 60, 30, 40);
  widget_t* e3 = edit_create(g, 10, 90, 30, 40);

  ASSERT_EQ(edit_set_focus(e1, TRUE), RET_OK);
  ASSERT_EQ(g->key_target, e1);
  ASSERT_EQ(widget_focus_next(e1), RET_OK);
  ASSERT_EQ(g->key_target, e2);
  ASSERT_EQ(widget_focus_next(e1), RET_FAIL);

  ASSERT_EQ(widget_focus_next(e2), RET_OK);
  ASSERT_EQ(g->key_target, e3);

  ASSERT_EQ(widget_focus_next(e3), RET_OK);
  ASSERT_EQ(g->key_target, e1);

  widget_destroy(w);
}

TEST(Edit, focus_prev) {
  widget_t* w = window_create(NULL, 0, 0, 400, 400);
  widget_t* g = group_box_create(w, 0, 0, 400, 400);
  widget_t* e1 = edit_create(g, 10, 20, 30, 40);
  widget_t* e2 = edit_create(g, 10, 60, 30, 40);
  widget_t* e3 = edit_create(g, 10, 90, 30, 40);

  ASSERT_EQ(edit_set_focus(e1, TRUE), RET_OK);
  ASSERT_EQ(g->key_target, e1);

  ASSERT_EQ(widget_focus_prev(e1), RET_OK);
  ASSERT_EQ(g->key_target, e3);
  ASSERT_EQ(widget_focus_prev(e1), RET_FAIL);

  ASSERT_EQ(widget_focus_prev(e3), RET_OK);
  ASSERT_EQ(g->key_target, e2);

  ASSERT_EQ(widget_focus_prev(e2), RET_OK);
  ASSERT_EQ(g->key_target, e1);

  widget_destroy(w);
}

TEST(Edit, is_valid_chr_4_str) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_text_limit(e, 0, 3);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'b'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'c'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);
  ASSERT_EQ(e->text.size, 3u);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_int) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_INT);
  edit_set_int_limit(e, -1000, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(e->text.size, 5u);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_uint) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_UINT);
  edit_set_uint_limit(e, 0, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(e->text.size, 4u);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_float) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1000, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'.'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_ufloat) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_UFLOAT);
  edit_set_ufloat_limit(e, 0, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'.'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_value_int_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_INT);
  edit_set_int_limit(e, -1e9, 1e9, 1);

  widget_set_text(e, L"1.9");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1e5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1.0");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+1.2");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-3.14");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"abc");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1a");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), 123);

  widget_set_text(e, L"-321");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), -321);

  widget_set_text(e, L"+100");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), +100);

  widget_set_text(e, L"+0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), +0);

  widget_set_text(e, L"-0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(edit_get_int(e), -0);

  widget_destroy(e);
}

TEST(Edit, is_valid_value_uint_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_UINT);
  edit_set_uint_limit(e, 0, 1e9, 1);

  widget_set_text(e, L"1.9");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-1");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+1.2");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"abc");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 123u);

  widget_set_text(e, L"+100");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 100u);

  widget_set_text(e, L"0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);

  widget_destroy(e);
}

TEST(Edit, is_valid_value_float_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1e10, 1e10, 10);

  widget_set_text(e, L"+e5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1e.5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1.5E.3");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1.5e3E2");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+1+2");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1e+");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(123, edit_get_double(e));

  widget_set_text(e, L"-321.123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-321.123, edit_get_double(e));

  widget_set_text(e, L"-1.");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-1., edit_get_double(e));

  widget_set_text(e, L"+.2");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(+.2, edit_get_double(e));

  widget_set_text(e, L"-2.e-3");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-2.e-3, edit_get_double(e));

  widget_set_text(e, L".3e-2");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(.3e-2, edit_get_double(e));

  widget_set_text(e, L".0e2");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(.0e2, edit_get_double(e));

  widget_set_text(e, L"+1.5E+3");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(+1.5E+3, edit_get_double(e));

  widget_set_text(e, L"1.5e+3");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(1.5e+3, edit_get_double(e));

  widget_set_text(e, L"-1.5e-5");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-1.5e-5, edit_get_double(e));

  widget_set_text(e, L"-1.5e+5");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-1.5e+5, edit_get_double(e));

  widget_set_text(e, L"-2E+5");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-2E+5, edit_get_double(e));

  widget_set_text(e, L"-0e0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-0e0, edit_get_double(e));

  widget_set_text(e, L"-0.0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(-0.0, edit_get_double(e));

  widget_set_text(e, L"3.14e0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(3.14e0, edit_get_double(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_value_ufloat_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_UFLOAT);
  edit_set_ufloat_limit(e, -1e10, 1e10, 10);

  widget_set_text(e, L"+e5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1e.5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1.5E.3");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1.5e3E2");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+1+2");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"1e+");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-321.123");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-1.");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-2.e-3");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-0e0");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-0.0");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-1.5e-5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-1.5e+5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"-2E+5");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+0.0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(+0.0, edit_get_double(e));

  widget_set_text(e, L"123");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(123, edit_get_double(e));

  widget_set_text(e, L"+.2");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(+.2, edit_get_double(e));

  widget_set_text(e, L"1E5");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(1E5, edit_get_double(e));

  widget_set_text(e, L".3e-2");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(.3e-2, edit_get_double(e));

  widget_set_text(e, L"+1.5E+3");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(+1.5E+3, edit_get_double(e));

  widget_set_text(e, L"1.5e+3");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(1.5e+3, edit_get_double(e));

  widget_set_text(e, L"3.14e0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_FLOAT_EQ(3.14e0, edit_get_double(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_float_scientific) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1e10, 1e10, 10);

  // +1.5e+3
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'5'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'e'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);

  // 第三个 +/-
  ASSERT_NE(edit_input_char(e, L'+'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  // 第二个小数点
  ASSERT_NE(edit_input_char(e, L'.'), RET_OK);
  // 第二个 e/E
  ASSERT_NE(edit_input_char(e, L'e'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'E'), RET_OK);

  ASSERT_FLOAT_EQ(+1.5e+3, edit_get_double(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_float_negative_exponent) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1e10, 1e10, 10);

  // -.5e-3
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'5'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'E'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);

  ASSERT_FLOAT_EQ(-.5e-3, edit_get_double(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_float_uppercase_E) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1e10, 1e10, 10);

  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'E'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);

  ASSERT_FLOAT_EQ(1E3, edit_get_double(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_float_prevent_invalid) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1000, 1000, 10);

  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'e'), RET_OK);

  widget_set_text(e, L"1e");
  edit_set_cursor(e, 2);
  ASSERT_NE(edit_input_char(e, L'.'), RET_OK); // 1e.

  widget_set_text(e, L"1.");
  edit_set_cursor(e, 2);
  ASSERT_EQ(edit_input_char(e, L'e'), RET_OK); // 1.e
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK); // 1.e3
  ASSERT_FLOAT_EQ(1.e3, edit_get_double(e));

  widget_set_text(e, L"1");
  edit_set_cursor(e, 0);
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK); // +1

  widget_set_text(e, L"1");
  edit_set_cursor(e, 0);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK); // -1

  widget_set_text(e, L"1");
  edit_set_cursor(e, 1);
  ASSERT_NE(edit_input_char(e, L'-'), RET_OK); // 1-
  ASSERT_NE(edit_input_char(e, L'+'), RET_OK); // 1+

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_ufloat_exponent) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_UFLOAT);
  edit_set_ufloat_limit(e, 0, 1e10, 10);

  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);

  // +1.5e-3
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'5'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'e'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);

  double value = edit_get_double(e);
  ASSERT_FLOAT_EQ(value, +1.5e-3);

  widget_destroy(e);
}

TEST(Edit, is_valid_value_hex_format) {
  value_t v;
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_HEX);
  edit_set_hex_limit(e, 0, 0xFFFF, 1);

  widget_set_text(e, L"abc");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"0x");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"123");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"X1");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"0x1g");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"0XFf");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 0xFfu);

  widget_set_text(e, L"0x01");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0x01u);

  widget_set_text(e, L"0X0");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0u);

  widget_set_text(e, L"0xABCD");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ(widget_get_prop(e, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 0xABCDu);

  // 大值 hex: 0xFFFFFFFF
  edit_set_hex_limit(e, 0, 0xFFFFFFFF, 1);
  widget_set_text(e, L"0xFFFFFFFF");
  ASSERT_TRUE(edit_is_valid_value(e));
  ASSERT_EQ((uint32_t)edit_get_int64(e), 0xFFFFFFFF);

  widget_destroy(e);
}

TEST(Edit, is_valid_value_email_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_EMAIL);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 0);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 100);

  widget_set_text(e, L"abc");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"@example.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L".user@example.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user..name@example.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@.example.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@example.");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@example..com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@example");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@-example.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@example-.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@name@host.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user name@example.com");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"user@example.c");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "user@example.com");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "+user_01-.name@exa-mple.com");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "user+tag@example.com");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "a@b.cn");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "2@2--3.233");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_value_phone_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_PHONE);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 0);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 100);

  widget_set_text(e, L"-435");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+086-138");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"abc");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"123-");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"123--456");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+1");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"+12");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"1");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"12345678901");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"123-456-7890");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"+8612345678");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text(e, L"+86-123-45678");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_value_ascii_format) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);
  edit_set_input_type(e, INPUT_ASCII);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 0);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 100);

  widget_set_text(e, L"\x01" L"abc");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"abc\x7F");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"abc\x80");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text(e, L"你好");
  ASSERT_FALSE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "hello");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "Hello World!");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "a@b.com");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_set_text_utf8(e, "123");
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_email) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_EMAIL);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'@'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'@'), RET_OK);
  ASSERT_NE(edit_input_char(e, L' '), RET_OK);

  edit_clear((edit_t*)e);

  // @ 不能在开头
  ASSERT_NE(edit_input_char(e, L'@'), RET_OK);

  // . 不能在开头
  ASSERT_NE(edit_input_char(e, L'.'), RET_OK);

  // 合法的 local part 字符: +, _, -, A, 5, ., b
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'_'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'A'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'5'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'b'), RET_OK);

  // 非法字符
  ASSERT_NE(edit_input_char(e, L'!'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'#'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'$'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'%'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'&'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'*'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'='), RET_OK);

  edit_clear((edit_t*)e);

  // _ 和 + 在 local part 合法，在 domain part 非法
  ASSERT_EQ(edit_input_char(e, L'u'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'_'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'@'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'_'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'+'), RET_OK);

  edit_clear((edit_t*)e);

  // domain label 不能以 - 开头
  ASSERT_EQ(edit_input_char(e, L'u'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'@'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  // 输入过程中 domain 的 . 后面不能输入 -
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  // 输入过程中 domain 的 . 前面能输入 -
  edit_set_cursor(e, e->text.size - 1);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK); // u@1-.
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK); // u@1-a.
  ASSERT_EQ(edit_input_char(e, L'b'), RET_OK); // u@1-ab.
  ASSERT_FALSE(edit_is_valid_value(e));
  edit_set_cursor(e, e->text.size);
  ASSERT_EQ(edit_input_char(e, L'c'), RET_OK); // u@1-ab.c
  ASSERT_EQ(edit_input_char(e, L'n'), RET_OK); // u@1-ab.cn
  ASSERT_TRUE(edit_is_valid_value(e));

  edit_clear((edit_t*)e);

  // 输入完整合法 email: user@example.com
  ASSERT_EQ(edit_input_char(e, L'u'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L's'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'e'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'r'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'@'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'e'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'x'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'm'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'p'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'l'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'e'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'c'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'o'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'm'), RET_OK);
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_hex) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_HEX);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'x'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'2'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'4'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'x'), RET_OK);

  edit_clear((edit_t*)e);
  ASSERT_EQ(edit_input_char(e, L'A'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'B'), RET_OK);
  edit_set_cursor(e, 0);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'X'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_phone) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_PHONE);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'2'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'4'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  // 011-234-
  ASSERT_FALSE(edit_is_valid_value(e));

  ASSERT_NE(edit_input_char(e, L'x'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'a'), RET_OK);

  edit_set_cursor(e, 0);
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_FALSE(edit_is_valid_value(e)); // + 后面在输入时允许 0，但最终的号码不允许 + 后面出现 0
  key_event_t key;
  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_BACKSPACE);
  widget_dispatch(e, (event_t*)&key);
  ASSERT_EQ(edit_input_char(e, L'8'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'6'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  // +86-011-234-
  ASSERT_FALSE(edit_is_valid_value(e));
  edit_set_cursor(e, e->text.size);
  // +86-011-234-7
  ASSERT_EQ(edit_input_char(e, L'7'), RET_OK);
  ASSERT_TRUE(edit_is_valid_value(e));

  ASSERT_NE(edit_input_char(e, L'+'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_ascii) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_ASCII);
  value_t v;
  value_set_int64(&v, -1);
  ASSERT_EQ(widget_set_prop(e, WIDGET_PROP_MIN, &v), RET_OK);
  value_set_int64(&v, 1e10);
  ASSERT_EQ(widget_set_prop(e, WIDGET_PROP_MAX, &v), RET_OK);
  ASSERT_EQ(EDIT(e)->min, 0);
  ASSERT_EQ(EDIT(e)->max, UINT32_MAX);

  widget_set_text(e, L"《》"); // 有非法字符也应该能继续输入
  uint32_t save_size = e->text.size;
  edit_set_cursor(e, save_size);
  ASSERT_EQ(edit_input_char(e, L'A'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'B'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L' '), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'!'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L' '), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'@'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'\x01'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'\x7F'), RET_OK);
  ASSERT_FALSE(edit_is_valid_value(e));
  edit_set_select(e, 0, save_size);
  key_event_t key;
  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_BACKSPACE);
  widget_dispatch(e, (event_t*)&key);
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

static bool_t any_char_is_valid(widget_t* widget, wchar_t c) {
  return TRUE;
}

static bool_t any_string_is_valid_value(widget_t* widget) {
  return TRUE;
}

TEST(Edit, is_valid_chr_4_custom) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_PHONE);
  edit_set_is_valid_char(e, any_char_is_valid);
  edit_set_is_valid_value(e, any_string_is_valid_value);

  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'2'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'4'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);

  ASSERT_EQ(edit_input_char(e, L'x'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_TRUE(edit_is_valid_value(e));

  widget_destroy(e);
}

TEST(Edit, events) {
  string event_log;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  widget_on(b, EVT_VALUE_CHANGING, widget_log_events, &event_log);
  widget_on(b, EVT_VALUE_CHANGED, widget_log_events, &event_log);

  event_log = "";
  widget_set_text(b, L"a");
  ASSERT_EQ(event_log, "value_changed");
  ASSERT_EQ(wcscmp(b->text.str, L"a"), 0);

  event_log = "";
  widget_set_text(b, L"a");
  ASSERT_EQ(event_log, "");
  ASSERT_EQ(wcscmp(b->text.str, L"a"), 0);

  event_log = "";
  widget_set_text(b, L"1");
  ASSERT_EQ(event_log, "value_changed");
  ASSERT_EQ(wcscmp(b->text.str, L"1"), 0);

  event_log = "";
  edit_input_char(b, 'a');
  ASSERT_EQ(event_log, "value_changing");
  ASSERT_EQ(wcscmp(b->text.str, L"a1"), 0);

  widget_destroy(b);
}

TEST(Edit, clone) {
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  widget_t* w2 = widget_clone(w1, NULL);

  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
}

TEST(Edit, value) {
  value_t v;
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  widget_set_text(w1, L"123");
  edit_set_input_type(w1, INPUT_INT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_INT32);
  ASSERT_EQ(value_int(&v), 123);

  edit_set_input_type(w1, INPUT_UINT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_UINT32);
  ASSERT_EQ(value_int(&v), 123);

  edit_set_input_type(w1, INPUT_FLOAT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_int(&v), 123);

  edit_set_input_type(w1, INPUT_UFLOAT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_int(&v), 123);

  widget_set_text(w1, L"0xABCD");
  edit_set_input_type(w1, INPUT_HEX);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&v), 0xABCDu);

  // 所有类型的 step 都可用 double 读取
  edit_set_int_limit(w1, -100, 100, 5);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_STEP, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_int(&v), 5);

  edit_set_ufloat_limit(w1, 0, 100, 0.5);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_STEP, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_double(&v), 0.5);

  edit_set_hex_limit(w1, 0, 0xFFFF, 7);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_STEP, &v), RET_OK);
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_int(&v), 7);

  widget_destroy(w1);
}

TEST(Edit, intputing2) {
  key_event_t evt;
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  key_event_init(&evt, EVT_KEY_DOWN, w1, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_dispatch(w1, (event_t*)&evt);
  widget_set_focused(w1, TRUE);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}

TEST(Edit, intputing3) {
  wheel_event_t evt;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* w1 = edit_create(win, 10, 20, 30, 40);
  wheel_event_init(&evt, EVT_WHEEL, w1, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_set_focused(w1, TRUE);
  widget_dispatch(w1, (event_t*)&evt);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}

TEST(Edit, is_valid_password) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_PASSWORD);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 4);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 8);
  widget_set_text_utf8(e, "12");
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_set_text_utf8(e, "1234");
  ASSERT_EQ(edit_is_valid_value(e), TRUE);

  widget_set_text_utf8(e, "1234abcd");
  ASSERT_EQ(edit_is_valid_value(e), TRUE);

  widget_set_text_utf8(e, "1234abcdef");
  ASSERT_EQ(wcscmp(e->text.str, L"1234abcd"), 0);
  ASSERT_EQ(edit_is_valid_value(e), TRUE);

  widget_destroy(e);
  idle_dispatch();
}

TEST(Edit, is_valid_email) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_EMAIL);
  widget_set_prop_int(e, WIDGET_PROP_MIN, 4);
  widget_set_prop_int(e, WIDGET_PROP_MAX, 8);
  widget_set_text_utf8(e, "12");
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_set_text_utf8(e, "12@@@22");
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_set_text_utf8(e, "12@34");
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_set_text_utf8(e, "1234@ab");
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_set_text_utf8(e, "1234a@bcdef");
  ASSERT_EQ(wcscmp(e->text.str, L"1234a@bc"), 0);
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_destroy(e);
  idle_dispatch();
}

TEST(Edit, keys) {
  key_event_t key;
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_ESCAPE);
  ASSERT_EQ(widget_dispatch(e, (event_t*)&key), RET_OK);

  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_F1);
  ASSERT_EQ(widget_dispatch(e, (event_t*)&key), RET_OK);

  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_F10);
  ASSERT_EQ(widget_dispatch(e, (event_t*)&key), RET_OK);

  widget_destroy(e);
}

TEST(Edit, set_text_exceed_max) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_text_limit(e, 0, 3);
  ASSERT_EQ(widget_set_text_utf8(e, "123456"), RET_OK);
  ASSERT_EQ(e->text.size, 3u);

  widget_destroy(e);
}

TEST(Edit, set_double) {
  char text[64];
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_double(e, 10);
  ASSERT_EQ(edit_get_double(e), 10);

  widget_get_text_utf8(e, text, sizeof(text) - 1);
  ASSERT_STREQ(text, "10.000000");

  edit_set_double_ex(e, NULL, 10);
  ASSERT_EQ(edit_get_double(e), 10);
  widget_get_text_utf8(e, text, sizeof(text) - 1);
  ASSERT_STREQ(text, "10.00");

  edit_set_double_ex(e, "%2.1lf", 10);
  ASSERT_EQ(edit_get_double(e), 10);
  widget_get_text_utf8(e, text, sizeof(text) - 1);
  ASSERT_STREQ(text, "10.0");

  widget_destroy(e);
}

TEST(Edit, set_text_for_float) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_float_limit(e, 0.7, 1.3, 0.1);

  widget_set_text_utf8(e, "1.23");
  double v = edit_get_double(e) - 1.23;
  ASSERT_EQ(abs(v) < 0.001, TRUE);

  widget_destroy(e);
}

TEST(Edit, set_props) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  widget_set_props(e, "input_type=int");
  ASSERT_EQ(EDIT(e)->input_type, INPUT_INT);

  widget_set_props(e, "min=10");
  ASSERT_EQ(EDIT(e)->min, 10);

  widget_set_props(e, "max=100");
  ASSERT_EQ(EDIT(e)->max, 100);

  widget_set_props(e, "max=11&min=1");
  ASSERT_EQ(EDIT(e)->min, 1);
  ASSERT_EQ(EDIT(e)->max, 11);

  widget_destroy(e);
}

TEST(Edit, validator) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text(e, L"abcd");
  ASSERT_EQ(edit_is_valid_value(e), TRUE);

  edit_set_validator(e, "len(text) < 3");
  ASSERT_EQ(edit_is_valid_value(e), FALSE);

  widget_destroy(e);
}
