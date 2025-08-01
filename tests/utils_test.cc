#include <string>
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/object_array.h"
#include "gtest/gtest.h"

using std::string;

TEST(Utils, basic) {
  char str[64 + 1];

  ASSERT_EQ(tk_atoi("0015"), 15);
  ASSERT_EQ(tk_atoi("100"), 100);
  ASSERT_EQ(tk_atoi("0xff"), 0xff);
  ASSERT_EQ(tk_atoi("0x1"), 0x1);
  ASSERT_EQ(tk_atoi("0xf"), 0xf);
  ASSERT_EQ(tk_atoi("0Xf"), 0xf);

  ASSERT_EQ(tk_atoi("#f"), 0xf);
  ASSERT_EQ(tk_atoi("#f3"), 0xf3);
  ASSERT_EQ(tk_atoi("#f321"), 0xf321);
  ASSERT_EQ(tk_atoi("#f321"), 0xf321);
  ASSERT_EQ(tk_atoi("#13211234"), 0x13211234);
  ASSERT_EQ(tk_atol("#f"), 0xf);
  ASSERT_EQ(tk_atol("#f3"), 0xf3);
  ASSERT_EQ(tk_atol("#f321"), 0xf321);
  ASSERT_EQ(tk_atol("#f321"), 0xf321);
  ASSERT_EQ(tk_atol("#13211234"), 0x13211234);
  ASSERT_EQ(tk_atoul("#f"), (unsigned long)0xf);
  ASSERT_EQ(tk_atoul("#f3"), (unsigned long)0xf3);
  ASSERT_EQ(tk_atoul("#f321"), (unsigned long)0xf321);
  ASSERT_EQ(tk_atoul("#f321"), (unsigned long)0xf321);
  ASSERT_EQ(tk_atoul("#f3211234"), (unsigned long)0xf3211234);

  ASSERT_EQ(tk_atol("0b11"), 3);
  ASSERT_EQ(tk_atol("0B101"), 5);
  ASSERT_EQ(tk_watoi(L"100"), 100);
  ASSERT_EQ(tk_atof("100"), 100);
  ASSERT_EQ(tk_atof("1e2"), 100);
  ASSERT_EQ(tk_watof(L"100"), 100);

  ASSERT_EQ(tk_atol("0x1122334455667788"), 0x1122334455667788);
  ASSERT_EQ(tk_atol("1122334455667788"), 1122334455667788);
  ASSERT_EQ(tk_atol("-1122334455667788"), -1122334455667788);
  ASSERT_EQ(strcmp(tk_itoa(str, sizeof(str), tk_atoi("100")), "100"), 0);

  ASSERT_EQ(tk_atol("0xFFFFFFFF"), 0xFFFFFFFF);
  ASSERT_EQ(strcmp(tk_lltoa(str, sizeof(str), tk_atol("0xFFFFFFFF")), "4294967295"), 0);
}

static void check_buff16(uint16_t* buff, uint16_t val, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    ASSERT_EQ(buff[i], val);
  }
}

TEST(Utils, tk_memset16) {
  uint16_t buff[32];
  uint16_t val = 0x1234;

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 1);
  check_buff16(buff, val, 1);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 7);
  check_buff16(buff, val, 7);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff + 1, val, 10);
  check_buff16(buff + 1, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff + 1, val, 11);
  check_buff16(buff + 1, val, 11);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 10);
  check_buff16(buff, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 11);
  check_buff16(buff, val, 11);
}

static void check_buff24(uint32_t* buff, uint32_t val, uint32_t size) {
  uint32_t i = 0;
  uint8_t* p = (uint8_t*)buff;
  uint8_t* src = (uint8_t*)&(val);

  for (i = 0; i < size; i++) {
    ASSERT_EQ(p[0], src[0]);
    ASSERT_EQ(p[1], src[1]);
    ASSERT_EQ(p[2], src[2]);
    p += 3;
  }
}

TEST(Utils, tk_memset24) {
  uint32_t buff[32];
  uint32_t val = 0x123456;

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff, &val, 1);
  check_buff24(buff, val, 1);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 5, &val, 7);
  check_buff24(buff + 5, val, 7);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 7, &val, 10);
  check_buff24(buff + 7, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 1, &val, 11);
  check_buff24(buff + 1, val, 11);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 3, &val, 10);
  check_buff24(buff + 3, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff, &val, 11);
  check_buff24(buff, val, 11);
}

static void check_buff32(uint32_t* buff, uint32_t val, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    ASSERT_EQ(buff[i], val);
  }
}

TEST(Utils, tk_memset32) {
  uint32_t buff[32];
  uint32_t val = 0x1234;

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 1);
  check_buff32(buff, val, 1);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 7);
  check_buff32(buff, val, 7);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff + 1, val, 10);
  check_buff32(buff + 1, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff + 1, val, 11);
  check_buff32(buff + 1, val, 11);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 10);
  check_buff32(buff, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 11);
  check_buff32(buff, val, 11);
}

TEST(Utils, tk_memcpy16) {
  uint32_t i = 0;
  uint16_t dst[100];
  uint16_t src[100];

  tk_memset16(src, 0x1234, ARRAY_SIZE(src));
  for (i = 1; i < ARRAY_SIZE(src); i++) {
    tk_memcpy16(dst, src, i);
    ASSERT_EQ(memcmp(dst, src, i * 2), 0);
  }
}

TEST(Utils, tk_memcpy32) {
  uint32_t i = 0;
  uint32_t dst[100];
  uint32_t src[100];

  tk_memset32(src, 0x12345678, ARRAY_SIZE(src));
  for (i = 1; i < ARRAY_SIZE(src); i++) {
    tk_memcpy32(dst, src, i);
    ASSERT_EQ(memcmp(dst, src, i * 4), 0);
  }
}

TEST(Utils, tk_strncpy) {
  char dst[32];
  const char* str = "hello world";

  ASSERT_EQ(string(tk_strncpy(dst, str, 1)), string("h"));
  ASSERT_EQ(string(tk_strncpy(dst, str, strlen(str))), string(str));
  ASSERT_EQ(string(tk_strncpy(dst, str, strlen(str) + 1)), string(str));
  ASSERT_EQ(string(tk_strncpy(dst, str, strlen(str) + 10)), string(str));
  ASSERT_EQ(string(tk_strncpy(dst, str, strlen(str) + 10)), string(str));

  ASSERT_EQ(string(tk_strncpy(dst, dst, strlen(str) + 10)), string(str));
}

TEST(Utils, tk_strncpy_s) {
  char dst[32];
  const char* str = "hello world";

  ASSERT_EQ(tk_strncpy_s(dst, 0, str, 4), (const char*)NULL);
  ASSERT_EQ(string(tk_strncpy_s(dst, 1, str, 4)), string(""));
  ASSERT_EQ(string(tk_strncpy_s(dst, 2, str, 4)), string("h"));
  ASSERT_EQ(string(tk_strncpy_s(dst, 3, str, strlen(str))), string("he"));
  ASSERT_EQ(string(tk_strncpy_s(dst, sizeof(dst), str, strlen(str) + 1)), string(str));
}

TEST(Utils, filename_to_name) {
  char name[TK_NAME_LEN + 1];

  filename_to_name("test.png", name, TK_NAME_LEN);
  ASSERT_EQ(string(name), string("test"));

  filename_to_name("/a/test.png", name, TK_NAME_LEN);
  ASSERT_EQ(string(name), string("test"));
}

TEST(Utils, filename_to_name_ex) {
  char name[TK_NAME_LEN + 1];

  filename_to_name_ex("test.png", name, TK_NAME_LEN, FALSE);
  ASSERT_EQ(string(name), string("test.png"));

  filename_to_name_ex("/a/test.png", name, TK_NAME_LEN, TRUE);
  ASSERT_EQ(string(name), string("test"));
}

TEST(Utils, filename_to_name_ex_dot) {
  char name[TK_NAME_LEN + 1];

  filename_to_name_ex("/a/app.test.png", name, TK_NAME_LEN, TRUE);
  ASSERT_EQ(string(name), string("app.test"));
}

TEST(Utils, tk_skip_to_num) {
  ASSERT_EQ(tk_atoi(tk_skip_to_num("123")), 123);
  ASSERT_EQ(tk_atoi(tk_skip_to_num("abc123")), 123);
  ASSERT_EQ(tk_atoi(tk_skip_to_num("abc:@#123")), 123);
}

TEST(Utils, sscanf) {
  int r = 0;
  int g = 0;
  int b = 0;

  tk_sscanf("#123456", "#%02x%02x%02x", &r, &g, &b);
  ASSERT_EQ(r, 0x12);
  ASSERT_EQ(g, 0x34);
  ASSERT_EQ(b, 0x56);
}

TEST(Utils, xml_file_expand) {
  str_t s;
  str_init(&s, 0);
  const char* filename = "./tests/testdata/main.xml";
  const char* xml_string =
      "<window><?include filename=\"button.xml\"?><?include filename=\"label.xml\"?></window>";
  const char* xml_string_1 =
      "<window><mledit text=\"<?include filename=\"button.xml\"?>\"/><?include "
      "filename=\"label.xml\"?></window>";
  const char* xml_string_2 =
      "<window><mledit text=\"<?include filename=\"button.xml\"?>\"/><?include "
      "filename=\"label.xml\"?><mledit text=\"<?include filename=\"button.xml\"?>\"/></window>";
  const char* xml_string_3 =
      "<window><mledit><property name=\"text\"><?include "
      "filename=\"button.xml\"?></property></mledit><?include filename=\"label.xml\"?></window>";

  ASSERT_EQ(xml_file_expand(filename, &s, xml_string), RET_OK);
  str_replace(&s, "\r\n", "\n");
  ASSERT_EQ(string(s.str), "<window><button />\n<label />\n</window>");
  str_reset(&s);

  str_init(&s, 0);
  ASSERT_EQ(xml_file_expand(filename, &s, xml_string_1), RET_OK);
  str_replace(&s, "\r\n", "\n");
  ASSERT_EQ(string(s.str),
            "<window><mledit text=\"<?include filename=\"button.xml\"?>\"/><label />\n</window>");
  str_reset(&s);

  str_init(&s, 0);
  ASSERT_EQ(xml_file_expand(filename, &s, xml_string_2), RET_OK);
  str_replace(&s, "\r\n", "\n");
  ASSERT_EQ(string(s.str),
            "<window><mledit text=\"<?include filename=\"button.xml\"?>\"/><label />\n<mledit "
            "text=\"<?include filename=\"button.xml\"?>\"/></window>");
  str_reset(&s);

  str_init(&s, 0);
  ASSERT_EQ(xml_file_expand(filename, &s, xml_string_3), RET_OK);
  str_replace(&s, "\r\n", "\n");
  ASSERT_EQ(string(s.str),
            "<window><mledit><property name=\"text\"><button />\n</property></mledit><label "
            "/>\n</window>");
  str_reset(&s);
}

