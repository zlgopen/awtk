#include "tkc/str.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Str, demo) {
  str_t s;
  str_init(&s, 0);

  str_append(&s, "abc");
  str_append(&s, "123");

  str_reset(&s);
}

TEST(Str, basic) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(s->size, 0);

  ASSERT_EQ(str_set(s, "hello"), RET_OK);
  ASSERT_EQ(str_eq(s, "hello"), TRUE);

  ASSERT_EQ(str_append(s, " world"), RET_OK);
  ASSERT_EQ(str_eq(s, "hello world"), TRUE);

  ASSERT_EQ(str_to_upper(s), RET_OK);
  ASSERT_EQ(str_eq(s, "HELLO WORLD"), TRUE);

  ASSERT_EQ(str_to_lower(s), RET_OK);
  ASSERT_EQ(str_eq(s, "hello world"), TRUE);

  ASSERT_EQ(str_replace(s, "o", "ooo"), RET_OK);
  ASSERT_EQ(str_eq(s, "hellooo wooorld"), TRUE);

  ASSERT_EQ(str_replace(s, "ooo", "xxx"), RET_OK);
  ASSERT_EQ(str_eq(s, "hellxxx wxxxrld"), TRUE);

  ASSERT_EQ(str_trim_left(s, "hel"), RET_OK);
  ASSERT_EQ(str_eq(s, "xxx wxxxrld"), TRUE);

  ASSERT_EQ(str_trim_right(s, "rld"), RET_OK);
  ASSERT_EQ(str_eq(s, "xxx wxxx"), TRUE);

  ASSERT_EQ(str_trim(s, "x "), RET_OK);
  ASSERT_EQ(str_eq(s, "w"), TRUE);

  ASSERT_EQ(str_trim_right(s, "w"), RET_OK);
  ASSERT_EQ(str_eq(s, ""), TRUE);

  ASSERT_EQ(str_set(s, "test.png"), RET_OK);
  ASSERT_EQ(str_start_with(s, "test"), TRUE);
  ASSERT_EQ(str_end_with(s, ".png"), TRUE);

  str_reset(s);
}

TEST(Str, set_with_len) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(str_set_with_len(s, "hello world", 0), RET_OK);
  ASSERT_EQ(str_eq(s, ""), TRUE);

  ASSERT_EQ(str_set_with_len(s, "hello world", 1), RET_OK);
  ASSERT_EQ(str_eq(s, "h"), TRUE);

  ASSERT_EQ(str_set_with_len(s, "hello world", 5), RET_OK);
  ASSERT_EQ(str_eq(s, "hello"), TRUE);

  str_reset(s);
}

TEST(Str, decode_xml_entity) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(str_decode_xml_entity(s, "a&lt;b"), RET_OK);
  ASSERT_EQ(str_eq(s, "a<b"), TRUE);

  ASSERT_EQ(str_decode_xml_entity(s, "a&lt;b&gt;c"), RET_OK);
  ASSERT_EQ(str_eq(s, "a<b>c"), TRUE);

  ASSERT_EQ(str_decode_xml_entity(s, "&quot;a&lt;b&gt;c&quot;"), RET_OK);
  ASSERT_EQ(str_eq(s, "\"a<b>c\""), TRUE);

  ASSERT_EQ(str_decode_xml_entity(s, "&quot;a&lt;b&gt;c&quot;&amp;&amp;"), RET_OK);
  ASSERT_EQ(str_eq(s, "\"a<b>c\"&&"), TRUE);

  str_reset(s);
}

TEST(Str, value) {
  str_t str;
  value_t v;
  str_t* s = str_init(&str, 0);

  value_set_int(&v, 100);
  ASSERT_EQ(str_from_value(s, &v), RET_OK);
  ASSERT_EQ(string(s->str), string("100"));

  value_set_str(&v, "123");
  ASSERT_EQ(str_from_value(s, &v), RET_OK);
  ASSERT_EQ(string(s->str), string("123"));

  value_set_wstr(&v, L"abc");
  ASSERT_EQ(str_from_value(s, &v), RET_OK);
  ASSERT_EQ(string(s->str), string("abc"));

  value_set_float32(&v, 100);
  ASSERT_EQ(str_from_value(s, &v), RET_OK);
  ASSERT_EQ(string(s->str), string("100.000000"));

  value_set_double(&v, 100);
  ASSERT_EQ(str_from_value(s, &v), RET_OK);
  ASSERT_EQ(string(s->str), string("100.000000"));

  str_reset(s);
}

TEST(Str, insert) {
  str_t str;
  str_t* s = str_init(&str, 0);

  ASSERT_EQ(str_insert(s, 0, "abc"), RET_OK);
  ASSERT_EQ(string(s->str), "abc");

  ASSERT_EQ(str_insert(s, 0, "abc"), RET_OK);
  ASSERT_EQ(string(s->str), "abcabc");

  ASSERT_EQ(str_insert(s, 1, "A"), RET_OK);
  ASSERT_EQ(string(s->str), "aAbcabc");

  ASSERT_EQ(str_insert(s, 3, "B"), RET_OK);
  ASSERT_EQ(string(s->str), "aAbBcabc");

  ASSERT_EQ(str_insert(s, 8, "C"), RET_OK);
  ASSERT_EQ(string(s->str), "aAbBcabcC");

  str_reset(s);
}

