#include "tkc/object_default.h"
#include "tkc/object_array.h"
#include "tkc/data_reader_mem.h"
#include "conf_io/conf_utils.h"

#include "gtest/gtest.h"

TEST(ConfUtils, object_to_json) {
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

TEST(ConfUtils, object_from_json) {
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

TEST(ConfUtils, object_load_conf_disable_path) {
  char url[MAX_PATH + 1];
  const char* str =
      "abc.log:\n"
      "  output: file\n"
      "  level: DEBUG\n";
  tk_object_t* obj = object_default_create_ex(FALSE);
  tk_object_t* sub_obj = NULL;

  data_reader_mem_build_url(str, tk_strlen(str), url);

  ASSERT_EQ(object_load_conf(obj, url, "yaml"), RET_OK);

  sub_obj = tk_object_get_prop_object(obj, "abc.log");
  ASSERT_TRUE(sub_obj != NULL);

  ASSERT_STREQ(tk_object_get_prop_str(sub_obj, "output"), "file");
  ASSERT_STREQ(tk_object_get_prop_str(sub_obj, "level"), "DEBUG");

  TK_OBJECT_UNREF(obj);
}
