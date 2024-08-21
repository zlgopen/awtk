#include "tkc/named_value_hash.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

TEST(NamedValueHash, basic) {
  named_value_hash_t named_value_hash;
  named_value_hash_t* nvh = &named_value_hash;

  named_value_hash_init(nvh, NULL, NULL, HASH_BASE_DEFAULT);

  ASSERT_EQ(nvh->hash, 0);
  ASSERT_EQ(nvh->base.name, (char*)NULL);
  ASSERT_EQ(nvh->base.value.type, VALUE_TYPE_INVALID);

  named_value_hash_deinit(nvh);
}

TEST(NamedValueHash, init) {
  value_t v;
  named_value_hash_t named_value_hash;
  named_value_hash_t* nvh = &named_value_hash;

  value_set_int(&v, 123);
  named_value_hash_init(nvh, "name", &v, HASH_BASE_DEFAULT);

  ASSERT_EQ(nvh->hash, named_value_hash_get_hash_from_str("name", HASH_BASE_DEFAULT));
  ASSERT_EQ(string(nvh->base.name), string("name"));
  ASSERT_EQ(value_int(&v), value_int(&(nvh->base.value)));

  named_value_hash_deinit(nvh);

  ASSERT_EQ(nvh->hash, 0);
  ASSERT_EQ(nvh->base.name, (char*)NULL);
  ASSERT_EQ(nvh->base.value.type, VALUE_TYPE_INVALID);
}

TEST(NamedValueHash, create_ex) {
  value_t v;
  named_value_hash_t* nvh =
      named_value_hash_create_ex("name", value_set_str(&v, "jim"), HASH_BASE_DEFAULT);

  ASSERT_EQ(nvh->hash, named_value_hash_get_hash_from_str("name", HASH_BASE_DEFAULT));
  ASSERT_STREQ(nvh->base.name, "name");
  ASSERT_STREQ(value_str(&(nvh->base.value)), "jim");

  named_value_hash_destroy(nvh);
}

TEST(NamedValueHash, create_ex1) {
  value_t v;
  named_value_hash_t* nvh =
      named_value_hash_create_ex("name", value_set_str(&v, "jim"), HASH_BASE_DEFAULT);

  ASSERT_EQ(nvh->hash, named_value_hash_get_hash_from_str("name", HASH_BASE_DEFAULT));
  ASSERT_STREQ(nvh->base.name, "name");
  ASSERT_STREQ(value_str(&(nvh->base.value)), "jim");
  ASSERT_EQ(named_value_hash_set_name(nvh, "tom", HASH_BASE_DEFAULT), RET_OK);
  ASSERT_EQ(named_value_hash_set_name(nvh, "jike", HASH_BASE_DEFAULT), RET_OK);

  named_value_hash_destroy(nvh);
}
