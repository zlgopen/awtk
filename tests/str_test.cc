#include "tkc/str.h"
#include "tkc/utils.h"
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

  ASSERT_EQ(s->size, 0u);

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

TEST(Str, create) {
  str_t* str = str_create(0);

  ASSERT_EQ(str != NULL, true);
  ASSERT_EQ(str->size, 0u);

  ASSERT_EQ(str_set(str, "hello"), RET_OK);
  ASSERT_EQ(str_eq(str, "hello"), TRUE);

  ASSERT_EQ(str_append(str, " world"), RET_OK);
  ASSERT_EQ(str_eq(str, "hello world"), TRUE);

  str_destroy(str);
}

TEST(Str, set_with_len) {
  str_t str;
  str_t* s = NULL;
  char c = 'a';
  s = str_init(&str, 0);

  ASSERT_EQ(str_set_with_len(s, "hello world", 0), RET_OK);
  ASSERT_EQ(str_eq(s, ""), TRUE);

  ASSERT_EQ(str_set_with_len(s, "hello world", 1), RET_OK);
  ASSERT_EQ(str_eq(s, "h"), TRUE);

  ASSERT_EQ(str_set_with_len(s, "hello world", 5), RET_OK);
  ASSERT_EQ(str_eq(s, "hello"), TRUE);

  ASSERT_EQ(str_set_with_len(s, &c, 1), RET_OK);
  ASSERT_EQ(s->size, 1);
  ASSERT_EQ(str_eq(s, "a"), TRUE);

  c = '\0';
  ASSERT_EQ(str_set_with_len(s, &c, 1), RET_OK);
  ASSERT_EQ(s->size, 0);
  ASSERT_EQ(str_eq(s, ""), TRUE);

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

  ASSERT_EQ(str_set(s, "\\a"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\a");

  ASSERT_EQ(str_set(s, "\\b"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\b");

  ASSERT_EQ(str_set(s, "\\e"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\033");

  ASSERT_EQ(str_set(s, "\\f"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\f");

  ASSERT_EQ(str_set(s, "\\v"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\v");

  ASSERT_EQ(str_set(s, "\\?"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\?");

  ASSERT_EQ(str_set(s, "\\0"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "");

  ASSERT_EQ(str_set(s, "\x3a"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), ":");

  ASSERT_EQ(str_set(s, "\x3b"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), ";");

  ASSERT_EQ(str_set(s, "\x2d"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "-");

  ASSERT_EQ(str_set(s, "\x2"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\x2");

  ASSERT_EQ(str_set(s, "\x0"), RET_OK);
  ASSERT_EQ(str_unescape(s), RET_OK);
  ASSERT_EQ(string(s->str), "\0");

  str_reset(s);
}

TEST(Str, expand_vars) {
  str_t str;
  tk_object_t* vars = object_default_create();
  tk_object_set_prop_int(vars, "x", 100);
  tk_object_set_prop_int(vars, "y", 200);
  tk_object_set_prop_int(vars, "w", 300);
  tk_object_set_prop_int(vars, "h", 400);
  tk_object_set_prop_str(vars, "os", "mac");

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
  ASSERT_STREQ(s->str, "abc");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "123${abc}456", vars), RET_OK);
  ASSERT_STREQ(s->str, "123abc456");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "123${}456", vars), RET_OK);
  ASSERT_STREQ(s->str, "123456");

  str_set(s, "");
  ASSERT_EQ(str_expand_vars(s, "123${abc+$x}456", vars), RET_OK);
  ASSERT_STREQ(s->str, "123abc100456");

  tk_object_unref(vars);
  str_reset(s);
}

TEST(Str, from_wstr) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(s->size, 0u);
  str_from_wstr(s, L"123456");
  ASSERT_STREQ(s->str, "123456");

  str_from_wstr_with_len(s, L"123456", 3);
  ASSERT_STREQ(s->str, "123");

  str_from_wstr_with_len(s, L"123456", 0);
  ASSERT_STREQ(s->str, "");

  str_reset(s);
}

TEST(Str, json0) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);

  ASSERT_EQ(str_append_json_str(&str, "\"zhang\r\nshan\""), RET_OK);
  ASSERT_STREQ(str.str, "\"\\\"zhang\\r\\nshan\\\"\"");

  str_reset(s);
}

TEST(Str, json1) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);

  ASSERT_EQ(str_append_json_str(&str, "\\n"), RET_OK);
  ASSERT_STREQ(str.str, "\"\\\\n\"");

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

TEST(Str, encode_hex_basic) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);
  uint8_t data[] = {1, 0x01, 0x0a, 0x2a};
  ASSERT_EQ(str_encode_hex(s, data, sizeof(data), NULL), RET_OK);
  ASSERT_STREQ(s->str, "01010a2a");
  str_reset(s);
}

TEST(Str, encode_hex_upper) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);
  uint8_t data[] = {1, 0x01, 0x0a, 0x2a};
  ASSERT_EQ(str_encode_hex(s, data, sizeof(data), "%02X"), RET_OK);
  ASSERT_STREQ(s->str, "01010A2A");
  str_reset(s);
}

TEST(Str, encode_hex_sep) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);
  uint8_t data[] = {1, 0x01, 0x0a, 0x2a};
  ASSERT_EQ(str_encode_hex(s, data, sizeof(data), "%02X "), RET_OK);
  ASSERT_STREQ(s->str, "01 01 0A 2A ");
  str_reset(s);
}

TEST(Str, encode_hex_sep1) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 100);
  uint8_t data[] = {1, 0x01, 0x0a, 0x2a};
  ASSERT_EQ(str_encode_hex(s, data, sizeof(data), "0x%02X "), RET_OK);
  ASSERT_STREQ(s->str, "0x01 0x01 0x0A 0x2A ");
  str_reset(s);
}

