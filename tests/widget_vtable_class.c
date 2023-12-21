
#include "tkc/str.h"
#include "tkc/log.h"
#include "base/widget.h"
#include "base/widget_vtable.h"
#include "widget_vtable_class.h"

static ret_t all_base_class_set_prop(widget_t* widget, const char* name, const value_t* v) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_set_prop");
  return RET_OK;
}

static ret_t all_base_class_get_prop(widget_t* widget, const char* name, value_t* v) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_get_prop");
  return RET_OK;
}

static ret_t all_base_class_get_only_active_children(widget_t* widget, darray_t* all_focusable) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_get_only_active_children");
  return RET_OK;
}

static ret_t all_base_class_get_offset(widget_t* widget, xy_t* out_x, xy_t* out_y) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_get_offset");
  return RET_OK;
}

static widget_t* all_base_class_find_target(widget_t* widget, xy_t x, xy_t y) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, NULL);
  log_debug("all_base_class_find_target");
  return NULL;
}

static ret_t all_base_class_on_add_child(widget_t* widget, widget_t* child) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_add_child");
  return RET_OK;
}

static ret_t all_base_class_on_paint_children(widget_t* widget, canvas_t* c) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_paint_children");
  return RET_OK;
}

static ret_t all_base_class_on_layout_children(widget_t* widget) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_layout_children");
  return RET_OK;
}

static ret_t all_base_class_on_event(widget_t* widget, event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_event");
  return RET_OK;
}

static ret_t all_base_class_init(widget_t* widget) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  all_base_class->init = TRUE;
  return RET_OK;
}

static ret_t all_base_class_on_destroy(widget_t* widget) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_destroy");
  return RET_OK;
}

static ret_t all_base_class_invalidate(widget_t* widget, const rect_t* r) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_invalidate");
  return RET_OK;
}

static bool_t all_base_class_is_point_in(widget_t* widget, xy_t x, xy_t y) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, FALSE);
  log_debug("all_base_class_is_point_in");
  return TRUE;
}

static ret_t all_base_class_auto_adjust_size(widget_t* widget) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_auto_adjust_size");
  return RET_OK;
}

static ret_t all_base_class_get_prop_default_value(widget_t* widget, const char* name, value_t* v) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_get_prop_default_value");
  return RET_OK;
}

static ret_t all_base_class_on_copy(widget_t* widget, widget_t* other) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_copy");
  return RET_OK;
}

static ret_t all_base_class_on_keyup(widget_t* widget, key_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_keyup");
  return RET_OK;
}

static ret_t all_base_class_on_keydown(widget_t* widget, key_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_keydown");
  return RET_OK;
}

static ret_t all_base_class_on_wheel(widget_t* widget, wheel_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_wheel");
  return RET_OK;
}

static ret_t all_base_class_on_multi_gesture(widget_t* widget, multi_gesture_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_multi_gesture");
  return RET_OK;
}

static ret_t all_base_class_on_re_translate(widget_t* widget) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_re_translate");
  return RET_OK;
}

static ret_t all_base_class_on_paint_background(widget_t* widget, canvas_t* c) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_paint_background");
  return RET_OK;
}

static ret_t all_base_class_on_paint_self(widget_t* widget, canvas_t* c) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_paint_self");
  return RET_OK;
}

static ret_t all_base_class_on_paint_border(widget_t* widget, canvas_t* c) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_paint_border");
  return RET_OK;
}

static ret_t all_base_class_on_paint_begin(widget_t* widget, canvas_t* c) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_paint_begin");
  return RET_OK;
}

static ret_t all_base_class_on_paint_end(widget_t* widget, canvas_t* c) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_paint_end");
  return RET_OK;
}

static ret_t all_base_class_on_pointer_down(widget_t* widget, pointer_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_pointer_down");
  return RET_OK;
}

static ret_t all_base_class_on_pointer_move(widget_t* widget, pointer_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_pointer_move");
  return RET_OK;
}

static ret_t all_base_class_on_pointer_up(widget_t* widget, pointer_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_pointer_up");
  return RET_OK;
}

static ret_t all_base_class_on_context_menu(widget_t* widget, pointer_event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_context_menu");
  return RET_OK;
}

static ret_t all_base_class_on_remove_child(widget_t* widget, widget_t* child) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_remove_child");
  return RET_OK;
}

static ret_t all_base_class_on_attach_parent(widget_t* widget, widget_t* parent) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_attach_parent");
  return RET_OK;
}

static ret_t all_base_class_on_detach_parent(widget_t* widget, widget_t* parent) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_detach_parent");
  return RET_OK;
}

