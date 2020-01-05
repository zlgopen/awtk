#include "gtest/gtest.h"
#include "ext_widgets/file_browser/file_browser.h"

TEST(FileBrowser, dir) {
  fb_item_t* item = NULL;
  file_browser_t* fb = file_browser_create(os_fs());
  ASSERT_EQ(fb != NULL, true);
  ASSERT_EQ(file_browser_set_cwd(fb, "./tests/fbtest"), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  ASSERT_EQ(file_browser_create_dir(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 1);

  item = file_browser_get_item(fb, 0);
  ASSERT_EQ(item != NULL, true);
  ASSERT_STREQ(item->info.name, "a");
  ASSERT_EQ(item->info.is_dir, TRUE);
  ASSERT_EQ(file_browser_remove(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  file_browser_destroy(fb);
}

TEST(FileBrowser, file) {
  fb_item_t* item = NULL;
  file_browser_t* fb = file_browser_create(os_fs());
  ASSERT_EQ(fb != NULL, true);
  ASSERT_EQ(file_browser_set_cwd(fb, "./tests/fbtest"), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  ASSERT_EQ(file_browser_create_file(fb, "a", "hello", 5), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 1);

  item = file_browser_get_item(fb, 0);
  ASSERT_EQ(item != NULL, true);
  ASSERT_STREQ(item->info.name, "a");
  ASSERT_EQ(item->info.is_file, TRUE);
  ASSERT_EQ(file_browser_remove(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  file_browser_destroy(fb);
}

TEST(FileBrowser, basic) {
  fb_item_t* item = NULL;
  file_browser_t* fb = file_browser_create(os_fs());

  ASSERT_EQ(file_browser_set_cwd(fb, "./tests/fbtest"), RET_OK);
  ASSERT_EQ(file_browser_create_dir(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "a.txt", "hello", 5), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 2);

  ASSERT_EQ(file_browser_remove(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "a.txt"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  file_browser_destroy(fb);
}

TEST(FileBrowser, enter_up) {
  fb_item_t* item = NULL;
  file_browser_t* fb = file_browser_create(os_fs());

  ASSERT_EQ(file_browser_set_cwd(fb, "./tests/fbtest"), RET_OK);
  ASSERT_EQ(file_browser_create_dir(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "a.txt", "hello", 5), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "a1.txt", "world", 5), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 3);

  ASSERT_EQ(file_browser_enter(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  ASSERT_EQ(file_browser_create_dir(fb, "b"), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "b.txt", "hello", 5), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 2);
  
  ASSERT_EQ(file_browser_remove(fb, "b"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "b.txt"), RET_OK);
  ASSERT_EQ(file_browser_up(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 3);

  ASSERT_EQ(file_browser_remove(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "a.txt"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "a1.txt"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  file_browser_destroy(fb);
}
