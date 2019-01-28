#include "tkc/utils.h"
#include "mvvm/base/value_validator_delegate.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static bool_t is_valid_age(const value_t* value, str_t* msg) {
  int32_t age = value_int(value);

  if (age > 0 && age < 150) {
    return TRUE;
  } else {
    str_set(msg, "error");
    return FALSE;
  }
}

TEST(ValueValidatorDelegate, basic) {
  value_t v;
  str_t str;
  value_validator_t* c = value_validator_delegate_create(is_valid_age);

  str_init(&str, 0);
  value_set_int(&v, 1234);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), FALSE);
  ASSERT_EQ(string(str.str), "error");

  value_set_int(&v, 123);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), TRUE);

  str_reset(&str);
  object_unref(OBJECT(c));
}

static void* create_dummy_value_validator(void) {
  return value_validator_delegate_create(is_valid_age);
}

TEST(ValueValidatorDelegate, factory) {
  value_t v;
  str_t str;
  value_validator_init();
  ASSERT_EQ(value_validator_register("dummy", create_dummy_value_validator), RET_OK);
  value_validator_t* c = value_validator_create("dummy");

  ASSERT_NE(c, (value_validator_t*)NULL);

  str_init(&str, 0);
  value_set_int(&v, 1234);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), FALSE);
  ASSERT_EQ(string(str.str), "error");

  value_set_int(&v, 123);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), TRUE);

  object_unref(OBJECT(c));
  value_validator_deinit();
}