TEST(Utils, xml_file_expand_read) {
  str_t s;
  str_init(&s, 0);

  ASSERT_EQ(xml_file_expand_read("./tests/testdata/main.xml", &s), RET_OK);
  str_replace(&s, "\r\n", "\n");
  ASSERT_EQ(string(s.str), "<window><button />\n<label />\n</window>\n");

  ASSERT_EQ(xml_file_expand_read("./tests/testdata/button.xml", &s), RET_OK);
  str_replace(&s, "\r\n", "\n");
  ASSERT_EQ(string(s.str), "<button />\n");

  str_reset(&s);
}

TEST(Utils, str_append) {
  char str[6];
  memset(str, 0x00, sizeof(str));
  ASSERT_EQ(tk_str_append(str, sizeof(str), "1"), RET_OK);
  ASSERT_EQ(string(str), string("1"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "2"), RET_OK);
  ASSERT_EQ(string(str), string("12"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "3"), RET_OK);
  ASSERT_EQ(string(str), string("123"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "4"), RET_OK);
  ASSERT_EQ(string(str), string("1234"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "5"), RET_OK);
  ASSERT_EQ(string(str), string("12345"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "6"), RET_FAIL);
  ASSERT_EQ(string(str), string("12345"));
}

TEST(Utils, tk_strcpy) {
  char str[TK_NAME_LEN + 1];

  tk_strcpy(str, "abc");
  ASSERT_STREQ(str, "abc");

  tk_strcpy(str, "abc123");
  ASSERT_STREQ(str, "abc123");

  tk_strcpy(str, str);
  ASSERT_STREQ(str, "abc123");
}

TEST(Utils, tk_str_copy) {
  char* prev = NULL;
  char* p = NULL;
  p = tk_str_copy(p, "abc");
  ASSERT_STREQ(p, "abc");

  p = tk_str_copy(p, "abc123");
  ASSERT_STREQ(p, "abc123");

  prev = p;
  p = tk_str_copy(p, p);
  ASSERT_STREQ(p, "abc123");
  ASSERT_EQ(p, prev);

  prev = p;
  p = tk_str_copy(p, p + 3);
  ASSERT_STREQ(p, "123");
  ASSERT_EQ(p, prev);

  TKMEM_FREE(p);
}

TEST(Utils, tk_strdup) {
  char* str = tk_strdup("abc");
  ASSERT_EQ(string(str), "abc");

  TKMEM_FREE(str);
}

TEST(Utils, tk_wstrdup) {
  wchar_t* str = tk_wstrdup(L"abc");
  ASSERT_EQ(memcmp(str, L"abc", sizeof(wchar_t) * 3), 0);

  TKMEM_FREE(str);
}

TEST(Utils, tk_replace_locale) {
  char name[TK_NAME_LEN + 1];
  ASSERT_EQ(tk_replace_locale("test-$locale$", name, "zh_CN"), RET_OK);
  ASSERT_EQ(string(name), string("test-zh_CN"));

  ASSERT_EQ(tk_replace_locale("test-$locale$", name, "zh"), RET_OK);
  ASSERT_EQ(string(name), string("test-zh"));

  ASSERT_EQ(tk_replace_locale("test-$locale$", name, ""), RET_OK);
  ASSERT_EQ(string(name), string("test-"));

  ASSERT_EQ(tk_replace_locale("test", name, "zh"), RET_BAD_PARAMS);
}

TEST(Utils, tk_str_start_with) {
  ASSERT_EQ(tk_str_start_with("abc123", "a"), TRUE);
  ASSERT_EQ(tk_str_start_with("abc123", "ab"), TRUE);
  ASSERT_EQ(tk_str_start_with("abc123", "abc"), TRUE);
  ASSERT_EQ(tk_str_start_with("abc123", ""), TRUE);
  ASSERT_EQ(tk_str_start_with("abc123", "b"), FALSE);
}

TEST(Utils, tk_str_end_with) {
  ASSERT_EQ(tk_str_end_with("abc123", "3"), TRUE);
  ASSERT_EQ(tk_str_end_with("abc123", "23"), TRUE);
  ASSERT_EQ(tk_str_end_with("abc123", "123"), TRUE);
  ASSERT_EQ(tk_str_end_with("abc123", ""), TRUE);
  ASSERT_EQ(tk_str_end_with("abc123", "abc123"), TRUE);
  ASSERT_EQ(tk_str_end_with("abc123", "a"), FALSE);
  ASSERT_EQ(tk_str_end_with("abc123", "aabc123"), FALSE);
}

TEST(Utils, tk_str_case_start_with) {
  ASSERT_EQ(tk_str_case_start_with("abc123", "A"), TRUE);
  ASSERT_EQ(tk_str_case_start_with("abc123", "aB"), TRUE);
  ASSERT_EQ(tk_str_case_start_with("abc123", "abC"), TRUE);
  ASSERT_EQ(tk_str_case_start_with("abc123", ""), TRUE);
  ASSERT_EQ(tk_str_case_start_with("abc123", "b"), FALSE);
}

TEST(Utils, tk_str_case_end_with) {
  ASSERT_EQ(tk_str_case_end_with("abc123abc", "AbC"), TRUE);
  ASSERT_EQ(tk_str_case_end_with("abc123abc", "C"), TRUE);
  ASSERT_EQ(tk_str_case_end_with("abc123abc", "bC"), TRUE);
  ASSERT_EQ(tk_str_case_end_with("abc123", ""), TRUE);
  ASSERT_EQ(tk_str_case_end_with("abc123A", "abc123A"), TRUE);
  ASSERT_EQ(tk_str_case_end_with("abc123A", "a"), TRUE);
  ASSERT_EQ(tk_str_case_end_with("abc123", "aabc123"), FALSE);
}

TEST(Utils, ieq) {
  ASSERT_EQ(strcasecmp("Trigger", "trigger"), 0);
  ASSERT_EQ(tk_str_ieq("Trigger", "trigger"), TRUE);
  ASSERT_EQ(tk_str_ieq("Trigger", "Trigger"), TRUE);
}

TEST(Utils, tk_under_score_to_camel) {
  char name[11];
  ASSERT_EQ(string(tk_under_score_to_camel("test", name, sizeof(name) - 1)), string("test"));
  ASSERT_EQ(string(tk_under_score_to_camel("test_obj", name, sizeof(name) - 1)), string("testObj"));
  ASSERT_EQ(string(tk_under_score_to_camel("test_obj_", name, sizeof(name) - 1)),
            string("testObj"));
  ASSERT_EQ(string(tk_under_score_to_camel("test_obj_get", name, sizeof(name) - 1)),
            string("testObjGet"));
  ASSERT_EQ(string(tk_under_score_to_camel("test_obj_get", name, 7)), string("testObj"));
}

TEST(Utils, strcmp) {
  ASSERT_EQ(tk_str_cmp("abc", "abc") == 0, true);
  ASSERT_NE(tk_str_cmp("abc", "Abc") == 0, true);
  ASSERT_EQ(tk_str_icmp("abc", "abc") == 0, true);
  ASSERT_EQ(tk_str_icmp("abc", "Abc") == 0, true);
}

TEST(Utils, tk_normalize_key_name) {
  char fix_name[TK_NAME_LEN + 1];

  tk_normalize_key_name("a", fix_name);
  ASSERT_STREQ(fix_name, "a");

  tk_normalize_key_name("A", fix_name);
  ASSERT_STREQ(fix_name, "A");

  tk_normalize_key_name("left", fix_name);
  ASSERT_STREQ(fix_name, "LEFT");

  tk_normalize_key_name("LEFT", fix_name);
  ASSERT_STREQ(fix_name, "LEFT");
}

TEST(Utils, tk_str_toupper) {
  char str[TK_NAME_LEN + 1];

  tk_strcpy(str, "left");
  tk_str_toupper(str);
  ASSERT_STREQ(str, "LEFT");

  tk_strcpy(str, "Left");
  tk_str_toupper(str);
  ASSERT_STREQ(str, "LEFT");
}

TEST(Utils, tk_str_tolower) {
  char str[TK_NAME_LEN + 1];

  tk_strcpy(str, "left");
  tk_str_tolower(str);
  ASSERT_STREQ(str, "left");

  tk_strcpy(str, "Left");
  tk_str_tolower(str);
  ASSERT_STREQ(str, "left");
}

TEST(Utils, tk_wstr_count_c) {
  ASSERT_EQ(tk_wstr_count_c(L"", 'a'), 0u);
  ASSERT_EQ(tk_wstr_count_c(L"a", 'a'), 1u);
  ASSERT_EQ(tk_wstr_count_c(L"abcaba", 'a'), 3u);
}

TEST(Utils, tk_count_char) {
  ASSERT_EQ(tk_count_char("", 'a'), 0u);
  ASSERT_EQ(tk_count_char("a", 'a'), 1u);
  ASSERT_EQ(tk_count_char("abcaba", 'a'), 3u);
}

TEST(Utils, tk_wstr_select_word) {
  int32_t left = 0;
  int32_t right = 0;
  wchar_t wstr[] = L"test TEST hello,你好123abc";
  uint32_t len = wcslen(wstr);

  ASSERT_EQ(tk_wstr_select_word(wstr, len, 1, &left, &right), RET_OK);
  ASSERT_EQ(left, 0);
  ASSERT_EQ(right, 4);

  ASSERT_EQ(tk_wstr_select_word(wstr, len, 17, &left, &right), RET_OK);
  ASSERT_EQ(left, 16);
  ASSERT_EQ(right, 18);

  ASSERT_EQ(tk_wstr_select_word(wstr, len, 19, &left, &right), RET_OK);
  ASSERT_EQ(left, 18);
  ASSERT_EQ(right, 21);

  ASSERT_EQ(tk_wstr_select_word(wstr, len, 5, &left, &right), RET_OK);
  ASSERT_EQ(left, 5);
  ASSERT_EQ(right, 9);

  ASSERT_EQ(tk_wstr_select_word(wstr, len, 15, &left, &right), RET_OK);
  ASSERT_EQ(left, 10);
  ASSERT_EQ(right, 15);

  ASSERT_EQ(tk_wstr_select_word(wstr, len, 16, &left, &right), RET_OK);
  ASSERT_EQ(left, 16);
  ASSERT_EQ(right, 18);
}

