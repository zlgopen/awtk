
#ifndef TEST_WIDGET_VTABLE_CLASS_H
#define TEST_WIDGET_VTABLE_CLASS_H

#include "base/widget.h"
#include "tkc/str.h"

BEGIN_C_DECLS

typedef struct _all_base_class_t {
  widget_t widget;

  bool_t init;
} all_base_class_t;

typedef struct _empty_class_t {
  all_base_class_t base;

  bool_t empty;
} empty_class_t;

typedef struct _base_class_t {
  all_base_class_t base;

  bool_t is_base;
} base_class_t;

typedef struct _sun_class_t {
  base_class_t base;

  bool_t is_sun;
} sun_class_t;

widget_t* all_base_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* empty_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* base_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* sun_class_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

widget_t* all_base_class_cast(widget_t* widget);
widget_t* empty_class_cast(widget_t* widget);
widget_t* base_class_cast(widget_t* widget);
widget_t* sun_class_cast(widget_t* widget);


const char* const* all_base_class_get_clone_properties();
const char* const* base_class_get_clone_properties();

#define ALL_BASE_CLASS(widget) ((all_base_class_t*)(all_base_class_cast(WIDGET(widget))))
#define EMPTY_CLASS(widget) ((empty_class_t*)(empty_class_cast(WIDGET(widget))))
#define BASE_CLASS(widget) ((base_class_t*)(base_class_cast(WIDGET(widget))))
#define SUN_CLASS(widget) ((sun_class_t*)(sun_class_cast(WIDGET(widget))))

#define WIDGET_VTABLE_CLASS_TYPE_ALL_BASE_CLASS "all_base_class"
#define WIDGET_VTABLE_CLASS_TYPE_EMPTY_CLASS "empty_class"
#define WIDGET_VTABLE_CLASS_TYPE_BASE_CLASS "base_class"
#define WIDGET_VTABLE_CLASS_TYPE_SUN_CLASS "sun_class"

#define WIDGET_VTABLE_CLASS_POINTER_CURSOR "pointer_cursor"
#define WIDGET_VTABLE_CLASS_BASE_POINTER_CURSOR "base_pointer_cursor"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(all_base_class);
TK_EXTERN_VTABLE(empty_class);
TK_EXTERN_VTABLE(base_class);
TK_EXTERN_VTABLE(sun_class);

END_C_DECLS

#endif /*TEST_WIDGET_VTABLE_CLASS_H*/
