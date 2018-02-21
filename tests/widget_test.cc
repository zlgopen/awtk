
#include "base/widget.h"
#include <stdlib.h>
#include "base/canvas.h"
#include "font_dummy.h"
#include "gtest/gtest.h"
#include "lcd_log.h"

TEST(Widget, moveresize) {
  widget_t w;
  memset(&w, 0x00, sizeof(w));

  ASSERT_EQ(widget_init(&w, NULL, WIDGET_NORMAL_WINDOW), &w);
  ASSERT_EQ(widget_move(&w, 100, 200), RET_OK);
  ASSERT_EQ(w.x, 100);
  ASSERT_EQ(w.y, 200);

  ASSERT_EQ(widget_init(&w, NULL, WIDGET_NORMAL_WINDOW), &w);
  ASSERT_EQ(widget_resize(&w, 300, 400), RET_OK);
  ASSERT_EQ(w.w, 300);
  ASSERT_EQ(w.h, 400);

  ASSERT_EQ(widget_set_state(&w, WIDGET_STATE_PRESSED), RET_OK);
  ASSERT_EQ(w.state, WIDGET_STATE_PRESSED);

  ASSERT_EQ(widget_set_enable(&w, TRUE), RET_OK);
  ASSERT_EQ(w.enable, TRUE);

  ASSERT_EQ(widget_set_enable(&w, FALSE), RET_OK);
  ASSERT_EQ(w.enable, FALSE);

  ASSERT_EQ(widget_set_visible(&w, TRUE), RET_OK);
  ASSERT_EQ(w.visible, TRUE);

  ASSERT_EQ(widget_set_visible(&w, FALSE), RET_OK);
  ASSERT_EQ(w.visible, FALSE);

  ASSERT_EQ(widget_set_selected(&w, TRUE), RET_OK);
  ASSERT_EQ(w.selected, TRUE);

  ASSERT_EQ(widget_set_selected(&w, FALSE), RET_OK);
  ASSERT_EQ(w.selected, FALSE);
}

TEST(Widget, props) {
  widget_t w;
  value_t v1;
  value_t v2;
  memset(&w, 0x00, sizeof(w));

  ASSERT_EQ(widget_init(&w, NULL, WIDGET_NORMAL_WINDOW), &w);
  value_set_str(&v1, "window");
  ASSERT_EQ(widget_set_prop(&w, "name", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(&w, "name", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  value_set_int(&v1, 1);
  ASSERT_EQ(widget_set_prop(&w, "x", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(&w, "x", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(v1), value_int(v2));

  value_set_int(&v1, 2);
  ASSERT_EQ(widget_set_prop(&w, "y", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(&w, "y", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(v1), value_int(v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(&w, "w", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(&w, "w", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(v1), value_int(v2));

  value_set_int(&v1, 4);
  ASSERT_EQ(widget_set_prop(&w, "h", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(&w, "h", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(v1), value_int(v2));

  ASSERT_EQ(widget_set_prop(&w, "hh", &v1), RET_NOT_FOUND);
  ASSERT_EQ(widget_get_prop(&w, "hh", &v2), RET_NOT_FOUND);
}

TEST(Widget, children) {
  widget_t c1;
  widget_t c2;
  widget_t c3;
  widget_t c4;
  widget_t c5;
  widget_t c6;
  widget_t group;
  widget_t w;
  uint32_t size = sizeof(c1);
  printf("widget size=%d\n", size);
  memset(&w, 0x00, size);
  memset(&c1, 0x00, size);
  memset(&c2, 0x00, size);
  memset(&c3, 0x00, size);
  memset(&c4, 0x00, size);
  memset(&c5, 0x00, size);
  memset(&c6, 0x00, size);
  memset(&group, 0x00, size);

  ASSERT_EQ(widget_init(&w, NULL, WIDGET_NORMAL_WINDOW), &w);
  ASSERT_EQ(widget_init(&c1, &w, WIDGET_BUTTON), &c1);
  ASSERT_EQ(widget_init(&c2, &w, WIDGET_BUTTON), &c2);
  ASSERT_EQ(widget_init(&c3, &w, WIDGET_BUTTON), &c3);
  ASSERT_EQ(widget_init(&group, &w, WIDGET_BUTTON), &group);

  ASSERT_EQ(widget_init(&c4, &group, WIDGET_BUTTON), &c4);
  ASSERT_EQ(widget_init(&c5, &group, WIDGET_BUTTON), &c5);
  ASSERT_EQ(widget_init(&c6, &group, WIDGET_BUTTON), &c6);

  ASSERT_EQ(widget_set_name(&c1, "c1"), RET_OK);
  ASSERT_EQ(widget_set_name(&c2, "c2"), RET_OK);
  ASSERT_EQ(widget_set_name(&c3, "c3"), RET_OK);
  ASSERT_EQ(widget_set_name(&c4, "c4"), RET_OK);
  ASSERT_EQ(widget_set_name(&c5, "c5"), RET_OK);
  ASSERT_EQ(widget_set_name(&c6, "c6"), RET_OK);
  ASSERT_EQ(widget_set_name(&group, "group"), RET_OK);

  ASSERT_EQ(widget_lookup_child(&w, "c1"), &c1);
  ASSERT_EQ(widget_lookup_child(&w, "c2"), &c2);
  ASSERT_EQ(widget_lookup_child(&w, "c3"), &c3);

  ASSERT_EQ(widget_lookup(&w, "c1"), &c1);
  ASSERT_EQ(widget_lookup(&w, "c2"), &c2);
  ASSERT_EQ(widget_lookup(&w, "c3"), &c3);

  ASSERT_EQ(widget_lookup(&w, "c4"), &c4);
  ASSERT_EQ(widget_lookup(&w, "c5"), &c5);
  ASSERT_EQ(widget_lookup(&w, "c6"), &c6);

  widget_move_resize(&c1, 10, 10, 20, 20);
  ASSERT_EQ(widget_find_target(&w, 11, 11), &c1);

  widget_move_resize(&c2, 150, 10, 20, 20);
  ASSERT_EQ(widget_find_target(&w, 151, 11), &c2);

  widget_move_resize(&c3, 150, 100, 20, 20);
  ASSERT_EQ(widget_find_target(&w, 151, 100), &c3);

  ASSERT_EQ(widget_remove_child(&w, &c3), RET_OK);
  ASSERT_EQ(widget_remove_child(&w, &c3), RET_NOT_FOUND);
}