TEST(Utils, tk_watoi_n) {
  ASSERT_EQ(tk_watoi_n(L"1234", 1), 1);
  ASSERT_EQ(tk_watoi_n(L"1234", 2), 12);
  ASSERT_EQ(tk_watoi_n(L"1234", 3), 123);
  ASSERT_EQ(tk_watoi_n(L"1234", 4), 1234);
}

TEST(Utils, image_region_parse) {
  rect_t r;
  ASSERT_EQ(image_region_parse(100, 100, "xywh(1,2,3,4)", &r), RET_OK);
  ASSERT_EQ(r.x, 1);
  ASSERT_EQ(r.y, 2);
  ASSERT_EQ(r.w, 3);
  ASSERT_EQ(r.h, 4);

  ASSERT_EQ(image_region_parse(200, 200, "xywh(0,0,106,72)", &r), RET_OK);
  ASSERT_EQ(r.x, 0);
  ASSERT_EQ(r.y, 0);
  ASSERT_EQ(r.w, 106);
  ASSERT_EQ(r.h, 72);

  ASSERT_EQ(image_region_parse(100, 100, "grid(4,4,0,0)", &r), RET_OK);
  ASSERT_EQ(r.x, 0);
  ASSERT_EQ(r.y, 0);
  ASSERT_EQ(r.w, 25);
  ASSERT_EQ(r.h, 25);

  ASSERT_EQ(image_region_parse(100, 100, "grid(4,4,1,0)", &r), RET_OK);
  ASSERT_EQ(r.x, 0);
  ASSERT_EQ(r.y, 25);
  ASSERT_EQ(r.w, 25);
  ASSERT_EQ(r.h, 25);

  ASSERT_EQ(image_region_parse(100, 100, "grid(4,4,1,1)", &r), RET_OK);
  ASSERT_EQ(r.x, 25);
  ASSERT_EQ(r.y, 25);
  ASSERT_EQ(r.w, 25);
  ASSERT_EQ(r.h, 25);

  ASSERT_EQ(image_region_parse(100, 100, "grid(4,4,3,3)", &r), RET_OK);
  ASSERT_EQ(r.x, 75);
  ASSERT_EQ(r.y, 75);
  ASSERT_EQ(r.w, 25);
  ASSERT_EQ(r.h, 25);
}

TEST(Utils, to_json) {
  str_t str;
  value_t v;
  tk_object_t* obj = object_default_create();
  tk_object_t* addr = object_default_create();
  tk_object_t* arr = object_array_create();
  tk_object_set_prop_str(obj, "name", "jim");
  tk_object_set_prop_int(obj, "age", 100);

  tk_object_set_prop_int(arr, "-1", 1);
  tk_object_set_prop_int(arr, "-1", 2);
  tk_object_set_prop_str(arr, "-1", "abc");
  value_set_wstr(&v, L"hello");
  tk_object_set_prop(arr, "-1", &v);

  tk_object_set_prop_str(addr, "country", "zh");
  tk_object_set_prop_str(addr, "city", "sz");

  tk_object_set_prop_object(obj, "addr", addr);
  tk_object_set_prop_object(obj, "arr", arr);

  str_init(&str, 1000);
  ASSERT_EQ(object_to_json(obj, &str), RET_OK);
  ASSERT_STREQ(str.str,
               "{\n"
               "  \"addr\":    {\n"
               "      \"city\": \"sz\",\n"
               "      \"country\": \"zh\"\n"
               "   },\n"
               "  \"age\": 100,\n"
               "  \"arr\":    [\n"
               "      1,\n"
               "      2,\n"
               "      \"abc\",\n"
               "      \"hello\"\n"
               "   ],\n"
               "  \"name\": \"jim\"\n"
               "}");

  str_reset(&str);
  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(arr);
  TK_OBJECT_UNREF(addr);
}

TEST(Utils, from_json) {
  tk_object_t* obj = object_default_create();

  ASSERT_EQ(object_from_json(obj,
                             "{\n"
                             "  \"addr\":    {\n"
                             "      \"city\": \"sz\",\n"
                             "      \"country\": \"zh\"\n"
                             "   },\n"
                             "  \"age\": 100,\n"
                             "  \"arr\":    [\n"
                             "      1,\n"
                             "      2,\n"
                             "      \"abc\",\n"
                             "      \"hello\"\n"
                             "   ],\n"
                             "  \"name\": \"jim\"\n"
                             "}"),
            RET_OK);

  ASSERT_STREQ(tk_object_get_prop_str(obj, "name"), "jim");
  ASSERT_EQ(tk_object_get_prop_int(obj, "age", 0), 100);

  ASSERT_EQ(tk_object_get_prop_int(obj, "arr.[0]", 0), 1);
  ASSERT_EQ(tk_object_get_prop_int(obj, "arr.[1]", 0), 2);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "arr.[2]"), "abc");
  ASSERT_STREQ(tk_object_get_prop_str(obj, "arr.[3]"), "hello");

  ASSERT_STREQ(tk_object_get_prop_str(obj, "addr.city"), "sz");
  ASSERT_STREQ(tk_object_get_prop_str(obj, "addr.country"), "zh");

  TK_OBJECT_UNREF(obj);
}

TEST(Utils, strrstr) {
  ASSERT_STREQ(tk_strrstr("abc", "abc"), "abc");
  ASSERT_STREQ(tk_strrstr("1abc", "abc"), "abc");
  ASSERT_STREQ(tk_strrstr("1abc2", "abc"), "abc2");
  ASSERT_STREQ(tk_strrstr("abc abc", "abc"), "abc");
  ASSERT_STREQ(tk_strrstr("abc abc123", "abc"), "abc123");
  ASSERT_STREQ(tk_strrstr("abc abc123aaabc", "abc"), "abc");

  ASSERT_EQ(tk_strrstr("bc", "abc") == NULL, true);
  ASSERT_EQ(tk_strrstr("bc", "123") == NULL, true);
}

TEST(Utils, totitle) {
  char str[] = "it is nice!";
  ASSERT_STREQ(tk_str_totitle(str), "It Is Nice!");
}

TEST(Utils, isspace) {
  uint32_t i = 0;
  const char* str = "（１）这；这个。（２）他（她，它）”，在“，它）";
  size_t n = strlen(str);

  for (i = 0; i < n; i++) {
    tk_isspace(str[i]);
    tk_isdigit(str[i]);
    tk_isalpha(str[i]);
    tk_isprint(str[i]);
    tk_isxdigit(str[i]);
  }

  bool_t is_s = tk_isspace(' ');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isspace('\t');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isspace('\r');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isspace('\n');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isdigit('1');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isdigit('a');
  ASSERT_EQ(is_s, FALSE);
  is_s = tk_isxdigit('a');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isprint('a');
  ASSERT_EQ(is_s, TRUE);
  is_s = tk_isalpha('a');
  ASSERT_EQ(is_s, TRUE);
}

TEST(Utils, is_in_array) {
  const char* arr1[] = {"abc"};
  const char* arr2[] = {"abc", "xyz"};
  const char* arr3[] = {"abc", "xyz", "123"};
  ASSERT_EQ(tk_str_is_in_array("abc", arr1, ARRAY_SIZE(arr1)), TRUE);
  ASSERT_EQ(tk_str_is_in_array("abc", arr2, ARRAY_SIZE(arr2)), TRUE);
  ASSERT_EQ(tk_str_is_in_array("abc", arr3, ARRAY_SIZE(arr3)), TRUE);

  ASSERT_EQ(tk_str_is_in_array("xyz", arr1, ARRAY_SIZE(arr1)), FALSE);
  ASSERT_EQ(tk_str_is_in_array("xyz", arr2, ARRAY_SIZE(arr2)), TRUE);
  ASSERT_EQ(tk_str_is_in_array("xyz", arr3, ARRAY_SIZE(arr3)), TRUE);

  ASSERT_EQ(tk_str_is_in_array("123", arr1, ARRAY_SIZE(arr1)), FALSE);
  ASSERT_EQ(tk_str_is_in_array("123", arr2, ARRAY_SIZE(arr2)), FALSE);
  ASSERT_EQ(tk_str_is_in_array("123", arr3, ARRAY_SIZE(arr3)), TRUE);
}

TEST(Utils, int_is_in_array) {
  int32_t arr1[] = {123};
  int32_t arr2[] = {123, 200};
  int32_t arr3[] = {100, 210, 123};

  ASSERT_EQ(tk_int_is_in_array(123, arr1, ARRAY_SIZE(arr1)), TRUE);
  ASSERT_EQ(tk_int_is_in_array(123, arr2, ARRAY_SIZE(arr2)), TRUE);
  ASSERT_EQ(tk_int_is_in_array(123, arr3, ARRAY_SIZE(arr3)), TRUE);
  ASSERT_EQ(tk_int_is_in_array(23, arr3, ARRAY_SIZE(arr1)), FALSE);
  ASSERT_EQ(tk_int_is_in_array(23, arr3, ARRAY_SIZE(arr3)), FALSE);
}

TEST(Utils, memcpy_by_align_4) {
#define path_max_len 64
  uint8_t path[path_max_len * 2] = {0};
  uint8_t path1[path_max_len * 2] = {0};

  for (uint8_t i = 0; i < path_max_len * 2; i++) {
    path1[i] = i;
  }

  tk_memcpy_by_align_4(path, path1, path_max_len);

  for (uint8_t i = 0; i < path_max_len; i++) {
    ASSERT_EQ(path[i], path1[i]);
  }

  memset(path, 0x0, sizeof(path));

  tk_memcpy_by_align_4(path, path1, path_max_len * 2);

  for (uint8_t i = 0; i < path_max_len * 2; i++) {
    ASSERT_EQ(path[i], path1[i]);
  }
}