TEST(Str, decode_hex) {
  str_t str;
  ret_t ret;
  uint8_t data[6];

  memset(data, 0, sizeof(data));
  str_init(&str, 100);
  str_append(&str, "fF fe 12    0x65");
  str_decode_hex(&str, data, sizeof(data));
  ASSERT_TRUE(data[0] == 0xff && data[1] == 0xfe && data[2] == 0x12 && data[3] == 0x65);

  str_append(&str, "ya    e8");
  str_decode_hex(&str, data, sizeof(data));
  ASSERT_TRUE(data[0] == 0xff && data[1] == 0xfe && data[2] == 0x12 && data[3] == 0x65);
  ASSERT_TRUE(data[4] == 0 && data[5] == 0xe8);

  ret = str_append(&str, "6b  33");
  ASSERT_EQ(ret, RET_OK);
  str_decode_hex(&str, data, sizeof(data));
  ASSERT_TRUE(data[0] == 0xff && data[1] == 0xfe && data[2] == 0x12 && data[3] == 0x65);
  ASSERT_TRUE(data[4] == 0);
  ASSERT_EQ(data[5], 0xe8);

  str_reset(&str);
}

TEST(Str, append_n_chars) {
  str_t str;

  str_init(&str, 100);

  str_clear(&str);
  ASSERT_EQ(str_append_n_chars(&str, ' ', 1), RET_OK);
  ASSERT_STREQ(str.str, " ");

  str_clear(&str);
  ASSERT_EQ(str_append_n_chars(&str, ' ', 2), RET_OK);
  ASSERT_STREQ(str.str, "  ");

  str_clear(&str);
  ASSERT_EQ(str_append_n_chars(&str, ' ', 5), RET_OK);
  ASSERT_STREQ(str.str, "     ");
  str_reset(&str);
}

TEST(Str, common_prefix) {
  str_t str;

  str_init(&str, 100);

  str_set(&str, "abc.cde.123");
  ASSERT_EQ(str_common_prefix(&str, "abc.123"), RET_OK);
  ASSERT_STREQ(str.str, "abc.");

  str_set(&str, "abc.cde.123");
  ASSERT_EQ(str_common_prefix(&str, "abc.c123"), RET_OK);
  ASSERT_STREQ(str.str, "abc.c");

  str_set(&str, "abc.cde.123");
  ASSERT_EQ(str_common_prefix(&str, "c123"), RET_OK);
  ASSERT_STREQ(str.str, "");

  str_set(&str, "abc.cde.123");
  ASSERT_EQ(str_common_prefix(&str, "abc.cde.123"), RET_OK);
  ASSERT_STREQ(str.str, "abc.cde.123");

  str_set(&str, "");
  ASSERT_EQ(str_common_prefix(&str, "abc.cde.123"), RET_OK);
  ASSERT_STREQ(str.str, "");

  str_reset(&str);
}

TEST(Str, int64) {
  str_t str;
  str_init(&str, 100);
  ASSERT_EQ(str_append_int64(&str, 123567890123), RET_OK);
  ASSERT_STREQ(str.str, "123567890123");
  str_reset(&str);

  ASSERT_EQ(str_append_int64(&str, -123567890123), RET_OK);
  ASSERT_STREQ(str.str, "-123567890123");
  str_reset(&str);
}

TEST(Str, uint64) {
  str_t str;
  str_init(&str, 100);
  ASSERT_EQ(str_append_uint64(&str, 123567890123), RET_OK);
  ASSERT_STREQ(str.str, "123567890123");
  str_reset(&str);

  ASSERT_EQ(str_append_uint64(&str, -1), RET_OK);
  ASSERT_STREQ(str.str, "18446744073709551615");
  str_reset(&str);
}

