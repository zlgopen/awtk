#include "gtest/gtest.h"
#include "ext_widgets/combo_box_ex/combo_box_ex.h"

#include <string>

using std::string;

TEST(ComboBoxEx, basic) {
  widget_t* w = combo_box_ex_create(NULL, 10, 20, 30, 40);
  ASSERT_STREQ(widget_get_prop_str(w, WIDGET_PROP_TYPE, NULL), WIDGET_TYPE_COMBO_BOX_EX);

  widget_destroy(w);
}

TEST(ComboBoxEx, localize) {
  value_t v1;
  value_t v2;
  widget_t* w = combo_box_ex_create(NULL, 10, 20, 30, 40);
  combo_box_t* combo_box = COMBO_BOX(w);

  ASSERT_EQ(TRUE, combo_box->localize_options);

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_LOCALIZE_OPTIONS, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_LOCALIZE_OPTIONS, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), combo_box->localize_options);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_LOCALIZE_OPTIONS, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_LOCALIZE_OPTIONS, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), combo_box->localize_options);

  widget_destroy(w);
}