TEST(Utils, tk_memcpy) {
#define path_max_len 64
  uint8_t path[path_max_len * 2 + 1] = {0};
  uint8_t path1[path_max_len * 2 + 1] = {0};

  ASSERT_EQ(tk_memcpy(path, path1, 0) == path, true);
  ASSERT_EQ(tk_memcpy(path, NULL, path_max_len) == NULL, true);
  ASSERT_EQ(tk_memcpy(NULL, path1, path_max_len) == NULL, true);

  for (uint8_t i = 0; i < path_max_len * 2; i++) {
    path1[i] = i;
  }

  tk_memcpy(path, path1, path_max_len);

  for (uint8_t i = 0; i < path_max_len; i++) {
    ASSERT_EQ(path[i], path1[i]);
  }

  memset(path, 0x0, sizeof(path));

  tk_memcpy(path, path1, path_max_len * 2);

  for (uint8_t i = 0; i < path_max_len * 2; i++) {
    ASSERT_EQ(path[i], path1[i]);
  }

  memset(path, 0x0, sizeof(path));

  tk_memcpy(path + 1, path1 + 1, path_max_len * 2);

  for (uint8_t i = 1; i < path_max_len * 2; i++) {
    ASSERT_EQ(path[i], path1[i]);
  }

  memset(path, 0x0, sizeof(path));

  tk_memcpy(path, path1 + 1, path_max_len * 2);

  for (uint8_t i = 0; i < path_max_len * 2; i++) {
    ASSERT_EQ(path[i], path1[i + 1]);
  }
}

