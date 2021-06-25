#include "tkc/str.h"
#include "gtest/gtest.h"
#include "lcd_log.h"
#include "layer_window/layer_window.h"

TEST(LayerWindow, basic) {
  widget_t* w = layer_window_create(NULL, 0, 0, 100, 100);
  ASSERT_EQ(widget_set_prop_str(w, LAYER_WINDOW_PROP_LAYER_NAME, "fast"), RET_OK);
  ASSERT_STREQ(widget_get_prop_str(w, LAYER_WINDOW_PROP_LAYER_NAME, NULL), "fast"); 
  widget_destroy(w);
}
