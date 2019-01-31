#include "mvvm/awtk/binding_context_awtk.h"
#include "widgets/window.h"
#include "widgets/slider.h"
#include "base/idle.h"
#include "gtest/gtest.h"
#include "test_obj.inc"
#include <string>

using std::string;

TEST(BindingContextAwtk, data_two_way) {
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

TEST(BindingContextAwtk, data_once) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=Once}");
  binding_context_bind_simple(vm, win);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
}

TEST(BindingContextAwtk, data_one_way) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=OneWay}");
  binding_context_bind_simple(vm, win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);
  
  value_set_int(&v, 88);
  view_model_set_prop(vm, "i32", &v);
  ASSERT_EQ(widget_get_value(slider), 88);

  widget_destroy(win);
}

TEST(BindingContextAwtk, data_changed) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changed}");
  binding_context_bind_simple(vm, win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  widget_destroy(win);
}

TEST(BindingContextAwtk, data_changing) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changing}");
  binding_context_bind_simple(vm, win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 33);

  widget_destroy(win);
}

TEST(BindingContextAwtk, data_explicit) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  binding_context_bind_simple(vm, win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
}

TEST(BindingContextAwtk, command_update_to_model) {
  value_t v;
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 66);
  view_model_set_prop(vm, "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  widget_set_prop_str(slider, "v-command:on_pointer_down", "{save, Args=2, UpdateModel=True}");
  binding_context_bind_simple(vm, win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);
  
  ASSERT_EQ(view_model_get_prop(vm, "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  e.e = event_init(EVT_POINTER_DOWN, slider);
  e.x = 30;
  e.y = 30;
  widget_dispatch(slider, &(e.e));

  ASSERT_EQ(view_model_get_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);
  
  ASSERT_EQ(view_model_get_prop(vm, "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  widget_destroy(win);
}

TEST(BindingContextAwtk, command_close_window) {
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  view_model_t* vm = test_obj_create_view_model();

  widget_set_prop_str(slider, "v-command:on_pointer_down", "{save, Args=2, CloseWindow=True}");
  binding_context_bind_simple(vm, win);

  e.e = event_init(EVT_POINTER_DOWN, slider);
  e.x = 30;
  e.y = 30;
  widget_dispatch(slider, &(e.e));

  ASSERT_EQ(win->parent, (widget_t*)NULL);
  idle_dispatch();
}