TEST(Utils, tk_wild_card_match) {
  ASSERT_EQ(tk_wild_card_match("", ""), TRUE);
  ASSERT_EQ(tk_wild_card_match("*", ""), TRUE);
  ASSERT_EQ(tk_wild_card_match("*", "a"), TRUE);
  ASSERT_EQ(tk_wild_card_match("*", "ab"), TRUE);
  ASSERT_EQ(tk_wild_card_match("*c", "abc"), TRUE);
  ASSERT_EQ(tk_wild_card_match("?bc", "abc"), TRUE);
  ASSERT_EQ(tk_wild_card_match("abc", "abc"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a?c", "a?c"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a?c", "abc"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a*c", "a123c"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a*c*", "a123c"), TRUE);
  ASSERT_EQ(tk_wild_card_match("*a*c*", "a123c"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a*", "a123c"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a*", "a"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a*", "ab"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a*", "abc"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a?", "ab"), TRUE);
  ASSERT_EQ(tk_wild_card_match("a?", "abc"), FALSE);
  ASSERT_EQ(tk_wild_card_match("awflow_*", "awflow_123"), TRUE);
  ASSERT_EQ(tk_wild_card_match("awflow_*", "awflow_abc"), TRUE);
}

TEST(Utils, tk_eval_ratio_or_px) {
  ASSERT_EQ(tk_eval_ratio_or_px("0", 100), 0);
  ASSERT_EQ(tk_eval_ratio_or_px("1px", 100), 1);
  ASSERT_EQ(tk_eval_ratio_or_px("10px", 100), 10);
  ASSERT_EQ(tk_eval_ratio_or_px("0.5", 100), 50);
  ASSERT_EQ(tk_eval_ratio_or_px("1", 100), 100);
  ASSERT_EQ(tk_eval_ratio_or_px("100%", 100), 100);
  ASSERT_EQ(tk_eval_ratio_or_px("10%", 100), 10);
  ASSERT_EQ(tk_eval_ratio_or_px("0.1%", 100), 10);
}

TEST(Utils, TK_STR_IS_EMPTY) {
  const char* str = NULL;
  ASSERT_EQ(TK_STR_IS_EMPTY(str), true);
  ASSERT_EQ(TK_STR_IS_EMPTY(""), true);
  ASSERT_EQ(TK_STR_IS_EMPTY("a"), false);

  ASSERT_EQ(TK_STR_IS_NOT_EMPTY(str), false);
  ASSERT_EQ(TK_STR_IS_NOT_EMPTY(""), false);
  ASSERT_EQ(TK_STR_IS_NOT_EMPTY("a"), true);
}

TEST(Utils, tk_strndup) {
  char* s = tk_strndup("hello", 1);
  ASSERT_STREQ(s, "h");
  TKMEM_FREE(s);

  s = tk_strndup("hello", 10);
  ASSERT_STREQ(s, "hello");
  TKMEM_FREE(s);
}

TEST(Utils, tk_memdup) {
  char* s = (char*)tk_memdup("hello", 1);
  ASSERT_STREQ(s, "h");
  TKMEM_FREE(s);

  s = (char*)tk_memdup("hello", 5);
  ASSERT_STREQ(s, "hello");
  TKMEM_FREE(s);

  s = (char*)tk_memdup("hello", 10);
  ASSERT_STREQ(s, "hello");
  TKMEM_FREE(s);
}

TEST(Utils, str_eq) {
  ASSERT_EQ(tk_str_eq(NULL, NULL), TRUE);
  ASSERT_EQ(tk_str_eq("a", NULL), FALSE);
  ASSERT_EQ(tk_str_eq("a", "a"), TRUE);

  ASSERT_EQ(tk_str_ieq(NULL, NULL), TRUE);
  ASSERT_EQ(tk_str_ieq("a", NULL), FALSE);
  ASSERT_EQ(tk_str_ieq("a", "a"), TRUE);
  ASSERT_EQ(tk_str_ieq("a", "A"), TRUE);

  ASSERT_EQ(tk_wstr_eq(NULL, NULL), TRUE);
  ASSERT_EQ(tk_wstr_eq(L"a", NULL), FALSE);
  ASSERT_EQ(tk_wstr_eq(L"a", L"a"), TRUE);
  ASSERT_EQ(tk_wstr_eq(L"a", L"A"), FALSE);
}

TEST(Utils, replace_char) {
  char str[32];

  tk_strcpy(str, "a\\b\\c");
  ASSERT_STREQ(tk_replace_char(str, '\\', '/'), "a/b/c");

  tk_strcpy(str, "a/b/c");
  ASSERT_STREQ(tk_replace_char(str, '\\', '/'), "a/b/c");
}

#include "tkc/thread.h"

TEST(Utils, ui_thread) {
  tk_set_ui_thread(tk_thread_self());
  ASSERT_EQ(tk_is_ui_thread(), TRUE);
}

TEST(Utils, wcsncmp) {
  ASSERT_EQ(wcsncmp(L"ABC", L"A", 1), 0);
  ASSERT_EQ(wcsncmp(L"ABC", L"AB", 1), 0);
  ASSERT_EQ(wcsncmp(L"ABC", L"ABC", 2), 0);
  ASSERT_EQ(wcsncmp(L"ABC", L"ABC", 3), 0);
  ASSERT_EQ(wcsncmp(L"ABCD", L"ABCD", 4), 0);
  ASSERT_EQ(wcsncmp(L"ABCD1234", L"ABCD1234", 7), 0);
  ASSERT_EQ(wcsncmp(L"ABCD1234", L"ABCD1234", 8), 0);
}

TEST(Utils, file_read_as_unix_text) {
  uint32_t size = 0;
  char* data = file_read_as_unix_text("tests/testdata/test.txt", &size);

#define EXPECT_STR \
  "\
abc\n\
123\n\
hello\n\
"
  ASSERT_STREQ(data, EXPECT_STR);

  TKMEM_FREE(data);
}

TEST(Utils, ret_code) {
  ASSERT_STREQ(ret_code_to_name(RET_OK), "RET_OK");
  ASSERT_STREQ(ret_code_to_name(RET_IO), "RET_IO");
  ASSERT_STREQ(ret_code_to_name(RET_FOUND), "RET_FOUND");
  ASSERT_STREQ(ret_code_to_name(RET_NOT_MODIFIED), "RET_NOT_MODIFIED");

  ASSERT_EQ(ret_code_from_name("RET_OK"), RET_OK);
  ASSERT_EQ(ret_code_from_name("RET_NOT_IMPL"), RET_NOT_IMPL);
  ASSERT_EQ(ret_code_from_name("RET_EOS"), RET_EOS);
  ASSERT_EQ(ret_code_from_name("RET_NO_PERMISSION"), RET_NO_PERMISSION);
}

TEST(Utils, bits_stream) {
  uint8_t buff[32];
  uint32_t i = 0;
  uint32_t n = sizeof(buff) * 8;
  bool_t v = FALSE;

  for (i = 0; i < n; ++i) {
    ASSERT_EQ(bits_stream_set(buff, sizeof(buff), i, TRUE), RET_OK);
    ASSERT_EQ(bits_stream_get(buff, sizeof(buff), i, &v), RET_OK);
    ASSERT_EQ(v, TRUE);
  }

  for (i = 0; i < n; ++i) {
    ASSERT_EQ(bits_stream_set(buff, sizeof(buff), i, FALSE), RET_OK);
    ASSERT_EQ(bits_stream_get(buff, sizeof(buff), i, &v), RET_OK);
    ASSERT_EQ(v, FALSE);
  }

  ASSERT_NE(bits_stream_set(buff, sizeof(buff), 10000, TRUE), RET_OK);
  ASSERT_NE(bits_stream_get(buff, sizeof(buff), 10000, &v), RET_OK);
}

TEST(Utils, argv) {
  const wchar_t* wargv[2] = {L"abc", L"123"};
  char** argv = tk_to_utf8_argv(2, (wchar_t**)wargv);
  ASSERT_STREQ(argv[0], "abc");
  ASSERT_STREQ(argv[1], "123");
  tk_free_utf8_argv(2, argv);
}

TEST(Utils, sscanf_simple_d) {
  int d = 0;
  int d1 = 0;
  tk_sscanf_simple("123", "%d", &d);
  ASSERT_EQ(d, 123);

  tk_sscanf_simple("-123", "%d", &d);
  ASSERT_EQ(d, -123);

  tk_sscanf_simple("aaa123", "aaa%d", &d);
  ASSERT_EQ(d, 123);

  tk_sscanf_simple("aaa-123bbb", "aaa%dbbb", &d);
  ASSERT_EQ(d, -123);

  tk_sscanf_simple("aaa123b100", "aaa%db%d", &d, &d1);
  ASSERT_EQ(d, 123);
  ASSERT_EQ(d1, 100);
}

TEST(Utils, sscanf_simple_u) {
  uint32_t u = 0;
  uint32_t u1 = 0;
  tk_sscanf_simple("123", "%u", &u);
  ASSERT_EQ(u, 123);

  tk_sscanf_simple("-123", "%u", &u);
  ASSERT_EQ(u, -123);

  tk_sscanf_simple("aaa123", "aaa%u", &u);
  ASSERT_EQ(u, 123);

  tk_sscanf_simple("aaa-123bbb", "aaa%ubbb", &u);
  ASSERT_EQ(u, -123);

  tk_sscanf_simple("aaa123b100", "aaa%ub%u", &u, &u1);
  ASSERT_EQ(u, 123);
  ASSERT_EQ(u1, 100);
}

TEST(Utils, sscanf_simple_f) {
  float f = 0;
  float f1 = 0;
  tk_sscanf_simple("123", "%f", &f);
  ASSERT_EQ(f, 123);

  tk_sscanf_simple("-123", "%f", &f);
  ASSERT_EQ(f, -123);

  tk_sscanf_simple("aaa123", "aaa%f", &f);
  ASSERT_EQ(f, 123);

  tk_sscanf_simple("aaa-123bbb", "aaa%fbbb", &f);
  ASSERT_EQ(f, -123);
  tk_sscanf_simple("aaa123", "aaa%f", &f);
  ASSERT_EQ(f, 123);

  tk_sscanf_simple("aaa1.23", "aaa%f", &f);
  ASSERT_EQ(tk_fequal(f, 1.23), TRUE);

  tk_sscanf_simple("aaa-12.3bbb", "aaa%fbbb", &f);
  ASSERT_EQ(tk_fequal(f, -12.3), TRUE);

  tk_sscanf_simple("aaa-123bbb", "aaa%fbbb", &f);
  ASSERT_EQ(f, -123);

  tk_sscanf_simple("aaa123b100", "aaa%fb%f", &f, &f1);
  ASSERT_EQ(f, 123);
  ASSERT_EQ(f1, 100);
}

TEST(Utils, sscanf_simple_x) {
  int x = 0;
  int x1 = 0;
  tk_sscanf_simple("123", "%x", &x);
  ASSERT_EQ(x, 0x123);

  tk_sscanf_simple("-123", "%x", &x);
  ASSERT_EQ(x, -0x123);

  tk_sscanf_simple("aaa123", "aaa%x", &x);
  ASSERT_EQ(x, 0x123);

  tk_sscanf_simple("aaa-123:bbb", "aaa%x:bbb", &x);
  ASSERT_EQ(x, -0x123);

  tk_sscanf_simple("aaa123:100", "aaa%x:%x", &x, &x1);
  ASSERT_EQ(x, 0x123);
  ASSERT_EQ(x1, 0x100);

  tk_sscanf_simple("aaaabc:def", "aaa%x:%x", &x, &x1);
  ASSERT_EQ(x, 0xabc);
  ASSERT_EQ(x1, 0xdef);
}

TEST(Utils, sscanf_simple_p) {
  int* p = NULL;
  int* p1 = NULL;
  tk_sscanf_simple("0x123", "%p", &p);
  ASSERT_EQ(tk_pointer_to_int(p), 0x123);

  tk_sscanf_simple("aaa0x123:0x100", "aaa%p:%p", &p, &p1);
  ASSERT_EQ(tk_pointer_to_int(p), 0x123);
  ASSERT_EQ(tk_pointer_to_int(p1), 0x100);

  tk_sscanf_simple("aaa0x123abc:0x100def", "aaa%p:%p", &p, &p1);
  ASSERT_EQ(tk_pointer_to_int(p), 0x123abc);
  ASSERT_EQ(tk_pointer_to_int(p1), 0x100def);
}

TEST(Utils, sscanf_simple_ld) {
  long d = 0;
  long d1 = 0;
  tk_sscanf_simple("123", "%ld", &d);
  ASSERT_EQ(d, 123);

  tk_sscanf_simple("-123", "%ld", &d);
  ASSERT_EQ(d, -123);

  tk_sscanf_simple("aaa123", "aaa%ld", &d);
  ASSERT_EQ(d, 123);

  tk_sscanf_simple("aaa-123bbb", "aaa%ldbbb", &d);
  ASSERT_EQ(d, -123);

  tk_sscanf_simple("aaa123b100", "aaa%ldb%ld", &d, &d1);
  ASSERT_EQ(d, 123);
  ASSERT_EQ(d1, 100);
}

TEST(Utils, sscanf_simple_lu) {
  unsigned long d = 0;
  unsigned long d1 = 0;
  tk_sscanf_simple("123", "%lu", &d);
  ASSERT_EQ(d, 123u);

  tk_sscanf_simple("-123", "%lu", &d);
  ASSERT_EQ(d, -123);

  tk_sscanf_simple("aaa123", "aaa%lu", &d);
  ASSERT_EQ(d, 123u);

  tk_sscanf_simple("aaa-123bbb", "aaa%lubbb", &d);
  ASSERT_EQ(d, -123);

  tk_sscanf_simple("aaa123b100", "aaa%lub%lu", &d, &d1);
  ASSERT_EQ(d, 123u);
  ASSERT_EQ(d1, 100);
}

TEST(Utils, sscanf_simple_lf) {
  double f = 0;
  double f1 = 0;
  tk_sscanf_simple("123", "%lf", &f);
  ASSERT_EQ(f, 123);

  tk_sscanf_simple("12e3", "%lf", &f);
  ASSERT_EQ(f, 12000);

  tk_sscanf_simple("12E3", "%lf", &f);
  ASSERT_EQ(f, 12000);

  tk_sscanf_simple("12E-3", "%lf", &f);
  ASSERT_EQ(f, 0.012);

  tk_sscanf_simple("-123", "%lf", &f);
  ASSERT_EQ(f, -123);

  tk_sscanf_simple("aaa123", "aaa%lf", &f);
  ASSERT_EQ(f, 123);

  tk_sscanf_simple("aaa-123bbb", "aaa%lfbbb", &f);
  ASSERT_EQ(f, -123);
  tk_sscanf_simple("aaa123", "aaa%lf", &f);
  ASSERT_EQ(f, 123);

  tk_sscanf_simple("aaa1.23", "aaa%lf", &f);
  ASSERT_EQ(tk_fequal(f, 1.23), TRUE);

  tk_sscanf_simple("aaa-12.3bbb", "aaa%lfbbb", &f);
  ASSERT_EQ(tk_fequal(f, -12.3), TRUE);

  tk_sscanf_simple("aaa-123bbb", "aaa%lfbbb", &f);
  ASSERT_EQ(f, -123);

  tk_sscanf_simple("aaa123b100", "aaa%lfb%lf", &f, &f1);
  ASSERT_EQ(f, 123);
  ASSERT_EQ(f1, 100);
}

TEST(Utils, sscanf_simple_nx) {
  int x = 0;
  int x1 = 0;
  tk_sscanf_simple("123", "%2x", &x);
  ASSERT_EQ(x, 0x12);

  tk_sscanf_simple("aaa123", "aaa%3x", &x);
  ASSERT_EQ(x, 0x123);

  tk_sscanf_simple("aaa1234", "aaa%02x%02x", &x, &x1);
  ASSERT_EQ(x, 0x12);
  ASSERT_EQ(x1, 0x34);

  tk_sscanf_simple("aaaabcd", "aaa%02x%02x", &x, &x1);
  ASSERT_EQ(x, 0xab);
  ASSERT_EQ(x1, 0xcd);
}

TEST(Utils, sscanf_simple_nd) {
  int d = 0;
  int d1 = 0;
  tk_sscanf_simple("123", "%2d", &d);
  ASSERT_EQ(d, 12);

  tk_sscanf_simple("aaa123", "aaa%3d", &d);
  ASSERT_EQ(d, 123);

  tk_sscanf_simple("aaa1234", "aaa%02d%02d", &d, &d1);
  ASSERT_EQ(d, 12);
  ASSERT_EQ(d1, 34);
}

TEST(Utils, sscanf_simple_nu) {
  int u = 0;
  int u1 = 0;
  tk_sscanf_simple("123", "%2u", &u);
  ASSERT_EQ(u, 12);

  tk_sscanf_simple("aaa123", "aaa%3u", &u);
  ASSERT_EQ(u, 123);

  tk_sscanf_simple("aaa1234", "aaa%02u%02u", &u, &u1);
  ASSERT_EQ(u, 12);
  ASSERT_EQ(u1, 34);
}

TEST(Utils, sscanf_simple_ns) {
  char str[256] = {0};
  tk_sscanf_simple("123abc", "123%3s", &str);
  ASSERT_STREQ(str, "abc");

  tk_sscanf_simple("123abcd", "123%3s", &str);
  ASSERT_STREQ(str, "abc");

  ASSERT_EQ(tk_sscanf_simple("123abcd", "123%30s", &str), 1);
  ASSERT_STREQ(str, "abcd");
}

TEST(Utils, sscanf_simple1) {
  uint32_t y = 0;
  uint32_t m = 0;
  uint32_t d = 0;
  int n = tk_sscanf_simple("#2023-10-1", "#%u-%u-%u", &y, &m, &d);
  ASSERT_EQ(n, 3);
  ASSERT_EQ(y, 2023);
  ASSERT_EQ(m, 10);
  ASSERT_EQ(d, 1);
}

TEST(Utils, sscanf_simple2) {
  uint32_t y = 0;
  uint32_t m = 0;
  uint32_t d = 0;
  uint32_t h = 0;
  uint32_t minute = 0;
  double s = 0;

  int n = tk_sscanf("#2022-10-1-12:10:1.23", "#%u-%u-%u-%u:%u:%lf", &y, &m, &d, &h, &minute, &s);
  ASSERT_EQ(n, 6);
  ASSERT_EQ(y, 2022);
  ASSERT_EQ(m, 10);
  ASSERT_EQ(d, 1);
  ASSERT_EQ(h, 12);
  ASSERT_EQ(minute, 10);
  ASSERT_EQ(s, 1.23);

  ASSERT_EQ(tk_isdigit('-'), FALSE);
  ASSERT_EQ(tk_isdigit('+'), FALSE);
}

TEST(Utils, levelize1) {
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", -1), 0);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 0), 0);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 10), 0);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 20), 0);

  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 21), 1);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 30), 1);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 40), 1);

  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 41), 2);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 50), 2);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 60), 2);

  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 61), 3);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 70), 3);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 80), 3);

  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 81), 4);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 90), 4);
  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 100), 4);

  ASSERT_EQ(tk_levelize("0-20;21-40;41-60;61-80;81-100", 1000), 4);
}