static ret_t all_base_class_on_event_before_children(widget_t* widget, event_t* e) {
  all_base_class_t* all_base_class =  ALL_BASE_CLASS(widget);
  return_value_if_fail(all_base_class != NULL, RET_BAD_PARAMS);
  log_debug("all_base_class_on_event_before_children");
  return RET_OK;
}

static const char* s_all_base_class_clone_properties[] = {
    WIDGET_PROP_VIRTUAL_W,     WIDGET_PROP_VIRTUAL_H,     WIDGET_PROP_XSLIDABLE,
    WIDGET_PROP_YSLIDABLE,     WIDGET_PROP_XOFFSET,       WIDGET_PROP_YOFFSET,  NULL};

const char* const* all_base_class_get_clone_properties() {
  return s_all_base_class_clone_properties;
}

TK_DECL_VTABLE(all_base_class) = {.size = sizeof(all_base_class_t),
                                  .type = WIDGET_VTABLE_CLASS_TYPE_ALL_BASE_CLASS,
                                  .clone_properties = s_all_base_class_clone_properties,
                                  .persistent_properties = s_all_base_class_clone_properties,
                                  .pointer_cursor = WIDGET_VTABLE_CLASS_POINTER_CURSOR,
                                  .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                                  .create = all_base_class_create,
                                  .init = all_base_class_init,
                                  .on_keyup = all_base_class_on_keyup,
                                  .on_keydown = all_base_class_on_keydown,
                                  .on_wheel = all_base_class_on_wheel,
                                  .on_event = all_base_class_on_event,
                                  .on_event_before_children = all_base_class_on_event_before_children,
                                  .on_context_menu = all_base_class_on_context_menu,
                                  .on_pointer_up = all_base_class_on_pointer_up,
                                  .on_pointer_down = all_base_class_on_pointer_down,
                                  .on_pointer_move = all_base_class_on_pointer_move,
                                  .on_detach_parent = all_base_class_on_detach_parent,
                                  .on_attach_parent = all_base_class_on_attach_parent,
                                  .on_paint_end = all_base_class_on_paint_end,
                                  .on_paint_begin = all_base_class_on_paint_begin,
                                  .on_paint_border = all_base_class_on_paint_border,
                                  .on_paint_self = all_base_class_on_paint_self,
                                  .on_paint_background = all_base_class_on_paint_background,
                                  .on_re_translate = all_base_class_on_re_translate,
                                  .on_multi_gesture = all_base_class_on_multi_gesture,
                                  .on_layout_children = all_base_class_on_layout_children,
                                  .on_paint_children = all_base_class_on_paint_children,
                                  .on_add_child = all_base_class_on_add_child,
                                  .on_remove_child = all_base_class_on_remove_child,
                                  .find_target = all_base_class_find_target,
                                  .get_offset = all_base_class_get_offset,
                                  .get_only_active_children = all_base_class_get_only_active_children,
                                  .on_destroy = all_base_class_on_destroy,
                                  .invalidate = all_base_class_invalidate,
                                  .on_copy = all_base_class_on_copy,
                                  .auto_adjust_size = all_base_class_auto_adjust_size,
                                  .get_prop_default_value = all_base_class_get_prop_default_value,
                                  .is_point_in = all_base_class_is_point_in,
                                  .get_prop = all_base_class_get_prop,
                                  .set_prop = all_base_class_set_prop};


widget_t* all_base_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(all_base_class), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  all_base_class_init(widget);
  return widget;
}

widget_t* all_base_class_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, all_base_class), NULL);
  return widget;
}



static ret_t empty_class_init(widget_t* widget) {
  empty_class_t* empty_class = EMPTY_CLASS(widget);
  return_value_if_fail(empty_class != NULL, RET_BAD_PARAMS);
  empty_class->empty = TRUE;
  widget_vtable_init_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(empty_class));
  return RET_OK;
}

TK_DECL_VTABLE(empty_class) = {.size = sizeof(empty_class_t),
                               .type = WIDGET_VTABLE_CLASS_TYPE_EMPTY_CLASS,
                               .get_parent_vt = TK_GET_PARENT_VTABLE(all_base_class),
                               .init = empty_class_init,
                               .create = empty_class_create};

widget_t* empty_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(empty_class), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  empty_class_init(widget);
  return widget;
}

widget_t* empty_class_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, empty_class), NULL);
  return widget;
}





