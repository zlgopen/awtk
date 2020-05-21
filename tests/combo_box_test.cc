#include "gtest/gtest.h"
#include "widgets/combo_box.h"
#include "ui_loader/ui_serializer.h"

#include <string>

using std::string;

TEST(ComboBox, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);
  combo_box_t* combo_box = COMBO_BOX(w);

  value_set_str(&v1, "window");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_OPEN_WINDOW, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_OPEN_WINDOW, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);
  ASSERT_EQ(strcmp(v1.value.str, combo_box->open_window), 0);

  value_set_int(&v1, 1);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_SELECTED_INDEX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_SELECTED_INDEX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), combo_box->selected_index);

  widget_destroy(w);
}

TEST(ComboBox, item_height) {
  value_t v1;
  value_t v2;
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);
  combo_box_t* combo_box = COMBO_BOX(w);

  value_set_int(&v1, 32);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ITEM_HEIGHT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ITEM_HEIGHT, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), combo_box->item_height);

  widget_destroy(w);
}

TEST(ComboBox, localize) {
  value_t v1;
  value_t v2;
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);
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

TEST(ComboBox, options) {
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(combo_box_count_options(w), 0);
  ASSERT_EQ(combo_box_append_option(w, 1, "red"), RET_OK);
  ASSERT_EQ(combo_box_count_options(w), 1);

  ASSERT_EQ(combo_box_append_option(w, 2, "green"), RET_OK);
  ASSERT_EQ(combo_box_count_options(w), 2);

  ASSERT_EQ(combo_box_append_option(w, 3, "blue"), RET_OK);
  ASSERT_EQ(combo_box_count_options(w), 3);

  combo_box_set_selected_index(w, 0);
  ASSERT_EQ(combo_box_get_value(w), 1);
  ASSERT_EQ(string(combo_box_get_text(w)), string("red"));

  combo_box_set_selected_index(w, 1);
  ASSERT_EQ(combo_box_get_value(w), 2);
  ASSERT_EQ(string(combo_box_get_text(w)), string("green"));

  combo_box_reset_options(w);
  ASSERT_EQ(combo_box_count_options(w), 0);

  widget_destroy(w);
}

TEST(ComboBox, parse_options) {
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);

  combo_box_parse_options(w, "1:red;2:green;3:blue");
  ASSERT_EQ(combo_box_count_options(w), 3);

  combo_box_set_selected_index(w, 0);
  ASSERT_EQ(combo_box_get_value(w), 1);
  ASSERT_EQ(string(combo_box_get_text(w)), string("red"));

  combo_box_set_selected_index(w, 1);
  ASSERT_EQ(combo_box_get_value(w), 2);
  ASSERT_EQ(string(combo_box_get_text(w)), string("green"));

  combo_box_reset_options(w);
  ASSERT_EQ(combo_box_count_options(w), 0);

  widget_destroy(w);
}

TEST(ComboBox, props) {
  value_t v1;
  value_t v2;
  const char* str = "1:red;2:green;3:blue";
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, str);
  ASSERT_EQ(combo_box_cast(w), w);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_OPTIONS, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_OPTIONS, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v2)), string(str));
  ASSERT_EQ(combo_box_count_options(w), 3);

  widget_destroy(w);
}

TEST(ComboBox, value) {
  value_t v1;
  value_t v2;
  const char* str = "1:red;2:green;3:blue";
  widget_t* w = combo_box_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, str);
  ASSERT_EQ(combo_box_cast(w), w);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_OPTIONS, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_OPTIONS, &v2), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_VALUE, "1"), RET_OK);
  ASSERT_EQ(widget_get_value(w), 1);

  widget_destroy(w);
}

#include "log_change_events.inc"

TEST(ComboBox, event) {
  const char* str = "1:red;2:green;3:blue";
  widget_t* w = combo_box_create(NULL, 0, 0, 100, 100);
  combo_box_set_options(w, str);
  combo_box_set_selected_index(w, 0);

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  s_log = "";
  combo_box_set_selected_index(w, 0);
  ASSERT_EQ(s_log, "");

  s_log = "";
  combo_box_set_selected_index(w, 1);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(ComboBox, cast) {
  widget_t* w = combo_box_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, edit_cast(w));
  ASSERT_EQ(w, combo_box_cast(w));

  widget_destroy(w);
}

TEST(ComboBox, resize) {
  widget_t* w = combo_box_create(NULL, 0, 0, 100, 100);
  edit_t* edit = EDIT(w);

  widget_resize(w, 200, 30);
  ASSERT_EQ(edit->right_margin, 30);

  widget_destroy(w);
}

TEST(ComboBox, move_resize) {
  widget_t* w = combo_box_create(NULL, 0, 0, 100, 100);
  edit_t* edit = EDIT(w);

  widget_move_resize(w, 10, 10, 200, 50);
  ASSERT_EQ(edit->right_margin, 50);

  widget_destroy(w);
}

TEST(ComboBox, clone) {
  str_t str;
  widget_t* w2 = NULL;
  widget_t* w1 = combo_box_create(NULL, 0, 0, 100, 100);
  const char* options = "1:red;2:green;3:blue";

  str_init(&str, 0);
  ASSERT_EQ(combo_box_set_options(w1, options), RET_OK);
  ASSERT_EQ(combo_box_set_selected_index(w1, 0), RET_OK);

  ASSERT_STREQ(COMBO_BOX(w1)->options, options);

  w2 = widget_clone(w1, NULL);
  log_debug("==================================\n");
  widget_to_xml(w1, &str);
  log_debug("w1:%s\n", str.str);

  str_set(&str, "");
  widget_to_xml(w2, &str);
  log_debug("w2:%s\n", str.str);
  log_debug("==================================\n");
  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
  str_reset(&str);
}
