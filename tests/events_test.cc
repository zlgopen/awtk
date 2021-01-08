#include "tkc/mem.h"
#include "base/events.h"
#include "gtest/gtest.h"

TEST(Events, pointer_event_rotate) {
  system_info_t info;
  info.lcd_w = 320;
  info.lcd_h = 480;
  pointer_event_t e;

  e.x = 10;
  e.y = 20;
  info.lcd_orientation = LCD_ORIENTATION_0;
  ASSERT_EQ(pointer_event_rotate(&e, &info), RET_OK);
  ASSERT_EQ(e.x, 10);
  ASSERT_EQ(e.y, 20);

  e.x = 10;
  e.y = 20;
  info.lcd_orientation = LCD_ORIENTATION_90;
  ASSERT_EQ(pointer_event_rotate(&e, &info), RET_OK);
  ASSERT_EQ(e.y, 309);
  ASSERT_EQ(e.x, 20);
}

TEST(Events, from_name) {
  ASSERT_EQ(event_from_name("click"), EVT_CLICK);
  ASSERT_EQ(event_from_name("window_close"), EVT_WINDOW_CLOSE);
  ASSERT_EQ(event_from_name("window_open"), EVT_WINDOW_OPEN);
  ASSERT_EQ(event_from_name("window_will_open"), EVT_WINDOW_WILL_OPEN);
  ASSERT_EQ(event_from_name("pointer_up"), EVT_POINTER_UP);
  ASSERT_EQ(event_from_name("pointer_down"), EVT_POINTER_DOWN);
  ASSERT_EQ(event_from_name("pointer_move"), EVT_POINTER_MOVE);
  ASSERT_EQ(event_from_name("key_down"), EVT_KEY_DOWN);
  ASSERT_EQ(event_from_name("key_up"), EVT_KEY_UP);
  ASSERT_EQ(event_from_name("key_long_press"), EVT_KEY_LONG_PRESS);
  ASSERT_EQ(event_from_name("global_key_down"), EVT_KEY_DOWN);
  ASSERT_EQ(event_from_name("global_key_up"), EVT_KEY_UP);
  ASSERT_EQ(event_from_name("global_key_long_press"), EVT_KEY_LONG_PRESS);
  ASSERT_EQ(event_from_name("key_down_before_children"), EVT_KEY_DOWN_BEFORE_CHILDREN);
  ASSERT_EQ(event_from_name("key_up_before_children"), EVT_KEY_UP_BEFORE_CHILDREN);
}