TEST(Utils, levelize2) {
  ASSERT_EQ(tk_levelize("0-20", -1), 0);
  ASSERT_EQ(tk_levelize("0-20", 0), 0);
  ASSERT_EQ(tk_levelize("0-20", 10), 0);
  ASSERT_EQ(tk_levelize("0-20", 100), 0);
}

TEST(Utils, levelize3) {
  ASSERT_EQ(tk_levelize("0-20;21-40", -1), 0);
  ASSERT_EQ(tk_levelize("0-20;21-40", 30), 1);
  ASSERT_EQ(tk_levelize("0-20;21-40", 100), 1);
}

TEST(Utils, tk_date_time_format) {
  str_t str;
  char buff[128];

  str_attach(&str, buff, sizeof(buff));
  ASSERT_EQ(tk_date_time_format(0, "", &str), RET_OK);
  ASSERT_STREQ(str.str, "");

  ASSERT_EQ(tk_date_time_format(0, "Y", &str), RET_OK);
  ASSERT_STREQ(str.str, "1970");

  ASSERT_EQ(tk_date_time_format(0, "Y-M-D", &str), RET_OK);
  ASSERT_STREQ(str.str, "1970-1-1");

  ASSERT_EQ(tk_date_time_format(0, "Y-M-D h:m:s", &str), RET_OK);
  ASSERT_STREQ(str.str, "1970-1-1 0:0:0");

  ASSERT_EQ(tk_date_time_format(0, "YY", &str), RET_OK);
  ASSERT_STREQ(str.str, "70");

  ASSERT_EQ(tk_date_time_format(0, "YY-MM-DD", &str), RET_OK);
  ASSERT_STREQ(str.str, "70-01-01");

  ASSERT_EQ(tk_date_time_format(0, "YY-MM-DD hh:mm:ss", &str), RET_OK);
  ASSERT_STREQ(str.str, "70-01-01 00:00:00");

  ASSERT_EQ(tk_date_time_format(3765, "YY-MM-DD hh:mm:ss", &str), RET_OK);
  ASSERT_STREQ(str.str, "70-01-01 01:02:45");
}

TEST(Utils, upper) {
  ASSERT_EQ(tk_isupper('A'), TRUE);
  ASSERT_EQ(tk_isupper('a'), FALSE);
  ASSERT_EQ(tk_isupper('1'), FALSE);
  ASSERT_EQ(tk_isupper('Z'), TRUE);
  ASSERT_EQ(tk_isupper('z'), FALSE);
  ASSERT_EQ(tk_isupper(','), FALSE);
  ASSERT_EQ(tk_isupper(' '), FALSE);
}

TEST(Utils, lower) {
  ASSERT_EQ(tk_islower('A'), FALSE);
  ASSERT_EQ(tk_islower('a'), TRUE);
  ASSERT_EQ(tk_islower('1'), FALSE);
  ASSERT_EQ(tk_islower('Z'), FALSE);
  ASSERT_EQ(tk_islower('z'), TRUE);
  ASSERT_EQ(tk_islower(','), FALSE);
  ASSERT_EQ(tk_islower(' '), FALSE);
}

TEST(Utils, alpha) {
  ASSERT_EQ(tk_isalpha('A'), TRUE);
  ASSERT_EQ(tk_isalpha('a'), TRUE);
  ASSERT_EQ(tk_isalpha('1'), FALSE);
  ASSERT_EQ(tk_isalpha('Z'), TRUE);
  ASSERT_EQ(tk_isalpha('z'), TRUE);
  ASSERT_EQ(tk_isalpha(','), FALSE);
  ASSERT_EQ(tk_isalpha(' '), FALSE);
}

TEST(Utils, bits_data_byte_data) {
  uint16_t data = 0;
  uint8_t output[16];
  uint8_t bytes[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
  uint8_t* bits = (uint8_t*)&data;

  memset(output, 0, sizeof(output));
  ASSERT_EQ(tk_bits_data_from_bytes_data(bits, sizeof(data), bytes, 2), RET_OK);
  ASSERT_EQ(data, 0x02);
  ASSERT_EQ(tk_bits_data_to_bytes_data(bits, sizeof(data), output, 2), RET_OK);
  ASSERT_EQ(memcmp(output, bytes, 2), 0);

  ASSERT_EQ(tk_bits_data_from_bytes_data(bits, sizeof(data), bytes, 8), RET_OK);
  ASSERT_EQ(data, 0x0aa);
  ASSERT_EQ(tk_bits_data_to_bytes_data(bits, sizeof(data), output, 8), RET_OK);
  ASSERT_EQ(memcmp(output, bytes, 8), 0);

  ASSERT_EQ(tk_bits_data_from_bytes_data(bits, sizeof(data), bytes, sizeof(bytes)), RET_OK);
  ASSERT_EQ(data, 0xaaaa);
  ASSERT_EQ(tk_bits_data_to_bytes_data(bits, sizeof(data), output, 16), RET_OK);
  ASSERT_EQ(memcmp(output, bytes, 16), 0);
}

TEST(Utils, buffer_get_value_int8) {
  value_t v;
  uint8_t buffer[] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
  value_set_int(&v, 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT8, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT8, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_uint8(&v), 0xaa);
}

TEST(Utils, buffer_set_value_int8) {
  value_t v;
  uint8_t buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};
  value_set_int(&v, 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b11);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0b11);

  value_set_int(&v, 0xaa);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0xaa);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT8, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0xaa);

  value_set_int(&v, 0);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT8, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT8, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b10);
}

TEST(Utils, buffer_get_value_int16) {
  value_t v;
  uint16_t buffer[] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
  value_set_int(&v, 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT16, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT16, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_uint16(&v), 0xaa);
}

TEST(Utils, buffer_set_value_int16) {
  value_t v;
  uint16_t buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};
  value_set_int(&v, 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b11);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0b11);

  value_set_int(&v, 0xaa);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0xaa);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT16, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0xaa);

  value_set_int(&v, 0);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT16, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT16, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b10);
}

TEST(Utils, buffer_get_value_int32) {
  value_t v;
  uint32_t buffer[] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
  value_set_int(&v, 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT32, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT32, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_uint32(&v), 0xaa);
}

TEST(Utils, buffer_set_value_int32) {
  value_t v;
  uint32_t buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};
  value_set_int(&v, 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b11);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0b11);

  value_set_int(&v, 0xaa);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0xaa);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT32, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0xaa);

  value_set_int(&v, 0);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT32, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT32, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b10);
}

TEST(Utils, buffer_get_value_int64) {
  value_t v;
  uint64_t buffer[] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
  value_set_int(&v, 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 0xaa);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT64, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT64, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_uint64(&v), 0xaa);
}

TEST(Utils, buffer_set_value_int64) {
  value_t v;
  uint64_t buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};
  value_set_int(&v, 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 0, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b11);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 1, 0, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 1);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 1, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0b11);

  value_set_int(&v, 0xaa);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0xaa);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_UINT64, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0xaa);

  value_set_int(&v, 0);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT64, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0);

  value_set_int(&v, 1);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_INT64, 0, 1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0b10);
}

TEST(Utils, buffer_get_value_float) {
  value_t v;
  float buffer[] = {0, 1, 2, 3, 4, 5, 6};
  value_set_int(&v, 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_FLOAT32, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(value_float(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_FLOAT32, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), buffer[4]);
}

TEST(Utils, buffer_set_value_float) {
  value_t v;
  float buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};
  value_set_int(&v, 0);

  value_set_int(&v, 0xaa);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_FLOAT32, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0xaa);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_FLOAT32, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0xaa);
}

TEST(Utils, buffer_get_value_double) {
  value_t v;
  double buffer[] = {0, 1, 2, 3, 4, 5, 6};
  value_set_int(&v, 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_DOUBLE, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(value_double(&v), 0);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_DOUBLE, 4, -1, &v),
            RET_OK);
  ASSERT_EQ(value_int(&v), buffer[4]);
}

TEST(Utils, buffer_get_value_double_unaligned) {
  value_t v;
  double buffer[] = {0, 1, 2, 3, 4, 5, 6};

  value_set_double(&v, 123);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer + 1, sizeof(buffer), VALUE_TYPE_DOUBLE, 1, -1, &v),
            RET_OK);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer + 1, sizeof(buffer), VALUE_TYPE_DOUBLE, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(value_double(&v), 123);
}

TEST(Utils, buffer_get_value_str) {
  value_t v;
  char buffer[256];
  memcpy(buffer, "abc", 4);
  memcpy(buffer + 4, "123", 4);

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_STRING, 0, -1, &v),
            RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_STRING, 4, -1, &v),
            RET_OK);
  ASSERT_STREQ(value_str(&v), "123");
}

TEST(Utils, buffer_get_value_bin) {
  value_t v;
  char buffer[256];
  memcpy(buffer, "abc", 4);
  memcpy(buffer + 4, "123", 4);
  binary_data_t* b = NULL;
  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_BINARY, 0, -1, &v),
            RET_OK);

  b = value_binary_data(&v);
  ASSERT_STREQ((char*)(b->data), "abc");
  ASSERT_EQ(b->size, sizeof(buffer));

  ASSERT_EQ(tk_buffer_get_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_BINARY, 4, -1, &v),
            RET_OK);

  b = value_binary_data(&v);
  ASSERT_STREQ((char*)(b->data), "123");
  ASSERT_EQ(b->size, sizeof(buffer) - 4);
}

TEST(Utils, buffer_set_value_double) {
  value_t v;
  double buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};
  value_set_int(&v, 0);

  value_set_int(&v, 0xaa);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_DOUBLE, 0, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[0], 0xaa);

  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_DOUBLE, 1, -1, &v),
            RET_OK);
  ASSERT_EQ(buffer[1], 0xaa);
}

TEST(Utils, buffer_set_value_str) {
  value_t v;
  char buffer[256];

  memset(buffer, 0x00, sizeof(buffer));

  value_set_str(&v, "abc");
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_STRING, 0, -1, &v),
            RET_OK);
  ASSERT_STREQ(buffer, "abc");

  value_set_str(&v, "123");
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_STRING, 3, -1, &v),
            RET_OK);
  ASSERT_STREQ(buffer, "abc123");
}