TEST(Str, uint32) {
  str_t str;
  str_init(&str, 100);
  ASSERT_EQ(str_append_uint32(&str, 123), RET_OK);
  ASSERT_STREQ(str.str, "123");
  str_reset(&str);

  ASSERT_EQ(str_append_uint32(&str, 0xffffffff), RET_OK);
  ASSERT_STREQ(str.str, "4294967295");
  str_reset(&str);
}

TEST(Str, reverse) {
  str_t str;
  str_init(&str, 100);
  str_set(&str, "ABCD");
  ASSERT_EQ(str_reverse(&str), RET_OK);
  ASSERT_STREQ(str.str, "DCBA");

  str_set(&str, "A");
  ASSERT_EQ(str_reverse(&str), RET_OK);
  ASSERT_STREQ(str.str, "A");

  str_set(&str, "AB");
  ASSERT_EQ(str_reverse(&str), RET_OK);
  ASSERT_STREQ(str.str, "BA");

  str_set(&str, "ABC");
  ASSERT_EQ(str_reverse(&str), RET_OK);
  ASSERT_STREQ(str.str, "CBA");

  str_reset(&str);
}

TEST(Str, count) {
  str_t str;
  str_init(&str, 100);
  str_set(&str, "ABCD");
  ASSERT_EQ(str_count(&str, "BC"), 1);
  ASSERT_EQ(str_count(&str, "ABC"), 1);
  ASSERT_EQ(str_count(&str, "ABCD"), 1);
  ASSERT_EQ(str_count(&str, "ABCDE"), 0);

  str_set(&str, "ABCD ABCD");
  ASSERT_EQ(str_count(&str, "BC"), 2);
  ASSERT_EQ(str_count(&str, "ABC"), 2);
  ASSERT_EQ(str_count(&str, "ABCD"), 2);
  ASSERT_EQ(str_count(&str, "ABCDE"), 0);
  str_reset(&str);
}

TEST(Str, format) {
  str_t str;
  str_init(&str, 0);
  str_format(&str, 10, "%d", 123);
  ASSERT_STREQ(str.str, "123");
  ASSERT_EQ(str.size, 3);

  str_format(&str, 10, "%s", "abcd");
  ASSERT_STREQ(str.str, "abcd");
  ASSERT_EQ(str.size, 4);

  str_format(&str, 10, "", "abcd");
  ASSERT_STREQ(str.str, "");
  ASSERT_EQ(str.size, 0);

  str_reset(&str);
}

TEST(Str, append_format) {
  str_t str;
  str_init(&str, 0);
  str_append_format(&str, 10, "%d", 123);
  ASSERT_STREQ(str.str, "123");
  ASSERT_EQ(str.size, 3);

  str_append_format(&str, 10, "%s", "abcd");
  ASSERT_STREQ(str.str, "123abcd");
  ASSERT_EQ(str.size, 7);

  str_append_format(&str, 10, "", "abcd");
  ASSERT_STREQ(str.str, "123abcd");
  ASSERT_EQ(str.size, 7);

  str_reset(&str);
}

TEST(Str, encode_xml_entity) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(str_encode_xml_entity(s, "\"a<b>c\"&&\"a<b>c\"&&"), RET_OK);
  ASSERT_EQ(str_eq(s, "&quot;a&lt;b&gt;c&quot;&amp;&amp;&quot;a&lt;b&gt;c&quot;&amp;&amp;"), TRUE);

  str_reset(s);
}

TEST(Str, from) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(str_from_int(s, 123), RET_OK);
  ASSERT_EQ(str_eq(s, "123"), TRUE);

  ASSERT_EQ(str_from_int(s, -123), RET_OK);
  ASSERT_EQ(str_eq(s, "-123"), TRUE);

  ASSERT_EQ(str_from_uint32(s, 123), RET_OK);
  ASSERT_EQ(str_eq(s, "123"), TRUE);

  ASSERT_EQ(str_from_uint32(s, 0xffffffff), RET_OK);
  ASSERT_EQ(tk_atoul(s->str), 0xffffffff);

  ASSERT_EQ(str_from_int64(s, 1234), RET_OK);
  ASSERT_EQ(str_eq(s, "1234"), TRUE);

  ASSERT_EQ(str_from_int64(s, -1234), RET_OK);
  ASSERT_EQ(str_eq(s, "-1234"), TRUE);

  ASSERT_EQ(str_from_uint64(s, 12345), RET_OK);
  ASSERT_EQ(str_eq(s, "12345"), TRUE);

  ASSERT_EQ(str_from_float(s, 1.1), RET_OK);
  ASSERT_STREQ(s->str, "1.100000");

  str_reset(s);
}

TEST(Str, attach0) {
  str_t s;
  char buff[32];
  str_attach(&s, buff, ARRAY_SIZE(buff));
  str_set(&s, "abc");
  str_append(&s, "123");
}