static ret_t base_class_on_keydown(widget_t* widget, key_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_keydown_");
  return widget_vtable_on_keydown_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_wheel(widget_t* widget, wheel_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_wheel_");
  return widget_vtable_on_wheel_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_multi_gesture(widget_t* widget, multi_gesture_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_multi_gesture_");
  return widget_vtable_on_multi_gesture_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_re_translate(widget_t* widget) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_re_translate_");
  return widget_vtable_on_re_translate_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_paint_background(widget_t* widget, canvas_t* c) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_paint_background_");
  return widget_vtable_on_paint_background_by_parent(widget, c, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_paint_self(widget_t* widget, canvas_t* c) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_paint_self_");
  return widget_vtable_on_paint_self_by_parent(widget, c, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_paint_border(widget_t* widget, canvas_t* c) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_paint_border_");
  return widget_vtable_on_paint_border_by_parent(widget, c, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_paint_begin(widget_t* widget, canvas_t* c) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_paint_begin_");
  return widget_vtable_on_paint_begin_by_parent(widget, c, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_paint_end(widget_t* widget, canvas_t* c) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_paint_end_");
  return widget_vtable_on_paint_end_by_parent(widget, c, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_pointer_down(widget_t* widget, pointer_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_pointer_down_");
  return widget_vtable_on_pointer_down_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_pointer_move(widget_t* widget, pointer_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_pointer_move_");
  return widget_vtable_on_pointer_move_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_pointer_up(widget_t* widget, pointer_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_pointer_up_");
  return widget_vtable_on_pointer_up_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_context_menu(widget_t* widget, pointer_event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_context_menu_");
  return widget_vtable_on_context_menu_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_remove_child(widget_t* widget, widget_t* child) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_remove_child_");
  return widget_vtable_on_remove_child_by_parent(widget, child, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_attach_parent(widget_t* widget, widget_t* parent) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_attach_parent_");
  return widget_vtable_on_attach_parent_by_parent(widget, parent, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_detach_parent(widget_t* widget, widget_t* parent) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_detach_parent_");
  return widget_vtable_on_detach_parent_by_parent(widget, parent, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_on_event_before_children(widget_t* widget, event_t* e) {
  base_class_t* base_class =  BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  log_debug("base_class_on_event_before_children_");
  return widget_vtable_on_event_before_children_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(base_class));
}

static ret_t base_class_init(widget_t* widget) {
  base_class_t* base_class = BASE_CLASS(widget);
  return_value_if_fail(base_class != NULL, RET_BAD_PARAMS);
  base_class->is_base = TRUE;
  widget_vtable_init_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(base_class));
  return RET_OK;
}

static const char* s_base_class_clone_properties[] = {
    WIDGET_PROP_NAME,     WIDGET_PROP_CLOSABLE,     WIDGET_PROP_RADIO};

const char* const* base_class_get_clone_properties() {
  return s_base_class_clone_properties;
}

TK_DECL_VTABLE(base_class) = {.size = sizeof(base_class_t),
                              .type = WIDGET_VTABLE_CLASS_TYPE_BASE_CLASS,
                              .clone_properties = s_base_class_clone_properties,
                              .persistent_properties = s_base_class_clone_properties,
                              .pointer_cursor = WIDGET_VTABLE_CLASS_BASE_POINTER_CURSOR,
                              .get_parent_vt = TK_GET_PARENT_VTABLE(all_base_class),
                              .create = base_class_create,
                              .init = base_class_init,
                              .on_keydown = base_class_on_keydown,
                              .on_wheel = base_class_on_wheel,
                              .on_event_before_children = base_class_on_event_before_children,
                              .on_context_menu = base_class_on_context_menu,
                              .on_pointer_up = base_class_on_pointer_up,
                              .on_pointer_down = base_class_on_pointer_down,
                              .on_pointer_move = base_class_on_pointer_move,
                              .on_detach_parent = base_class_on_detach_parent,
                              .on_attach_parent = base_class_on_attach_parent,
                              .on_paint_end = base_class_on_paint_end,
                              .on_paint_begin = base_class_on_paint_begin,
                              .on_paint_border = base_class_on_paint_border,
                              .on_paint_self = base_class_on_paint_self,
                              .on_paint_background = base_class_on_paint_background,
                              .on_re_translate = base_class_on_re_translate,
                              .on_multi_gesture = base_class_on_multi_gesture,
                              .on_remove_child = base_class_on_remove_child,};

widget_t* base_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(base_class), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  base_class_init(widget);
  return widget;
}

widget_t* base_class_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, base_class), NULL);
  return widget;
}



static ret_t sun_class_set_prop(widget_t* widget, const char* name, const value_t* v) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_set_prop_");
  return widget_vtable_set_prop_by_parent(widget, name, v, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_get_prop(widget_t* widget, const char* name, value_t* v) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_get_prop_");
  return widget_vtable_get_prop_by_parent(widget, name, v, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_get_only_active_children(widget_t* widget, darray_t* all_focusable) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_get_only_active_children_");
  return widget_vtable_get_only_active_children_by_parent(widget, all_focusable, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_get_offset(widget_t* widget, xy_t* out_x, xy_t* out_y) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_get_offset_");
  return widget_vtable_get_offset_by_parent(widget, out_x, out_y, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static widget_t* sun_class_find_target(widget_t* widget, xy_t x, xy_t y) {
  ret_t ret = RET_OK;
  widget_t *ret_widget = NULL;
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, NULL);
  log_debug("sun_class_find_target_");
  ret = widget_vtable_find_target_by_parent(widget, x, y, &ret_widget, WIDGET_VTABLE_GET_VTABLE(sun_class));
  return ret == RET_NOT_IMPL ? NULL : ret_widget;
}

static ret_t sun_class_on_add_child(widget_t* widget, widget_t* child) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_add_child_");
  return widget_vtable_on_add_child_by_parent(widget, child, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_on_paint_children(widget_t* widget, canvas_t* c) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_paint_children_");
  return widget_vtable_on_paint_children_by_parent(widget, c, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_on_layout_children(widget_t* widget) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_layout_children_");
  return widget_vtable_on_layout_children_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_on_event(widget_t* widget, event_t* e) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_event_");
  return widget_vtable_on_event_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_on_destroy(widget_t* widget) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_destroy_");
  return widget_vtable_on_destroy_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_invalidate(widget_t* widget, const rect_t* r) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_invalidate_");
  return widget_vtable_invalidate_by_parent(widget, r, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static bool_t sun_class_is_point_in(widget_t* widget, xy_t x, xy_t y) {
  ret_t ret = RET_OK;
  bool_t ret_is_point_in = FALSE;
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, FALSE);
  log_debug("sun_class_is_point_in_");
  ret = widget_vtable_is_point_in_by_parent(widget, x, y, &ret_is_point_in, WIDGET_VTABLE_GET_VTABLE(sun_class));
  return ret == RET_NOT_IMPL ? FALSE : ret_is_point_in;
}

static ret_t sun_class_auto_adjust_size(widget_t* widget) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_auto_adjust_size_");
  return widget_vtable_auto_adjust_size_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_get_prop_default_value(widget_t* widget, const char* name, value_t* v) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_get_prop_default_value_");
  return widget_vtable_get_prop_default_value_by_parent(widget, name, v, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_on_copy(widget_t* widget, widget_t* other) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_copy_");
  return widget_vtable_on_copy_by_parent(widget, other, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_on_keyup(widget_t* widget, key_event_t* e) {
  sun_class_t* sun_class =  SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  log_debug("sun_class_on_keyup_");
  return widget_vtable_on_keyup_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(sun_class));
}

static ret_t sun_class_init(widget_t* widget) {
  sun_class_t* sun_class = SUN_CLASS(widget);
  return_value_if_fail(sun_class != NULL, RET_BAD_PARAMS);
  sun_class->is_sun = TRUE;
  widget_vtable_init_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(sun_class));
  return RET_OK;
}

TK_DECL_VTABLE(sun_class) = {.size = sizeof(sun_class_t),
                             .type = WIDGET_VTABLE_CLASS_TYPE_SUN_CLASS,
                             .get_parent_vt = TK_GET_PARENT_VTABLE(base_class),
                             .create = sun_class_create,
                             .create = sun_class_create,
                             .on_keyup = sun_class_on_keyup,
                             .on_event = sun_class_on_event,
                             .on_layout_children = sun_class_on_layout_children,
                             .on_paint_children = sun_class_on_paint_children,
                             .on_add_child = sun_class_on_add_child,
                             .find_target = sun_class_find_target,
                             .get_offset = sun_class_get_offset,
                             .get_only_active_children = sun_class_get_only_active_children,
                             .on_destroy = sun_class_on_destroy,
                             .invalidate = sun_class_invalidate,
                             .on_copy = sun_class_on_copy,
                             .auto_adjust_size = sun_class_auto_adjust_size,
                             .get_prop_default_value = sun_class_get_prop_default_value,
                             .is_point_in = sun_class_is_point_in,
                             .get_prop = sun_class_get_prop,
                             .set_prop = sun_class_set_prop};

widget_t* sun_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(sun_class), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  sun_class_init(widget);
  return widget;
}

widget_t* sun_class_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, sun_class), NULL);
  return widget;
}
