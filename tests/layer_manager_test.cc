#include "tkc/str.h"
#include "gtest/gtest.h"
#include "lcd_log.h"
#include "layer_window/layer_manager.h"

TEST(LayerManager, basic) {
  canvas_t c;
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, font_manager());
  layer_t* layer = layer_create("foo", &c, 10, 20, 60);
  layer_manager_t* layer_manager = layer_manager_create();

  ASSERT_EQ(layer_manager_add(layer_manager, layer), RET_OK);
  ASSERT_EQ(layer_manager->nr, 1);

  layer_manager_destroy(layer_manager);
}
