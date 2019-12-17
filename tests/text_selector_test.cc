#include "gtest/gtest.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "text_selector/text_selector.h"

#include <string>

using std::string;

TEST(TextSelector, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = text_selector_create(NULL, 10, 20, 30, 40);
  text_selector_t* text_selector = TEXT_SELECTOR(w);

  ASSERT_EQ(text_selector_append_option(w, 0, "000"), RET_OK);
  ASSERT_EQ(text_selector_append_option(w, 111, "111"), RET_OK);
  ASSERT_EQ(text_selector_append_option(w, 222, "222"), RET_OK);
  ASSERT_EQ(text_selector_append_option(w, 233, "333"), RET_OK);

  value_set_int(&v1, 1);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_SELECTED_INDEX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_SELECTED_INDEX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), text_selector->selected_index);
  ASSERT_EQ(text_selector_get_value(w), 111);
  ASSERT_EQ(string(text_selector_get_text(w)), string("111"));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(w, TEXT_SELECTOR_PROP_VISIBLE_NR, &v1), RET_OK);
  ASSERT_EQ(value_int(&v1), text_selector->visible_nr);

  /*not eq 3, set it to 5*/
  value_set_int(&v1, 4);
  ASSERT_EQ(widget_set_prop(w, TEXT_SELECTOR_PROP_VISIBLE_NR, &v1), RET_OK);
  ASSERT_EQ(5, text_selector->visible_nr);

  value_set_int(&v1, 5);
  ASSERT_EQ(widget_set_prop(w, TEXT_SELECTOR_PROP_VISIBLE_NR, &v1), RET_OK);
  ASSERT_EQ(value_int(&v1), text_selector->visible_nr);

  widget_destroy(w);
}

TEST(TextSelector, options) {
  widget_t* w = text_selector_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(text_selector_count_options(w), 0);
  ASSERT_EQ(text_selector_append_option(w, 1, "red"), RET_OK);
  ASSERT_EQ(text_selector_count_options(w), 1);

  ASSERT_EQ(text_selector_append_option(w, 2, "green"), RET_OK);
  ASSERT_EQ(text_selector_count_options(w), 2);

  ASSERT_EQ(text_selector_append_option(w, 3, "blue"), RET_OK);
  ASSERT_EQ(text_selector_count_options(w), 3);

  text_selector_set_selected_index(w, 0);
  ASSERT_EQ(text_selector_get_value(w), 1);
  ASSERT_EQ(string(text_selector_get_text(w)), string("red"));

  text_selector_set_selected_index(w, 1);
  ASSERT_EQ(text_selector_get_value(w), 2);
  ASSERT_EQ(string(text_selector_get_text(w)), string("green"));

  text_selector_reset_options(w);
  ASSERT_EQ(text_selector_count_options(w), 0);

  widget_destroy(w);
}

TEST(TextSelector, parse_options) {
  widget_t* w = text_selector_create(NULL, 10, 20, 30, 40);

  text_selector_parse_options(w, "1:red;2:green;3:blue");
  ASSERT_EQ(text_selector_count_options(w), 3);

  text_selector_set_selected_index(w, 0);
  ASSERT_EQ(text_selector_get_value(w), 1);
  ASSERT_EQ(string(text_selector_get_text(w)), string("red"));

  text_selector_set_selected_index(w, 1);
  ASSERT_EQ(text_selector_get_value(w), 2);
  ASSERT_EQ(string(text_selector_get_text(w)), string("green"));

  text_selector_reset_options(w);
  ASSERT_EQ(text_selector_count_options(w), 0);

  widget_destroy(w);
}

TEST(TextSelector, props) {
  value_t v1;
  value_t v2;
  const char* str = "1:red;2:green;3:blue";
  widget_t* w = text_selector_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, str);
  ASSERT_EQ(text_selector_cast(w), w);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_OPTIONS, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_OPTIONS, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v2)), string(str));
  ASSERT_EQ(text_selector_count_options(w), 3);

  widget_destroy(w);
}

#include "log_change_events.inc"

TEST(TextSelector, event) {
  const char* str = "1:red;2:green;3:blue";
  widget_t* w = text_selector_create(NULL, 0, 0, 100, 100);

  text_selector_set_options(w, str);
  text_selector_set_selected_index(w, 0);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  text_selector_set_value(w, 0);
  ASSERT_EQ(s_log, "");

  text_selector_set_selected_index(w, 1);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(TextSelector, cast) {
  widget_t* w = text_selector_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, text_selector_cast(w));

  widget_destroy(w);
}

TEST(TextSelector, range) {
  int32_t i = 0;
  int32_t n = 0;
  char text1[64];
  char text2[64];
  widget_t* w = text_selector_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(text_selector_set_options(w, "1-10"), RET_OK);
  n = text_selector_count_options(w);
  ASSERT_EQ(n, 10);

  for (i = 0; i < n; i++) {
    text_selector_option_t* iter = text_selector_get_option(w, i);
    ASSERT_EQ(iter->value, i + 1);
    tk_utf8_from_utf16(iter->text, text1, sizeof(text1) - 1);
    tk_snprintf(text2, sizeof(text2) - 1, "%d", i + 1);
    ASSERT_STREQ(text1, text2);
  }

  widget_destroy(w);
}

TEST(TextSelector, range_format) {
  int32_t i = 0;
  int32_t n = 0;
  char text1[64];
  char text2[64];
  widget_t* w = text_selector_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(text_selector_set_options(w, "1-10-%4d"), RET_OK);
  n = text_selector_count_options(w);
  ASSERT_EQ(n, 10);

  for (i = 0; i < n; i++) {
    text_selector_option_t* iter = text_selector_get_option(w, i);
    ASSERT_EQ(iter->value, i + 1);
    tk_utf8_from_utf16(iter->text, text1, sizeof(text1) - 1);
    tk_snprintf(text2, sizeof(text2) - 1, "%4d", i + 1);
    ASSERT_STREQ(text1, text2);
  }

  widget_destroy(w);
}

TEST(TextSelector, range_format2) {
  int32_t i = 0;
  int32_t n = 0;
  char text1[64];
  char text2[64];
  widget_t* w = text_selector_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(text_selector_set_options(w, "1-100-item%4X"), RET_OK);
  n = text_selector_count_options(w);
  ASSERT_EQ(n, 100);

  for (i = 0; i < n; i++) {
    text_selector_option_t* iter = text_selector_get_option(w, i);
    ASSERT_EQ(iter->value, i + 1);
    tk_utf8_from_utf16(iter->text, text1, sizeof(text1) - 1);
    tk_snprintf(text2, sizeof(text2) - 1, "item%4X", i + 1);
    ASSERT_STREQ(text1, text2);
  }

  widget_destroy(w);
}
