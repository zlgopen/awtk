#include "mvvm/awtk/binding_context_awtk.h"
#include "widgets/window.h"
#include "widgets/slider.h"
#include "gtest/gtest.h"
#include "test_obj.inc"
#include <string>

using std::string;

TEST(BindingContextAwtk, data_basic) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  widget_set_prop_str(slider, "v-data:value", "{i32}");
  binding_context_bind_simple(vm, win);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);
  ASSERT_EQ(widget_get_value(slider), value_int(&v));

  widget_destroy(win);
}