TEST(Utils, buffer_set_value_bin) {
  value_t v;
  char buffer[256];

  memset(buffer, 0x00, sizeof(buffer));

  value_set_binary_data(&v, (void*)"abc", 3);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_BINARY, 0, -1, &v),
            RET_OK);
  ASSERT_STREQ(buffer, "abc");

  value_set_binary_data(&v, (void*)"123", 3);
  ASSERT_EQ(tk_buffer_set_value((uint8_t*)buffer, sizeof(buffer), VALUE_TYPE_BINARY, 3, -1, &v),
            RET_OK);
  ASSERT_STREQ(buffer, "abc123");
}

TEST(Utils, tk_basic_type_from_name) {
  ASSERT_EQ(tk_basic_type_from_name("int8"), VALUE_TYPE_INT8);
  ASSERT_EQ(tk_basic_type_from_name("uint8"), VALUE_TYPE_UINT8);
  ASSERT_EQ(tk_basic_type_from_name("BYTE"), VALUE_TYPE_UINT8);
  ASSERT_EQ(tk_basic_type_from_name("int16"), VALUE_TYPE_INT16);
  ASSERT_EQ(tk_basic_type_from_name("uint16"), VALUE_TYPE_UINT16);
  ASSERT_EQ(tk_basic_type_from_name("WORD"), VALUE_TYPE_UINT16);
  ASSERT_EQ(tk_basic_type_from_name("int32"), VALUE_TYPE_INT32);
  ASSERT_EQ(tk_basic_type_from_name("uint32"), VALUE_TYPE_UINT32);
  ASSERT_EQ(tk_basic_type_from_name("dWORD"), VALUE_TYPE_UINT32);
  ASSERT_EQ(tk_basic_type_from_name("int64"), VALUE_TYPE_INT64);
  ASSERT_EQ(tk_basic_type_from_name("uint64"), VALUE_TYPE_UINT64);
  ASSERT_EQ(tk_basic_type_from_name("LWORD"), VALUE_TYPE_UINT64);
  ASSERT_EQ(tk_basic_type_from_name("float"), VALUE_TYPE_FLOAT32);
  ASSERT_EQ(tk_basic_type_from_name("double"), VALUE_TYPE_DOUBLE);
  ASSERT_EQ(tk_basic_type_from_name("bool"), VALUE_TYPE_BOOL);

  ASSERT_EQ(tk_basic_type_from_name("int8_t"), VALUE_TYPE_INT8);
  ASSERT_EQ(tk_basic_type_from_name("uint8_t"), VALUE_TYPE_UINT8);
  ASSERT_EQ(tk_basic_type_from_name("int16_t"), VALUE_TYPE_INT16);
  ASSERT_EQ(tk_basic_type_from_name("uint16_t"), VALUE_TYPE_UINT16);
  ASSERT_EQ(tk_basic_type_from_name("int32_t"), VALUE_TYPE_INT32);
  ASSERT_EQ(tk_basic_type_from_name("uint32_t"), VALUE_TYPE_UINT32);
  ASSERT_EQ(tk_basic_type_from_name("int64_t"), VALUE_TYPE_INT64);
  ASSERT_EQ(tk_basic_type_from_name("uint64_t"), VALUE_TYPE_UINT64);
  ASSERT_EQ(tk_basic_type_from_name("float_t"), VALUE_TYPE_FLOAT32);
  ASSERT_EQ(tk_basic_type_from_name("double_t"), VALUE_TYPE_DOUBLE);
  ASSERT_EQ(tk_basic_type_from_name("bool_t"), VALUE_TYPE_BOOL);
  ASSERT_EQ(tk_basic_type_from_name("bin"), VALUE_TYPE_BINARY);
  ASSERT_EQ(tk_basic_type_from_name("str"), VALUE_TYPE_STRING);
  ASSERT_EQ(tk_basic_type_from_name("unknown"), VALUE_TYPE_INVALID);
}

TEST(Utils, tk_distance) {
  ASSERT_EQ(tk_distance(0, 0, 0, 0), 0);
  ASSERT_EQ(tk_distance(0, 0, 1, 0), 1);
  ASSERT_EQ(tk_distance(0, 0, 0, 1), 1);
  ASSERT_EQ(tk_distance(0, 0, 1, 1), 1);
  ASSERT_EQ(tk_distance(0, 0, 1, 2), 2);
  ASSERT_EQ(tk_distance(0, 0, 2, 1), 2);
  ASSERT_EQ(tk_distance(0, 0, 2, 2), 2);
  ASSERT_EQ(tk_distance(0, 0, 3, 4), 5);
  ASSERT_EQ(tk_distance(0, 0, 4, 3), 5);
  ASSERT_EQ(tk_distance(0, 0, 4, 4), 5);
  ASSERT_EQ(tk_distance(0, 0, 5, 5), 7);
  ASSERT_EQ(tk_distance(0, 0, 6, 6), 8);
  ASSERT_EQ(tk_distance(0, 0, 8, 8), 11);
}

TEST(Utils, tk_value_to_angle) {
  int32_t v = TK_R2D(tk_value_to_angle(50, 0, 100, 0, 360, FALSE));
  ASSERT_EQ(v, 180);

  v = TK_R2D(tk_value_to_angle(0, 0, 100, 0, 360, FALSE));
  ASSERT_EQ(v, 0);

  v = TK_R2D(tk_value_to_angle(100, 0, 100, 0, 360, FALSE));
  ASSERT_EQ(v, 360);

  v = round(TK_R2D(tk_value_to_angle(50, 0, 100, 0, 360, TRUE)));
  ASSERT_EQ(v, 180);

  v = TK_R2D(tk_value_to_angle(0, 0, 100, 0, 360, TRUE));
  ASSERT_EQ(v, 360);

  v = TK_R2D(tk_value_to_angle(100, 0, 100, 0, 360, TRUE));
  ASSERT_EQ(v, 0);
}

TEST(Utils, tk_angle) {
  int32_t v = TK_R2D(tk_angle(0, 0, 100, 0));
  ASSERT_EQ(v, 0);

  v = TK_R2D(tk_angle(0, 0, 0, 90));
  ASSERT_EQ(v, 270);

  v = TK_R2D(tk_angle(0, 0, 0, -90));
  ASSERT_EQ(v, 90);

  v = TK_R2D(tk_angle(0, 0, -180, 0));
  ASSERT_EQ(v, 180);

  v = tk_roundi(TK_R2D(tk_angle(0, 0, 90, 90)));
  ASSERT_EQ(v, 315);
}

TEST(Utils, skip_chars) {
  ASSERT_STREQ(tk_skip_chars("abc123", "abc"), "123");
  ASSERT_STREQ(tk_skip_chars("abc123", ""), "abc123");
  ASSERT_STREQ(tk_skip_chars("abc123", "A"), "abc123");
  ASSERT_STREQ(tk_skip_chars("abc123", "a"), "bc123");
}

TEST(Utils, skip_to_chars) {
  ASSERT_STREQ(tk_skip_to_chars("abc123", "123"), "123");
  ASSERT_STREQ(tk_skip_to_chars("abc123", "21"), "123");
  ASSERT_STREQ(tk_skip_to_chars("abc123", ""), "");
  ASSERT_STREQ(tk_skip_to_chars("abc123", "A"), "");
  ASSERT_STREQ(tk_skip_to_chars("abc123", "a"), "abc123");
}

static int tk_compare_int32(const void* a, const void* b) {
  return *(int*)a - *(int*)b;
}

typedef struct _person_t {
  int age;
  char name[32];
} person_t;

static int person_cmp_age(const void* a, const void* b) {
  return ((person_t*)a)->age - ((person_t*)b)->age;
}

TEST(Utils, mergesort1) {
  int32_t arr[] = {38, 27, 43, 3, 9, 82, 10};
  tk_mergesort(arr, ARRAY_SIZE(arr), sizeof(arr[0]), tk_compare_int32);

  ASSERT_EQ(arr[0], 3);
  ASSERT_EQ(arr[1], 9);
  ASSERT_EQ(arr[2], 10);
  ASSERT_EQ(arr[3], 27);
  ASSERT_EQ(arr[4], 38);
  ASSERT_EQ(arr[5], 43);
  ASSERT_EQ(arr[6], 82);
}

TEST(Utils, mergesort2) {
  person_t arr[] = {{38, "a"}, {27, "b"}, {43, "c"}, {3, "d"}, {9, "e"}, {82, "f"}, {10, "g"}};
  tk_mergesort(arr, ARRAY_SIZE(arr), sizeof(arr[0]), person_cmp_age);

  ASSERT_EQ(arr[0].age, 3);
  ASSERT_EQ(arr[1].age, 9);
  ASSERT_EQ(arr[2].age, 10);
  ASSERT_EQ(arr[3].age, 27);
  ASSERT_EQ(arr[4].age, 38);
  ASSERT_EQ(arr[5].age, 43);
  ASSERT_EQ(arr[6].age, 82);
}

TEST(Utils, atob) {
  ASSERT_EQ(tk_atob("FALSE"), FALSE);
  ASSERT_EQ(tk_atob("False"), FALSE);
  ASSERT_EQ(tk_atob("F"), FALSE);
  ASSERT_EQ(tk_atob("f"), FALSE);

  ASSERT_EQ(tk_atob("\"FALSE\""), TRUE);
  ASSERT_EQ(tk_atob("'FALSE'"), TRUE);
  ASSERT_EQ(tk_atob(" FALSE "), TRUE);

  ASSERT_EQ(tk_atob("TRUE"), TRUE);
  ASSERT_EQ(tk_atob("True"), TRUE);
  ASSERT_EQ(tk_atob("T"), TRUE);
  ASSERT_EQ(tk_atob("t"), TRUE);

  ASSERT_EQ(tk_atob("\"TRUE\""), TRUE);
  ASSERT_EQ(tk_atob("'TRUE'"), TRUE);
  ASSERT_EQ(tk_atob(" TRUE "), TRUE);

  ASSERT_EQ(tk_atob(NULL), FALSE);
  ASSERT_EQ(tk_atob(""), FALSE);
  ASSERT_EQ(tk_atob(" "), TRUE);

  ASSERT_EQ(tk_atob("0"), FALSE);
  ASSERT_EQ(tk_atob("00"), TRUE);
  ASSERT_EQ(tk_atob("0AWTK"), TRUE);
  ASSERT_EQ(tk_atob("1"), TRUE);
  ASSERT_EQ(tk_atob("-1"), TRUE);
  ASSERT_EQ(tk_atob("2"), TRUE);

  ASSERT_EQ(tk_atob("abcd"), TRUE);
}

