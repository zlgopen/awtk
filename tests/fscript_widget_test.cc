#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widgets/progress_bar.h"

TEST(FScriptWidget, basic) {
  value_t v;
  tk_object_t* obj = object_default_create();
  widget_t* w = progress_bar_create(NULL, 0, 0, 100, 20);

  widget_set_value_int(w, 20);
  tk_object_set_prop_pointer(obj, STR_PROP_SELF, w);
  fscript_eval(obj, "widget_add_value('self', 20)", &v);
  ASSERT_EQ(widget_get_value_int(w), 40);
  value_reset(&v);

  fscript_eval(obj, "widget_set('self', 'value', 20)", &v);
  ASSERT_EQ(widget_get_value_int(w), 20);
  value_reset(&v);

  fscript_eval(obj, "widget_get('self', 'value')", &v);
  ASSERT_EQ(value_int(&v), 20);
  value_reset(&v);

  widget_unref(w);
  TK_OBJECT_UNREF(obj);
}