TEST(Str, remove) {
  str_t str;
  str_t* s = str_init(&str, 0);

  ASSERT_EQ(str_set(s, "abc"), RET_OK);
  ASSERT_EQ(str_remove(s, 0, 3), RET_OK);
  ASSERT_EQ(string(s->str), "");

  ASSERT_EQ(str_set(s, "abc"), RET_OK);
  ASSERT_EQ(str_remove(s, 0, 1), RET_OK);
  ASSERT_EQ(string(s->str), "bc");

  ASSERT_EQ(str_set(s, "abc"), RET_OK);
  ASSERT_EQ(str_remove(s, 1, 1), RET_OK);
  ASSERT_EQ(string(s->str), "ac");

  ASSERT_EQ(str_set(s, "abc"), RET_OK);
  ASSERT_EQ(str_remove(s, 2, 1), RET_OK);
  ASSERT_EQ(string(s->str), "ab");

  str_reset(s);
}

TEST(Str, unescap) {
  str_t str;
  str_t* s = str_init(&str, 0);

  ASSERT_EQ(str_set(s, "\\"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\\");

  ASSERT_EQ(str_set(s, "abc"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "abc");

  ASSERT_EQ(str_set(s, "a\\nbc"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "a\nbc");

  ASSERT_EQ(str_set(s, "\\ra\\rbc\\r"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\ra\rbc\r");

  ASSERT_EQ(str_set(s, "\\ta\\tbc\\t"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\ta\tbc\t");

  ASSERT_EQ(str_set(s, "\\\\a\\\\bc\\\\"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\\a\\bc\\");

  str_reset(s);
}

TEST(Str, expand_vars) {
  str_t str;
  object_t* vars = object_default_create();
  object_set_prop_int(vars, "x", 100);
  object_set_prop_int(vars, "y", 200);
  object_set_prop_int(vars, "w", 300);
  object_set_prop_int(vars, "h", 400);
  object_set_prop_str(vars, "os", "mac");

  str_t* s = str_init(&str, 0);

  ASSERT_EQ(str_expand_vars(s, "${x}", vars), RET_OK);
  ASSERT_STREQ(s->str, "100");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "${x},${y}", vars), RET_OK);
  ASSERT_STREQ(s->str, "100,200");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "img-${x},${y}", vars), RET_OK);
  ASSERT_STREQ(s->str, "img-100,200");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "img-${x},${y},${w},${h}-${os}", vars), RET_OK);
  ASSERT_STREQ(s->str, "img-100,200,300,400-mac");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "${$x+$y}", vars), RET_OK);
  ASSERT_STREQ(s->str, "300");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "${$os+$os}", vars), RET_OK);
  ASSERT_STREQ(s->str, "macmac");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "${($x+$y)+$os}", vars), RET_OK);
  ASSERT_STREQ(s->str, "300mac");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "${abc}", vars), RET_OK);
  ASSERT_STREQ(s->str, "");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "123${abc}456", vars), RET_OK);
  ASSERT_STREQ(s->str, "123456");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "123${}456", vars), RET_OK);
  ASSERT_STREQ(s->str, "123456");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "123${abc+$x}456", vars), RET_OK);
  ASSERT_STREQ(s->str, "123456");

  object_unref(vars);
  str_reset(s);
}

TEST(Str, from_wstr) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(s->size, 0);
  str_from_wstr(s, L"123456");
  ASSERT_STREQ(s->str, "123456");

  str_from_wstr_with_len(s, L"123456", 3);
  ASSERT_STREQ(s->str, "123");

  str_from_wstr_with_len(s, L"123456", 0);
  ASSERT_STREQ(s->str, "");

  str_reset(s);
}

TEST(Str, json) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);

  ASSERT_EQ(str_append_char(&str, '{'), RET_OK);
  ASSERT_EQ(str_append_json_str_pair(&str, "name", "zhangshan"), RET_OK);
  ASSERT_EQ(str_append_char(&str, ','), RET_OK);

  ASSERT_EQ(str_append_json_int_pair(&str, "age", 100), RET_OK);
  ASSERT_EQ(str_append_char(&str, ','), RET_OK);

  ASSERT_EQ(str_append_json_double_pair(&str, "weight", 60.5), RET_OK);
  ASSERT_EQ(str_append_char(&str, ','), RET_OK);

  ASSERT_EQ(str_append_json_bool_pair(&str, "gender", TRUE), RET_OK);
  ASSERT_EQ(str_append_char(&str, '}'), RET_OK);

  ASSERT_STREQ(str.str, "{\"name\":\"zhangshan\",\"age\":100,\"weight\":60.5000,\"gender\":true}");

  str_reset(s);
}

TEST(Str, append_more1) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);
  ASSERT_EQ(str_append_more(s, "123", NULL), RET_OK);
  ASSERT_STREQ(s->str, "123");

  str_reset(s);
}

TEST(Str, append_more2) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);
  ASSERT_EQ(str_append_more(s, "123", "abc", NULL), RET_OK);
  ASSERT_STREQ(s->str, "123abc");

  str_reset(s);
}
