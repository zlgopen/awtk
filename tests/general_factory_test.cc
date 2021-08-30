#include "tkc/general_factory.h"
#include "gtest/gtest.h"

static void* foo_create(void) {
  return NULL;
}

static void* bar_create(void) {
  return NULL;
}

static void* alpha_create(void) {
  return NULL;
}

FACTORY_TABLE_BEGIN(s_demo)
FACTORY_TABLE_ENTRY("foo", foo_create)
FACTORY_TABLE_ENTRY("bar", bar_create)
FACTORY_TABLE_ENTRY("alpha", alpha_create)
FACTORY_TABLE_END()

FACTORY_TABLE_BEGIN(s_bar)
FACTORY_TABLE_ENTRY("aaa", foo_create)
FACTORY_TABLE_ENTRY("bbb", bar_create)
FACTORY_TABLE_ENTRY("ccc", alpha_create)
FACTORY_TABLE_END()

TEST(GeneralFactory, basic) {
  general_factory_t factory;
  general_factory_t* f = general_factory_init(&factory);

  ASSERT_EQ(general_factory_find(f, "foo") == foo_create, false);
  ASSERT_EQ(general_factory_register(f, "foo", foo_create), RET_OK);
  ASSERT_EQ(general_factory_find(f, "foo") == foo_create, true);

  ASSERT_EQ(general_factory_find(f, "bar") == bar_create, false);
  ASSERT_EQ(general_factory_register(f, "bar", bar_create), RET_OK);
  ASSERT_EQ(general_factory_find(f, "bar") == bar_create, true);

  ASSERT_EQ(general_factory_find(f, "alpha") == alpha_create, false);
  ASSERT_EQ(general_factory_register(f, "alpha", alpha_create), RET_OK);
  ASSERT_EQ(general_factory_find(f, "alpha") == alpha_create, true);

  ASSERT_EQ(general_factory_unregister(f, "foo"), RET_OK);
  ASSERT_EQ(general_factory_find(f, "foo") == foo_create, false);

  ASSERT_EQ(general_factory_unregister(f, "bar"), RET_OK);
  ASSERT_EQ(general_factory_find(f, "bar") == bar_create, false);

  ASSERT_EQ(general_factory_unregister(f, "alpha"), RET_OK);
  ASSERT_EQ(general_factory_find(f, "alpha") == alpha_create, false);

  ASSERT_EQ(general_factory_register_ex(f, "alpha", alpha_create, FALSE), RET_OK);
  ASSERT_EQ(general_factory_find(f, "alpha") == alpha_create, true);

  general_factory_deinit(f);
}

TEST(GeneralFactory, table) {
  general_factory_t factory;
  general_factory_t* f = general_factory_init(&factory);
  ASSERT_EQ(general_factory_register_table(f, s_demo), RET_OK);

  ASSERT_EQ(general_factory_find(f, "foo") == foo_create, true);
  ASSERT_EQ(general_factory_find(f, "bar") == bar_create, true);
  ASSERT_EQ(general_factory_find(f, "alpha") == alpha_create, true);

  ASSERT_EQ(general_factory_register_table(f, s_bar), RET_OK);
  ASSERT_EQ(general_factory_find(f, "aaa") == foo_create, true);
  ASSERT_EQ(general_factory_find(f, "bbb") == bar_create, true);
  ASSERT_EQ(general_factory_find(f, "ccc") == alpha_create, true);

  general_factory_deinit(f);
}
