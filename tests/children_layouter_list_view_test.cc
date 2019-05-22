#include <string>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widgets/window.h"
#include "base/children_layouter_factory.h"
#include "scroll_view/children_layouter_list_view.h"

using std::string;

TEST(ChildrenLayoutListView, basic) {
  const char* layout_params = "list_view(x=3,y=4,s=14,i=30,d=40)";
  children_layouter_t* layouter = children_layouter_create(layout_params);
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;

  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 3);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 4);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 14);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "i", 0), 30);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "d", 0), 40);

  ASSERT_EQ(l->x_margin, 3);
  ASSERT_EQ(l->y_margin, 4);
  ASSERT_EQ(l->spacing, 14);
  ASSERT_EQ(l->x_margin, 3);
  ASSERT_EQ(l->item_height, 30);
  ASSERT_EQ(l->default_item_height, 40);

  children_layouter_destroy(layouter);
}
