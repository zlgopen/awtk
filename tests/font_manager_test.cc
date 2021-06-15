
#include "base/canvas.h"
#include "base/font_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(FontManager, basic) {
  font_t* tmp;
  font_manager_t font_manager;
  const char* default_font = system_info()->default_font;
  font_dummy_init();
  font_manager_init(&font_manager, NULL);
  font_manager_add_font(&font_manager, font_dummy_0("demo0", 10));
  font_manager_add_font(&font_manager, font_dummy_1("demo1", 11));
  font_manager_add_font(&font_manager, font_dummy_2("demo2", 12));
  font_manager_add_font(&font_manager, font_dummy_default(default_font, 10));

  ASSERT_EQ(font_manager_get_font(&font_manager, "demo0", 10), font_dummy_0("demo0", 10));
  ASSERT_EQ(font_manager_get_font(&font_manager, "demo1", 11), font_dummy_1("demo1", 11));
  ASSERT_EQ(font_manager_get_font(&font_manager, "demo2", 12), font_dummy_2("demo2", 12));

  tmp = font_dummy_default(default_font, 10);
  ASSERT_EQ(font_manager_get_font(&font_manager, NULL, 10), tmp);
  tmp = font_dummy_default(default_font, 11);
  ASSERT_EQ(font_manager_get_font(&font_manager, NULL, 11), tmp);
  tmp = font_dummy_default(default_font, 12);
  ASSERT_EQ(font_manager_get_font(&font_manager, NULL, 12), tmp);

  ASSERT_EQ(font_manager_get_font(&font_manager, NULL, 20) == NULL, true);
  ASSERT_EQ(font_manager_get_font(&font_manager, "demo0", 20) == NULL, true);

  ASSERT_EQ(font_manager_get_font(&font_manager, default_font, 12) != NULL, true);
  ASSERT_EQ(font_manager_get_font(&font_manager, "demo3", 12) != NULL, true);
  ASSERT_EQ(font_manager_get_font(&font_manager, "demo3", 20) == NULL, true);

  font_manager_deinit(&font_manager);
}

#if defined(WITH_FT_FONT) || defined(WITH_STB_FONT)
#include "font_loader/font_loader_truetype.h"

TEST(FontManager, loader) {
  font_manager_t font_manager;

  font_manager_init(&font_manager, font_loader_truetype());
  font_manager_set_assets_manager(&font_manager, assets_manager());
  ASSERT_EQ(font_manager_get_font(&font_manager, "ap", 20) != NULL, true);
  font_manager_deinit(&font_manager);
}

TEST(FontManager, unload) {
  font_manager_t font_manager;
  emitter_t* emitter = EMITTER(assets_manager());
  if (emitter_size(emitter) > 0) {
    emitter_off_by_tag(emitter, 0);
  }

  font_manager_init(&font_manager, font_loader_truetype());
  font_manager_set_assets_manager(&font_manager, assets_manager());
  ASSERT_EQ(font_manager_get_font(&font_manager, "ap", 20) != NULL, true);

  ASSERT_EQ(font_manager_unload_font(&font_manager, "ap", 0), RET_OK);
  ASSERT_EQ(font_manager_unload_font(&font_manager, "ap", 0), RET_NOT_FOUND);

  ASSERT_EQ(font_manager_lookup(&font_manager, "ap", 20) == NULL, true);

  font_manager_deinit(&font_manager);
}
#endif