TEST(Str, attach1) {
  str_t str;
  char buff[64];
  str_t* s = str_attach(&str, buff, sizeof(buff));
  ASSERT_EQ(s != NULL, true);

  str_set(s, "T#");
  str_append_int(s, 1);
  str_append(s, "d");

  str_append_int(s, 2);
  str_append(s, "h");

  str_append_int(s, 3);
  str_append(s, "s");
  ASSERT_STREQ(buff, "T#1d2h3s");
}

TEST(Str, attach2) {
  str_t str;
  char buff[64];
  str_t* s = str_attach(&str, buff, sizeof(buff));
  ASSERT_EQ(s != NULL, true);

  str_set(s, "T#");
  str_append_int(s, 1);
  str_append(s, "d");

  str_append_int(s, 2);
  str_append(s, "h");

  str_append_int(s, 3);
  str_append(s, "s");

  str_reset(s);
  ASSERT_STREQ(buff, "T#1d2h3s");
}

TEST(Str, replace1) {
  str_t str;
  char buff[10];
  str_t* s = str_attach(&str, buff, sizeof(buff));
  ASSERT_EQ(s != NULL, true);

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc", "a"), RET_OK);
  ASSERT_STREQ(buff, "aa123");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc", ""), RET_OK);
  ASSERT_STREQ(buff, "123");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc123", ""), RET_OK);
  ASSERT_STREQ(buff, "abc");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc123", "abc"), RET_OK);
  ASSERT_STREQ(buff, "abcabc");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abcabc123", "abc"), RET_OK);
  ASSERT_STREQ(buff, "abc");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abcabc123", ""), RET_OK);
  ASSERT_STREQ(buff, "");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "", "abc"), RET_BAD_PARAMS);

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc", "abcabc"), RET_FAIL);
}

TEST(Str, replace2) {
  str_t str;
  str_t* s = str_init(&str, 5);
  ASSERT_EQ(s != NULL, true);

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc", "a"), RET_OK);
  ASSERT_STREQ(s->str, "aa123");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc", ""), RET_OK);
  ASSERT_STREQ(s->str, "123");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc123", ""), RET_OK);
  ASSERT_STREQ(s->str, "abc");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc123", "abc"), RET_OK);
  ASSERT_STREQ(s->str, "abcabc");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abcabc123", ""), RET_OK);
  ASSERT_STREQ(s->str, "");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "a", ""), RET_OK);
  ASSERT_STREQ(s->str, "bcbc123");

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "", ""), RET_BAD_PARAMS);

  str_set(s, "abcabc123");
  ASSERT_EQ(str_replace(s, "abc", "abcabc"), RET_OK);
  ASSERT_STREQ(s->str, "abcabcabcabc123");
  ASSERT_EQ(s->size < s->capacity, TRUE);

  str_reset(s);
}

TEST(Str, escape_unescape) {
  str_t str;
  str_t str1;
  str_t* s = str_init(&str, 0);
  str_t* s1 = str_init(&str1, 0);
  
  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "abc", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "abc");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "abc");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "a\\nbc", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "a\nbc");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "a\\nbc");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\ra\\rbc\\r", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\ra\rbc\r");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\ra\\rbc\\r");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\ta\\tbc\\t", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\ta\tbc\t");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\ta\\tbc\\t");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\\\a\\\\bc\\\\", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\\a\\bc\\");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\\\a\\\\bc\\\\");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\a", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\a");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\a");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\b", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\b");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\b");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\e", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\033");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\e");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\f", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\f");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\f");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\v", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\v");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\\v");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\?", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\?");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "?");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\\0", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\x3a", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), ":");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, ":");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\x3b", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), ";");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, ";");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\x2d", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "-");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "-");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\x2", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\x2");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "\x2");

  str_clear(s);str_clear(s1);
  ASSERT_EQ(str_append_unescape(s, "\x0", 0xffff), RET_OK);
  ASSERT_EQ(string(s->str), "\0");
  ASSERT_EQ(str_append_escape(s1, s->str, s->size), RET_OK);
  ASSERT_STREQ(s1->str, "");

  str_reset(s);
  str_reset(s1);
}

TEST(Str, cstr) {
  str_t str;
  str_t* s = str_init(&str, 0);

  ASSERT_EQ(str_append_c_str(s, "a\'b\"c"), RET_OK);
  ASSERT_STREQ(s->str, "\"a\\\'b\\\"c\"");

  str_clear(s);
  ASSERT_EQ(str_append_c_str(s, "abc\r\nd\t"), RET_OK);
  ASSERT_STREQ(s->str, "\"abc\\r\\nd\\t\"");
  
  str_clear(s);
  ASSERT_EQ(str_append_c_str(s, "T#10s"), RET_OK);
  ASSERT_STREQ(s->str, "\"T#10s\"");

  str_reset(s);
}

