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
