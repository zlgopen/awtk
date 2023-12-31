
#include "base/widget.h"
#include "base/widget_vtable.h"
#include <string>
#include "gtest/gtest.h"
#include "widget_vtable_class.h"

using namespace std;

static string s_log;

static ret_t debugger_log(void* ctx, tk_log_level_t level, const char* format, va_list ap) {
  char msg[1024] = {0};
  tk_vsnprintf(msg, sizeof(msg)-1, format, ap);

  s_log += msg;
  return RET_OK;
}

TEST(Widget_Vtable, all_base_class) {
  log_set_hook(debugger_log, NULL);
  log_set_hook(debugger_log, NULL);

  widget_t* ret_widget = NULL;
  bool_t ret_is_point_in = FALSE;
  widget_t* all_base_class = all_base_class_create(NULL, 0, 0, 100, 100);
  ASSERT_EQ(ALL_BASE_CLASS(all_base_class)->init == TRUE, true);

  const char* const* clone_properties = all_base_class_get_clone_properties();

  ASSERT_EQ(clone_properties, widget_vtable_get_clone_properties(all_base_class->vt));
  ASSERT_EQ(clone_properties, widget_vtable_get_persistent_properties(all_base_class->vt));
  ASSERT_STREQ(WIDGET_VTABLE_CLASS_POINTER_CURSOR, widget_vtable_get_pointer_cursor(all_base_class->vt));

  s_log = "";
  widget_vtable_get_prop(all_base_class, NULL, NULL);
  ASSERT_EQ("all_base_class_get_prop", s_log);

  s_log = "";
  widget_vtable_set_prop(all_base_class, NULL, NULL);
  ASSERT_EQ("all_base_class_set_prop", s_log);

  s_log = "";
  widget_vtable_invalidate(all_base_class, NULL);
  ASSERT_EQ("all_base_class_invalidate", s_log);

  s_log = "";
  widget_vtable_find_target(all_base_class, 0, 0, &ret_widget);
  ASSERT_EQ("all_base_class_find_target", s_log);

  s_log = "";
  widget_vtable_is_point_in(all_base_class, 0, 0, &ret_is_point_in);
  ASSERT_EQ("all_base_class_is_point_in", s_log);

  s_log = "";
  widget_vtable_get_offset(all_base_class, 0, 0);
  ASSERT_EQ("all_base_class_get_offset", s_log);

  s_log = "";
  widget_vtable_auto_adjust_size(all_base_class);
  ASSERT_EQ("all_base_class_auto_adjust_size", s_log);

  s_log = "";
  widget_vtable_get_prop_default_value(all_base_class, NULL, NULL);
  ASSERT_EQ("all_base_class_get_prop_default_value", s_log);

  s_log = "";
  widget_vtable_get_only_active_children(all_base_class, NULL);
  ASSERT_EQ("all_base_class_get_only_active_children", s_log);

  s_log = "";
  widget_vtable_on_copy(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_copy", s_log);

  s_log = "";
  widget_vtable_on_keyup(all_base_class,NULL);
  ASSERT_EQ("all_base_class_on_keyup", s_log);

  s_log = "";
  widget_vtable_on_keydown(all_base_class,NULL);
  ASSERT_EQ("all_base_class_on_keydown", s_log);

  s_log = "";
  widget_vtable_on_wheel(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_wheel", s_log);

  s_log = "";
  widget_vtable_on_multi_gesture(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_multi_gesture", s_log);

  s_log = "";
  widget_vtable_on_re_translate(all_base_class);
  ASSERT_EQ("all_base_class_on_re_translate", s_log);

  s_log = "";
  widget_vtable_on_paint_background(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_paint_background", s_log);

  s_log = "";
  widget_vtable_on_paint_self(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_paint_self", s_log);

  s_log = "";
  widget_vtable_on_paint_children(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_paint_children", s_log);

  s_log = "";
  widget_vtable_on_paint_border(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_paint_border", s_log);

  s_log = "";
  widget_vtable_on_paint_begin(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_paint_begin", s_log);

  s_log = "";
  widget_vtable_on_paint_end(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_paint_end", s_log);

  s_log = "";
  widget_vtable_on_pointer_down(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_pointer_down", s_log);

  s_log = "";
  widget_vtable_on_pointer_move(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_pointer_move", s_log);

  s_log = "";
  widget_vtable_on_pointer_up(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_pointer_up", s_log);

  s_log = "";
  widget_vtable_on_context_menu(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_context_menu", s_log);

  s_log = "";
  widget_vtable_on_layout_children(all_base_class);
  ASSERT_EQ("all_base_class_on_layout_children", s_log);

  s_log = "";
  widget_vtable_on_add_child(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_add_child", s_log);

  s_log = "";
  widget_vtable_on_remove_child(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_remove_child", s_log);

  s_log = "";
  widget_vtable_on_attach_parent(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_attach_parent", s_log);

  s_log = "";
  widget_vtable_on_detach_parent(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_detach_parent", s_log);

  s_log = "";
  widget_vtable_on_event(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_event", s_log);

  s_log = "";
  widget_vtable_on_event_before_children(all_base_class, NULL);
  ASSERT_EQ("all_base_class_on_event_before_children", s_log);

  s_log = "";
  widget_vtable_on_destroy(all_base_class);
  ASSERT_EQ("all_base_class_on_destroy", s_log);
  
  log_set_hook(NULL, NULL);
  log_set_hook(NULL, NULL);
}

TEST(Widget_Vtable, empty_class) {
  log_set_hook(debugger_log, NULL);
  log_set_hook(debugger_log, NULL);

  widget_t* ret_widget = NULL;
  bool_t ret_is_point_in = FALSE;
  widget_t* widget = empty_class_create(NULL, 0, 0, 100, 100);
  ASSERT_EQ(ALL_BASE_CLASS(widget)->init == TRUE, true);
  ASSERT_EQ(EMPTY_CLASS(widget)->empty == TRUE, true);

  const char* const* clone_properties = all_base_class_get_clone_properties();

  ASSERT_EQ(clone_properties, widget_vtable_get_clone_properties(widget->vt));
  ASSERT_EQ(clone_properties, widget_vtable_get_persistent_properties(widget->vt));
  ASSERT_STREQ(WIDGET_VTABLE_CLASS_POINTER_CURSOR, widget_vtable_get_pointer_cursor(widget->vt));

  s_log = "";
  widget_vtable_get_prop(widget, NULL, NULL);
  ASSERT_EQ("all_base_class_get_prop", s_log);

  s_log = "";
  widget_vtable_set_prop(widget, NULL, NULL);
  ASSERT_EQ("all_base_class_set_prop", s_log);

  s_log = "";
  widget_vtable_invalidate(widget, NULL);
  ASSERT_EQ("all_base_class_invalidate", s_log);

  s_log = "";
  widget_vtable_find_target(widget, 0, 0, &ret_widget);
  ASSERT_EQ("all_base_class_find_target", s_log);

  s_log = "";
  widget_vtable_is_point_in(widget, 0, 0, &ret_is_point_in);
  ASSERT_EQ("all_base_class_is_point_in", s_log);

  s_log = "";
  widget_vtable_get_offset(widget, 0, 0);
  ASSERT_EQ("all_base_class_get_offset", s_log);

  s_log = "";
  widget_vtable_auto_adjust_size(widget);
  ASSERT_EQ("all_base_class_auto_adjust_size", s_log);

  s_log = "";
  widget_vtable_get_prop_default_value(widget, NULL, NULL);
  ASSERT_EQ("all_base_class_get_prop_default_value", s_log);

  s_log = "";
  widget_vtable_get_only_active_children(widget, NULL);
  ASSERT_EQ("all_base_class_get_only_active_children", s_log);

  s_log = "";
  widget_vtable_on_copy(widget, NULL);
  ASSERT_EQ("all_base_class_on_copy", s_log);

  s_log = "";
  widget_vtable_on_keyup(widget,NULL);
  ASSERT_EQ("all_base_class_on_keyup", s_log);

  s_log = "";
  widget_vtable_on_keydown(widget,NULL);
  ASSERT_EQ("all_base_class_on_keydown", s_log);

  s_log = "";
  widget_vtable_on_wheel(widget, NULL);
  ASSERT_EQ("all_base_class_on_wheel", s_log);

  s_log = "";
  widget_vtable_on_multi_gesture(widget, NULL);
  ASSERT_EQ("all_base_class_on_multi_gesture", s_log);

  s_log = "";
  widget_vtable_on_re_translate(widget);
  ASSERT_EQ("all_base_class_on_re_translate", s_log);

  s_log = "";
  widget_vtable_on_paint_background(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_background", s_log);

  s_log = "";
  widget_vtable_on_paint_self(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_self", s_log);

  s_log = "";
  widget_vtable_on_paint_children(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_children", s_log);

  s_log = "";
  widget_vtable_on_paint_border(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_border", s_log);

  s_log = "";
  widget_vtable_on_paint_begin(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_begin", s_log);

  s_log = "";
  widget_vtable_on_paint_end(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_end", s_log);

  s_log = "";
  widget_vtable_on_pointer_down(widget, NULL);
  ASSERT_EQ("all_base_class_on_pointer_down", s_log);

  s_log = "";
  widget_vtable_on_pointer_move(widget, NULL);
  ASSERT_EQ("all_base_class_on_pointer_move", s_log);

  s_log = "";
  widget_vtable_on_pointer_up(widget, NULL);
  ASSERT_EQ("all_base_class_on_pointer_up", s_log);

  s_log = "";
  widget_vtable_on_context_menu(widget, NULL);
  ASSERT_EQ("all_base_class_on_context_menu", s_log);

  s_log = "";
  widget_vtable_on_layout_children(widget);
  ASSERT_EQ("all_base_class_on_layout_children", s_log);

  s_log = "";
  widget_vtable_on_add_child(widget, NULL);
  ASSERT_EQ("all_base_class_on_add_child", s_log);

  s_log = "";
  widget_vtable_on_remove_child(widget, NULL);
  ASSERT_EQ("all_base_class_on_remove_child", s_log);

  s_log = "";
  widget_vtable_on_attach_parent(widget, NULL);
  ASSERT_EQ("all_base_class_on_attach_parent", s_log);

  s_log = "";
  widget_vtable_on_detach_parent(widget, NULL);
  ASSERT_EQ("all_base_class_on_detach_parent", s_log);

  s_log = "";
  widget_vtable_on_event(widget, NULL);
  ASSERT_EQ("all_base_class_on_event", s_log);

  s_log = "";
  widget_vtable_on_event_before_children(widget, NULL);
  ASSERT_EQ("all_base_class_on_event_before_children", s_log);

  s_log = "";
  widget_vtable_on_destroy(widget);
  ASSERT_EQ("all_base_class_on_destroy", s_log);
  
  log_set_hook(NULL, NULL);
  log_set_hook(NULL, NULL);
}

TEST(Widget_Vtable, base_class) {
  log_set_hook(debugger_log, NULL);
  log_set_hook(debugger_log, NULL);

  widget_t* ret_widget = NULL;
  bool_t ret_is_point_in = FALSE;
  widget_t* widget = base_class_create(NULL, 0, 0, 100, 100);
  ASSERT_EQ(ALL_BASE_CLASS(widget)->init == TRUE, true);
  ASSERT_EQ(BASE_CLASS(widget)->is_base == TRUE, true);

  const char* const* clone_properties = base_class_get_clone_properties();
  const char* const* all_base_class_clone_properties = all_base_class_get_clone_properties();

  ASSERT_NE(all_base_class_clone_properties, widget_vtable_get_clone_properties(widget->vt));
  ASSERT_NE(all_base_class_clone_properties, widget_vtable_get_persistent_properties(widget->vt));

  ASSERT_EQ(clone_properties, widget_vtable_get_clone_properties(widget->vt));
  ASSERT_EQ(clone_properties, widget_vtable_get_persistent_properties(widget->vt));

  ASSERT_STRNE(WIDGET_VTABLE_CLASS_POINTER_CURSOR, widget_vtable_get_pointer_cursor(widget->vt));
  ASSERT_STREQ(WIDGET_VTABLE_CLASS_BASE_POINTER_CURSOR, widget_vtable_get_pointer_cursor(widget->vt));

  s_log = "";
  widget_vtable_get_prop(widget, NULL, NULL);
  ASSERT_EQ("all_base_class_get_prop", s_log);

  s_log = "";
  widget_vtable_set_prop(widget, NULL, NULL);
  ASSERT_EQ("all_base_class_set_prop", s_log);

  s_log = "";
  widget_vtable_invalidate(widget, NULL);
  ASSERT_EQ("all_base_class_invalidate", s_log);

  s_log = "";
  widget_vtable_find_target(widget, 0, 0, &ret_widget);
  ASSERT_EQ("all_base_class_find_target", s_log);

  s_log = "";
  widget_vtable_is_point_in(widget, 0, 0, &ret_is_point_in);
  ASSERT_EQ("all_base_class_is_point_in", s_log);

  s_log = "";
  widget_vtable_get_offset(widget, 0, 0);
  ASSERT_EQ("all_base_class_get_offset", s_log);

  s_log = "";
  widget_vtable_auto_adjust_size(widget);
  ASSERT_EQ("all_base_class_auto_adjust_size", s_log);

  s_log = "";
  widget_vtable_get_prop_default_value(widget, NULL, NULL);
  ASSERT_EQ("all_base_class_get_prop_default_value", s_log);

  s_log = "";
  widget_vtable_get_only_active_children(widget, NULL);
  ASSERT_EQ("all_base_class_get_only_active_children", s_log);

  s_log = "";
  widget_vtable_on_copy(widget, NULL);
  ASSERT_EQ("all_base_class_on_copy", s_log);

  s_log = "";
  widget_vtable_on_keyup(widget,NULL);
  ASSERT_EQ("all_base_class_on_keyup", s_log);

  s_log = "";
  widget_vtable_on_keydown(widget,NULL);
  ASSERT_EQ("base_class_on_keydown_all_base_class_on_keydown", s_log);

  s_log = "";
  widget_vtable_on_wheel(widget, NULL);
  ASSERT_EQ("base_class_on_wheel_all_base_class_on_wheel", s_log);

  s_log = "";
  widget_vtable_on_multi_gesture(widget, NULL);
  ASSERT_EQ("base_class_on_multi_gesture_all_base_class_on_multi_gesture", s_log);

  s_log = "";
  widget_vtable_on_re_translate(widget);
  ASSERT_EQ("base_class_on_re_translate_all_base_class_on_re_translate", s_log);

  s_log = "";
  widget_vtable_on_paint_background(widget, NULL);
  ASSERT_EQ("base_class_on_paint_background_all_base_class_on_paint_background", s_log);

  s_log = "";
  widget_vtable_on_paint_self(widget, NULL);
  ASSERT_EQ("base_class_on_paint_self_all_base_class_on_paint_self", s_log);

  s_log = "";
  widget_vtable_on_paint_children(widget, NULL);
  ASSERT_EQ("all_base_class_on_paint_children", s_log);

  s_log = "";
  widget_vtable_on_paint_border(widget, NULL);
  ASSERT_EQ("base_class_on_paint_border_all_base_class_on_paint_border", s_log);

  s_log = "";
  widget_vtable_on_paint_begin(widget, NULL);
  ASSERT_EQ("base_class_on_paint_begin_all_base_class_on_paint_begin", s_log);

  s_log = "";
  widget_vtable_on_paint_end(widget, NULL);
  ASSERT_EQ("base_class_on_paint_end_all_base_class_on_paint_end", s_log);

  s_log = "";
  widget_vtable_on_pointer_down(widget, NULL);
  ASSERT_EQ("base_class_on_pointer_down_all_base_class_on_pointer_down", s_log);

  s_log = "";
  widget_vtable_on_pointer_move(widget, NULL);
  ASSERT_EQ("base_class_on_pointer_move_all_base_class_on_pointer_move", s_log);

  s_log = "";
  widget_vtable_on_pointer_up(widget, NULL);
  ASSERT_EQ("base_class_on_pointer_up_all_base_class_on_pointer_up", s_log);

  s_log = "";
  widget_vtable_on_context_menu(widget, NULL);
  ASSERT_EQ("base_class_on_context_menu_all_base_class_on_context_menu", s_log);

  s_log = "";
  widget_vtable_on_layout_children(widget);
  ASSERT_EQ("all_base_class_on_layout_children", s_log);

  s_log = "";
  widget_vtable_on_add_child(widget, NULL);
  ASSERT_EQ("all_base_class_on_add_child", s_log);

  s_log = "";
  widget_vtable_on_remove_child(widget, NULL);
  ASSERT_EQ("base_class_on_remove_child_all_base_class_on_remove_child", s_log);

  s_log = "";
  widget_vtable_on_attach_parent(widget, NULL);
  ASSERT_EQ("base_class_on_attach_parent_all_base_class_on_attach_parent", s_log);

  s_log = "";
  widget_vtable_on_detach_parent(widget, NULL);
  ASSERT_EQ("base_class_on_detach_parent_all_base_class_on_detach_parent", s_log);

  s_log = "";
  widget_vtable_on_event(widget, NULL);
  ASSERT_EQ("all_base_class_on_event", s_log);

  s_log = "";
  widget_vtable_on_event_before_children(widget, NULL);
  ASSERT_EQ("base_class_on_event_before_children_all_base_class_on_event_before_children", s_log);

  s_log = "";
  widget_vtable_on_destroy(widget);
  ASSERT_EQ("all_base_class_on_destroy", s_log);
  
  log_set_hook(NULL, NULL);
  log_set_hook(NULL, NULL);
}


TEST(Widget_Vtable, sun_class) {
  log_set_hook(debugger_log, NULL);
  log_set_hook(debugger_log, NULL);

  widget_t* ret_widget = NULL;
  bool_t ret_is_point_in = FALSE;
  widget_t* widget = sun_class_create(NULL, 0, 0, 100, 100);
  ASSERT_EQ(ALL_BASE_CLASS(widget)->init == TRUE, true);
  ASSERT_EQ(BASE_CLASS(widget)->is_base == TRUE, true);
  ASSERT_EQ(SUN_CLASS(widget)->is_sun == TRUE, true);

  const char* const* clone_properties = base_class_get_clone_properties();
  const char* const* all_base_class_clone_properties = all_base_class_get_clone_properties();

  ASSERT_NE(all_base_class_clone_properties, widget_vtable_get_clone_properties(widget->vt));
  ASSERT_NE(all_base_class_clone_properties, widget_vtable_get_persistent_properties(widget->vt));

  ASSERT_EQ(clone_properties, widget_vtable_get_clone_properties(widget->vt));
  ASSERT_EQ(clone_properties, widget_vtable_get_persistent_properties(widget->vt));

  ASSERT_STRNE(WIDGET_VTABLE_CLASS_POINTER_CURSOR, widget_vtable_get_pointer_cursor(widget->vt));
  ASSERT_STREQ(WIDGET_VTABLE_CLASS_BASE_POINTER_CURSOR, widget_vtable_get_pointer_cursor(widget->vt));

  s_log = "";
  widget_vtable_get_prop(widget, NULL, NULL);
  ASSERT_EQ("sun_class_get_prop_all_base_class_get_prop", s_log);

  s_log = "";
  widget_vtable_set_prop(widget, NULL, NULL);
  ASSERT_EQ("sun_class_set_prop_all_base_class_set_prop", s_log);

  s_log = "";
  widget_vtable_invalidate(widget, NULL);
  ASSERT_EQ("sun_class_invalidate_all_base_class_invalidate", s_log);

  s_log = "";
  widget_vtable_find_target(widget, 0, 0, &ret_widget);
  ASSERT_EQ("sun_class_find_target_all_base_class_find_target", s_log);

  s_log = "";
  widget_vtable_is_point_in(widget, 0, 0, &ret_is_point_in);
  ASSERT_EQ("sun_class_is_point_in_all_base_class_is_point_in", s_log);

  s_log = "";
  widget_vtable_get_offset(widget, 0, 0);
  ASSERT_EQ("sun_class_get_offset_all_base_class_get_offset", s_log);

  s_log = "";
  widget_vtable_auto_adjust_size(widget);
  ASSERT_EQ("sun_class_auto_adjust_size_all_base_class_auto_adjust_size", s_log);

  s_log = "";
  widget_vtable_get_prop_default_value(widget, NULL, NULL);
  ASSERT_EQ("sun_class_get_prop_default_value_all_base_class_get_prop_default_value", s_log);

  s_log = "";
  widget_vtable_get_only_active_children(widget, NULL);
  ASSERT_EQ("sun_class_get_only_active_children_all_base_class_get_only_active_children", s_log);

  s_log = "";
  widget_vtable_on_copy(widget, NULL);
  ASSERT_EQ("sun_class_on_copy_all_base_class_on_copy", s_log);

  s_log = "";
  widget_vtable_on_keyup(widget,NULL);
  ASSERT_EQ("sun_class_on_keyup_all_base_class_on_keyup", s_log);

  s_log = "";
  widget_vtable_on_keydown(widget,NULL);
  ASSERT_EQ("base_class_on_keydown_all_base_class_on_keydown", s_log);

  s_log = "";
  widget_vtable_on_wheel(widget, NULL);
  ASSERT_EQ("base_class_on_wheel_all_base_class_on_wheel", s_log);

  s_log = "";
  widget_vtable_on_multi_gesture(widget, NULL);
  ASSERT_EQ("base_class_on_multi_gesture_all_base_class_on_multi_gesture", s_log);

  s_log = "";
  widget_vtable_on_re_translate(widget);
  ASSERT_EQ("base_class_on_re_translate_all_base_class_on_re_translate", s_log);

  s_log = "";
  widget_vtable_on_paint_background(widget, NULL);
  ASSERT_EQ("base_class_on_paint_background_all_base_class_on_paint_background", s_log);

  s_log = "";
  widget_vtable_on_paint_self(widget, NULL);
  ASSERT_EQ("base_class_on_paint_self_all_base_class_on_paint_self", s_log);

  s_log = "";
  widget_vtable_on_paint_children(widget, NULL);
  ASSERT_EQ("sun_class_on_paint_children_all_base_class_on_paint_children", s_log);

  s_log = "";
  widget_vtable_on_paint_border(widget, NULL);
  ASSERT_EQ("base_class_on_paint_border_all_base_class_on_paint_border", s_log);

  s_log = "";
  widget_vtable_on_paint_begin(widget, NULL);
  ASSERT_EQ("base_class_on_paint_begin_all_base_class_on_paint_begin", s_log);

  s_log = "";
  widget_vtable_on_paint_end(widget, NULL);
  ASSERT_EQ("base_class_on_paint_end_all_base_class_on_paint_end", s_log);

  s_log = "";
  widget_vtable_on_pointer_down(widget, NULL);
  ASSERT_EQ("base_class_on_pointer_down_all_base_class_on_pointer_down", s_log);

  s_log = "";
  widget_vtable_on_pointer_move(widget, NULL);
  ASSERT_EQ("base_class_on_pointer_move_all_base_class_on_pointer_move", s_log);

  s_log = "";
  widget_vtable_on_pointer_up(widget, NULL);
  ASSERT_EQ("base_class_on_pointer_up_all_base_class_on_pointer_up", s_log);

  s_log = "";
  widget_vtable_on_context_menu(widget, NULL);
  ASSERT_EQ("base_class_on_context_menu_all_base_class_on_context_menu", s_log);

  s_log = "";
  widget_vtable_on_layout_children(widget);
  ASSERT_EQ("sun_class_on_layout_children_all_base_class_on_layout_children", s_log);

  s_log = "";
  widget_vtable_on_add_child(widget, NULL);
  ASSERT_EQ("sun_class_on_add_child_all_base_class_on_add_child", s_log);

  s_log = "";
  widget_vtable_on_remove_child(widget, NULL);
  ASSERT_EQ("base_class_on_remove_child_all_base_class_on_remove_child", s_log);

  s_log = "";
  widget_vtable_on_attach_parent(widget, NULL);
  ASSERT_EQ("base_class_on_attach_parent_all_base_class_on_attach_parent", s_log);

  s_log = "";
  widget_vtable_on_detach_parent(widget, NULL);
  ASSERT_EQ("base_class_on_detach_parent_all_base_class_on_detach_parent", s_log);

  s_log = "";
  widget_vtable_on_event(widget, NULL);
  ASSERT_EQ("sun_class_on_event_all_base_class_on_event", s_log);

  s_log = "";
  widget_vtable_on_event_before_children(widget, NULL);
  ASSERT_EQ("base_class_on_event_before_children_all_base_class_on_event_before_children", s_log);

  s_log = "";
  widget_vtable_on_destroy(widget);
  ASSERT_EQ("sun_class_on_destroy_all_base_class_on_destroy", s_log);
  
  log_set_hook(NULL, NULL);
  log_set_hook(NULL, NULL);
}