TEST(Utils, watob) {
  ASSERT_EQ(tk_watob(L"FALSE"), FALSE);
  ASSERT_EQ(tk_watob(L"False"), FALSE);
  ASSERT_EQ(tk_watob(L"F"), FALSE);
  ASSERT_EQ(tk_watob(L"f"), FALSE);

  ASSERT_EQ(tk_watob(L"\"FALSE\""), TRUE);
  ASSERT_EQ(tk_watob(L"'FALSE'"), TRUE);
  ASSERT_EQ(tk_watob(L" FALSE "), TRUE);

  ASSERT_EQ(tk_watob(L"TRUE"), TRUE);
  ASSERT_EQ(tk_watob(L"True"), TRUE);
  ASSERT_EQ(tk_watob(L"T"), TRUE);
  ASSERT_EQ(tk_watob(L"t"), TRUE);

  ASSERT_EQ(tk_watob(L"\"TRUE\""), TRUE);
  ASSERT_EQ(tk_watob(L"'TRUE'"), TRUE);
  ASSERT_EQ(tk_watob(L" TRUE "), TRUE);

  ASSERT_EQ(tk_watob(NULL), FALSE);
  ASSERT_EQ(tk_watob(L""), FALSE);
  ASSERT_EQ(tk_watob(L" "), TRUE);

  ASSERT_EQ(tk_watob(L"0"), FALSE);
  ASSERT_EQ(tk_watob(L"00"), TRUE);
  ASSERT_EQ(tk_watob(L"0AWTK"), TRUE);
  ASSERT_EQ(tk_watob(L"1"), TRUE);
  ASSERT_EQ(tk_watob(L"-1"), TRUE);
  ASSERT_EQ(tk_watob(L"2"), TRUE);

  ASSERT_EQ(tk_watob(L"abcd"), TRUE);
}

TEST(Utils, tk_strs_bsearch) {
  const char* strs[] = {"abc1", "abc2", "abc3", "abc4", "abc5", "b", "c", "d"};
  ASSERT_EQ(tk_strs_bsearch(strs, 0, "Abc1", TRUE), (char*)NULL);
  ASSERT_EQ(tk_strs_bsearch(strs, 0, "Abc1", FALSE), (char*)NULL);

  ASSERT_EQ(tk_strs_bsearch(strs, 1, "Abc1", TRUE), (char*)NULL);
  ASSERT_STREQ(tk_strs_bsearch(strs, 1, "Abc1", FALSE), "abc1");

  for (uint32_t i = 1; i < ARRAY_SIZE(strs); i++) {
    for (uint32_t j = 0; j < ARRAY_SIZE(strs); j++) {
      if (j >= i) {
        ASSERT_EQ(tk_strs_bsearch(strs, i, strs[j], TRUE), (char*)NULL);
      } else {
        ASSERT_STREQ(tk_strs_bsearch(strs, i, strs[j], TRUE), strs[j]);
      }
    }
  }
}

TEST(Utils, tk_str_indexable) {
  ASSERT_EQ(tk_str_indexable("abc"), FALSE);
  ASSERT_EQ(tk_str_indexable(""), FALSE);
  ASSERT_EQ(tk_str_indexable("[]"), FALSE);
  ASSERT_EQ(tk_str_indexable("[1]"), TRUE);
  ASSERT_EQ(tk_str_indexable("[123]"), TRUE);
  ASSERT_EQ(tk_str_indexable("[-123]"), TRUE);
}

TEST(Utils, tk_normalize_rad) {
  double twopi = 2 * M_PI;
  ASSERT_DOUBLE_EQ(tk_normalize_rad(0), 0);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(1), 1);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(2), 2);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(3), 3);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(4), 4);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(5), 5);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(6), 6);

  ASSERT_DOUBLE_EQ(tk_normalize_rad(0 + twopi), 0);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(1 + twopi), 1);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(2 + twopi), 2);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(3 + twopi), 3);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(4 + twopi), 4);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(5 + twopi), 5);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(6 + twopi), 6);

  ASSERT_DOUBLE_EQ(tk_normalize_rad(0 - twopi), 0);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(1 - twopi), 1);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(2 - twopi), 2);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(3 - twopi), 3);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(4 - twopi), 4);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(5 - twopi), 5);
  ASSERT_DOUBLE_EQ(tk_normalize_rad(6 - twopi), 6);
}

TEST(Utils, tk_compare_rad) {
  double twopi = 2 * M_PI;
  ASSERT_EQ(tk_rad_equal(0, 0, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(1, 1, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(2, 2, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(3, 3, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(4, 4, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(5, 5, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(6, 6, 0.0001), TRUE);

  ASSERT_EQ(tk_rad_equal(0, 0 + twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(1, 1 + twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(2, 2 + twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(3, 3 + twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(4, 4 + twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(5, 5 + twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(6, 6 + twopi, 0.0001), TRUE);

  ASSERT_EQ(tk_rad_equal(0, 0 - twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(1, 1 - twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(2, 2 - twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(3, 3 - twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(4, 4 - twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(5, 5 - twopi, 0.0001), TRUE);
  ASSERT_EQ(tk_rad_equal(6, 6 - twopi, 0.0001), TRUE);

  ASSERT_EQ(tk_rad_equal(0, 0 - twopi + 1, 0.0001), FALSE);
  ASSERT_EQ(tk_rad_equal(1, 1 - twopi + 1, 0.0001), FALSE);
  ASSERT_EQ(tk_rad_equal(2, 2 - twopi + 1, 0.0001), FALSE);
  ASSERT_EQ(tk_rad_equal(3, 3 - twopi + 1, 0.0001), FALSE);
  ASSERT_EQ(tk_rad_equal(4, 4 - twopi + 1, 0.0001), FALSE);
  ASSERT_EQ(tk_rad_equal(5, 5 - twopi + 1, 0.0001), FALSE);
  ASSERT_EQ(tk_rad_equal(6, 6 - twopi + 1, 0.0001), FALSE);
}

TEST(Utils, tk_swap) {
  int a = 1;
  int b = 2;
  tk_swap(a, b, int);
  ASSERT_EQ(a, 2);
  ASSERT_EQ(b, 1);

  tk_swap(a, b, int);
  ASSERT_EQ(a, 1);
  ASSERT_EQ(b, 2);
}

TEST(Utils, tk_isxxx) {
  ASSERT_TRUE(tk_isspace(' '));
  ASSERT_TRUE(!tk_isspace(-1));
  ASSERT_TRUE(!tk_isspace(160));

  ASSERT_TRUE(tk_isdigit('0'));
  ASSERT_TRUE(!tk_isdigit(-1));
  ASSERT_TRUE(!tk_isdigit(160));

  ASSERT_TRUE(tk_isxdigit('a'));
  ASSERT_TRUE(!tk_isxdigit(-1));
  ASSERT_TRUE(!tk_isxdigit(160));

  ASSERT_TRUE(tk_isprint('a'));
  ASSERT_TRUE(!tk_isprint(-1));
  ASSERT_TRUE(!tk_isprint(160));

  ASSERT_TRUE(tk_isalpha('a'));
  ASSERT_TRUE(!tk_isalpha(-1));
  ASSERT_TRUE(!tk_isalpha(160));

  ASSERT_TRUE(tk_islower('a'));
  ASSERT_TRUE(!tk_islower(-1));
  ASSERT_TRUE(!tk_islower(160));

  ASSERT_TRUE(tk_isupper('A'));
  ASSERT_TRUE(!tk_isupper(-1));
  ASSERT_TRUE(!tk_isupper(160));

  ASSERT_TRUE(tk_isalnum('a'));
  ASSERT_TRUE(!tk_isalnum(-1));
  ASSERT_TRUE(!tk_isalnum(160));
}

static ret_t return_ret_fail(void) {
  return RET_FAIL;
}

static ret_t test_return_ret_if_fail(void) {
  return_ret_if_fail(return_ret_fail());

  return RET_OK;
}

TEST(Utils, return_ret_if_fail) {
  test_return_ret_if_fail();
}

TEST(Utils, tk_str_trim_left) {
  char str[1024] = {0};

  strncpy(str, "abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, " "), RET_OK);
  ASSERT_STREQ(str, "abc");

  strncpy(str, " abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, " "), RET_OK);
  ASSERT_STREQ(str, "abc");

  strncpy(str, "abc ", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, " "), RET_OK);
  ASSERT_STREQ(str, "abc ");

  strncpy(str, " abc ", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, " "), RET_OK);
  ASSERT_STREQ(str, "abc ");

  strncpy(str, "::;; abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, ":; "), RET_OK);
  ASSERT_STREQ(str, "abc");

  strncpy(str, "::;; abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, ":; "), RET_OK);
  ASSERT_STREQ(str, "abc");

  strncpy(str, "::;; ", sizeof(str));
  ASSERT_EQ(tk_str_trim_left(str, ":; "), RET_OK);
  ASSERT_STREQ(str, "");
}

TEST(Utils, tk_str_trim_right) {
  char str[1024] = {0};

  strncpy(str, "abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, " "), RET_OK);
  ASSERT_STREQ(str, "abc");

  strncpy(str, " abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, " "), RET_OK);
  ASSERT_STREQ(str, " abc");

  strncpy(str, " 123abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, " "), RET_OK);
  ASSERT_STREQ(str, " 123abc");

  strncpy(str, " abc ", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, " "), RET_OK);
  ASSERT_STREQ(str, " abc");

  strncpy(str, "::;; abc", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, ":; "), RET_OK);
  ASSERT_STREQ(str, "::;; abc");

  strncpy(str, "abc ::;; ", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, ":; "), RET_OK);
  ASSERT_STREQ(str, "abc");

  strncpy(str, "::;; ", sizeof(str));
  ASSERT_EQ(tk_str_trim_right(str, ":; "), RET_OK);
  ASSERT_STREQ(str, "");
}