#include "gtest/gtest.h"
#include "base/mem.h"
#include "base/button.h"
#include "base/widget_factory.h"

typedef struct _custom_widget_t {
  widget_t base;
} custom_widget_t;

widget_t* custom_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  custom_widget_t* c = TKMEM_ZALLOC(custom_widget_t);
  return_value_if_fail(c != NULL, NULL);

  widget = WIDGET(c);
  widget->type = WIDGET_USER_START;
  widget_init(widget, parent, WIDGET_USER_START);
  widget_move_resize(widget, x, y, w, h);

  return widget;
}

TEST(WidgetFactory, button) {
  widget_t* button = NULL;
  widget_factory_t* factory = widget_factory_create();

  button = widget_factory_create_widget(factory, "button", NULL, 10, 20, 30, 40);
  ASSERT_EQ(button != NULL, true);
  ASSERT_EQ(button->type, WIDGET_BUTTON);
  ASSERT_EQ(button->x, 10);
  ASSERT_EQ(button->y, 20);
  ASSERT_EQ(button->w, 30);
  ASSERT_EQ(button->h, 40);

  widget_factory_destroy(factory);
}

TEST(WidgetFactory, custom) {
  widget_t* custom = NULL;
  widget_factory_t* factory = widget_factory_create();

  custom = widget_factory_create_widget(factory, "custom", NULL, 10, 20, 30, 40);
  ASSERT_EQ(custom, WIDGET(NULL));

  widget_factory_register(factory, "custom", custom_widget_create);
  custom = widget_factory_create_widget(factory, "custom", NULL, 10, 20, 30, 40);
  ASSERT_EQ(custom != NULL, true);
  ASSERT_EQ(custom->type, WIDGET_USER_START);
  ASSERT_EQ(custom->x, 10);
  ASSERT_EQ(custom->y, 20);
  ASSERT_EQ(custom->w, 30);
  ASSERT_EQ(custom->h, 40);

  widget_factory_destroy(factory);
}
