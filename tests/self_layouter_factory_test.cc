#include <string>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "base/window.h"
#include "layouters/self_layouter_default.h"
#include "base/self_layouter_factory.h"

TEST(SelfLayoutFactory, basic) {
  self_layouter_t* l = NULL;
  self_layouter_factory_t* f = self_layouter_factory_create();

  self_layouter_factory_register(f, "test1", self_layouter_default_create);
  self_layouter_factory_register(f, "test2", self_layouter_default_create);
  self_layouter_factory_register(f, "test3", self_layouter_default_create);

  l = self_layouter_factory_create_layouter(f, "test1");
  ASSERT_EQ(l != NULL, TRUE);
  self_layouter_destroy(l);

  l = self_layouter_factory_create_layouter(f, "test2");
  ASSERT_EQ(l != NULL, TRUE);
  self_layouter_destroy(l);

  l = self_layouter_factory_create_layouter(f, "test3");
  ASSERT_EQ(l != NULL, TRUE);
  self_layouter_destroy(l);

  l = self_layouter_factory_create_layouter(f, "notexist");
  ASSERT_EQ(l == NULL, TRUE);

  self_layouter_factory_destroy(f);
}
