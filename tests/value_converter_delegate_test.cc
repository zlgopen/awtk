#include "tkc/utils.h"
#include "mvvm/base/value_converter_delegate.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static ret_t to_int(const value_t* from, value_t* to) {
  value_set_int(to, value_int(from));

  return RET_OK;
}

static ret_t to_str(const value_t* from, value_t* to) {
  char str[32];
  tk_snprintf(str, sizeof(str), "%d", value_int(from));
  value_dup_str(to, str);

  return RET_OK;
}

TEST(ValueConverterDelegate, basic) {
  value_t v;
  value_t v_view;
  value_t v_model;
  value_converter_t* c = value_converter_delegate_create(to_int, to_str);

  value_set_int(&v, 1234);

  ASSERT_EQ(value_converter_to_view(c, &v, &v_view), RET_OK);
  ASSERT_EQ(string(value_str(&v_view)), string("1234"));

  ASSERT_EQ(value_converter_to_model(c, &v_view, &v_model), RET_OK);
  ASSERT_EQ(value_int(&v_model), 1234);
  value_reset(&v_view);
  value_reset(&v_model);

  object_unref(OBJECT(c));
}

static void* create_dummy_value_converter(void) {
  return value_converter_delegate_create(to_int, to_str);
}

TEST(ValueConverterDelegate, factory) {
  value_t v;
  value_t v_view;
  value_t v_model;
  value_converter_init();
  ASSERT_EQ(value_converter_register("dummy", create_dummy_value_converter), RET_OK);
  value_converter_t* c = value_converter_create("dummy");

  ASSERT_NE(c, (value_converter_t*)NULL);

  value_set_int(&v, 1234);
  ASSERT_EQ(value_converter_to_view(c, &v, &v_view), RET_OK);
  ASSERT_EQ(string(value_str(&v_view)), string("1234"));

  ASSERT_EQ(value_converter_to_model(c, &v_view, &v_model), RET_OK);
  ASSERT_EQ(value_int(&v_model), 1234);
  value_reset(&v_view);
  value_reset(&v_model);

  object_unref(OBJECT(c));
  value_converter_deinit();
}
