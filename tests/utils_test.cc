#include <string>
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/object_array.h"
#include "gtest/gtest.h"

using std::string;

TEST(Utils, basic) {
  char str[32];

  ASSERT_EQ(tk_atoi("0015"), 15);
  ASSERT_EQ(tk_atoi("100"), 100);
  ASSERT_EQ(tk_atoi("0xff"), 0xff);
  ASSERT_EQ(tk_atoi("0x1"), 0x1);
  ASSERT_EQ(tk_atoi("0xf"), 0xf);
  ASSERT_EQ(tk_atoi("0Xf"), 0xf);
  ASSERT_EQ(tk_atoi("0b11"), 3);
  ASSERT_EQ(tk_atoi("0B101"), 5);
  ASSERT_EQ(tk_watoi(L"100"), 100);
  ASSERT_EQ(tk_atof("100"), 100);
  ASSERT_EQ(tk_atof("1e2"), 100);
  ASSERT_EQ(tk_watof(L"100"), 100);

  ASSERT_EQ(tk_atol("0x1122334455667788"), 0x1122334455667788);
  ASSERT_EQ(tk_atol("1122334455667788"), 1122334455667788);
  ASSERT_EQ(tk_atol("-1122334455667788"), -1122334455667788);
  ASSERT_EQ(strcmp(tk_itoa(str, sizeof(str), tk_atoi("100")), "100"), 0);
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
            "<window><mledit><property name=\"text\"><?include "
            "filename=\"button.xml\"?></property></mledit><label />\n</window>");

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

TEST(Utils, tk_str_copy) {
  char* p = NULL;
  p = tk_str_copy(p, "abc");
  ASSERT_STREQ(p, "abc");

  p = tk_str_copy(p, "abc123");
  ASSERT_STREQ(p, "abc123");

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

TEST(Utils, ieq) {
  ASSERT_EQ(strcasecmp("Trigger", "trigger"), 0);
  ASSERT_EQ(tk_str_ieq("Trigger", "trigger"), true);
  ASSERT_EQ(tk_str_ieq("Trigger", "Trigger"), true);
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
               "{\"addr\":{\"city\":\"sz\",\"country\":\"zh\"},\"age\":100,\"arr\":[1,2,\"abc\","
               "\"hello\"],\"name\":\"jim\"}");

  str_reset(&str);
  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(arr);
  TK_OBJECT_UNREF(addr);
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
