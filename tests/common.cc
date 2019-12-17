#include "common.h"
#include "tkc/utf8.h"
#include "gtest/gtest.h"

#define PRINTF printf

void bitmap_dump(bitmap_t* b) {
  rgba_t rgba;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t w = b->w;
  uint32_t h = b->h;

  PRINTF("-----------------------------------------------\n");
  for (y = 0; y < h; y++) {
    PRINTF("%02d:", y);
    for (x = 0; x < w; x++) {
      ASSERT_EQ(bitmap_get_pixel(b, x, y, &rgba), RET_OK);
      PRINTF("%02x%02x%02x%02x ", rgba.r, rgba.g, rgba.b, rgba.a);
    }
    PRINTF("\n");
  }
}

void bitmap_check(bitmap_t* b, rect_t* r, rgba_t e) {
  rgba_t rgba;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t w = b->w;
  uint32_t h = b->h;
  uint32_t bpp = bitmap_get_bpp(b);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      ASSERT_EQ(bitmap_get_pixel(b, x, y, &rgba), RET_OK);
      if (rect_contains(r, x, y)) {
        ASSERT_EQ(rgba.r, e.r);
        ASSERT_EQ(rgba.g, e.g);
        ASSERT_EQ(rgba.b, e.b);
        ASSERT_EQ(rgba.a, e.a);
      } else {
        ASSERT_EQ(rgba.r, 0);
        ASSERT_EQ(rgba.g, 0);
        ASSERT_EQ(rgba.b, 0);

        if (bpp == 4) {
          ASSERT_EQ(rgba.a, 0);
        }
      }
    }
  }
}

#include <string>

using std::string;

void assert_str_eq(const wchar_t* wstr, const char* utf8) {
  char str[256];

  tk_utf8_from_utf16(wstr, str, sizeof(str));
  ASSERT_EQ(string(str), string(utf8));
}

ret_t widget_log_events(void* ctx, event_t* e) {
  string& event_log = *(string*)ctx;
  widget_t* widget = WIDGET(e->target);
  assert(widget->ref_count > 0);

  (void)ctx;
  switch (e->type) {
    case EVT_VALUE_CHANGED: {
      event_log += "value_changed";
      break;
    }
    case EVT_VALUE_CHANGING: {
      event_log += "value_changing";
      break;
    }
    case EVT_MOVE: {
      event_log += "move ";
      break;
    }
    case EVT_WILL_MOVE: {
      event_log += "will_move ";
      break;
    }
    case EVT_RESIZE: {
      event_log += "resize ";
      break;
    }
    case EVT_WILL_RESIZE: {
      event_log += "will_resize ";
      break;
    }
    case EVT_MOVE_RESIZE: {
      event_log += "move_resize ";
      break;
    }
    case EVT_WILL_MOVE_RESIZE: {
      event_log += "will_move_resize ";
      break;
    }
    case EVT_PROP_WILL_CHANGE: {
      prop_change_event_t* evt = (prop_change_event_t*)e;

      event_log += "prop_will_change ";
      event_log += evt->name;
      event_log += value_str(evt->value);
      event_log += " ";
      break;
    }
    case EVT_PROP_CHANGED: {
      prop_change_event_t* evt = (prop_change_event_t*)e;

      event_log += "prop_changed ";
      event_log += evt->name;
      event_log += value_str(evt->value);
      event_log += " ";
      break;
    }
    case EVT_POINTER_MOVE: {
      event_log += "EVT_POINTER_MOVE";
      break;
    }
    case EVT_POINTER_DOWN: {
      event_log += "EVT_POINTER_DOWN";
      break;
    }
    case EVT_POINTER_UP: {
      event_log += "EVT_POINTER_UP";
      break;
    }
    case EVT_KEY_DOWN: {
      event_log += "EVT_KEY_DOWN";
      break;
    }
    case EVT_KEY_UP: {
      event_log += "EVT_KEY_UP";
      break;
    }
  }
  return RET_OK;
}
