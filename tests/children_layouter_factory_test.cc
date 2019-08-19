#include <string>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "base/window.h"
#include "layouters/children_layouter_default.h"
#include "base/children_layouter_factory.h"

TEST(ChildrenLayoutFactory, basic) {
  children_layouter_t* l = NULL;
  children_layouter_factory_t* f = children_layouter_factory_create();

  children_layouter_factory_register(f, "test1", children_layouter_default_create);
  children_layouter_factory_register(f, "test2", children_layouter_default_create);
  children_layouter_factory_register(f, "test3", children_layouter_default_create);

  l = children_layouter_factory_create_layouter(f, "test1");
  ASSERT_EQ(l != NULL, TRUE);
  children_layouter_destroy(l);

  l = children_layouter_factory_create_layouter(f, "test2");
  ASSERT_EQ(l != NULL, TRUE);
  children_layouter_destroy(l);

  l = children_layouter_factory_create_layouter(f, "test3");
  ASSERT_EQ(l != NULL, TRUE);
  children_layouter_destroy(l);

  l = children_layouter_factory_create_layouter(f, "notexist");
  ASSERT_EQ(l == NULL, TRUE);

  children_layouter_factory_destroy(f);
}
