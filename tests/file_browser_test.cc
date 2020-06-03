#include "gtest/gtest.h"
#include "ext_widgets/file_browser/file_browser.h"

TEST(FileBrowser, prepare) {
  file_browser_t* fb = file_browser_create(os_fs());
  ASSERT_EQ(file_browser_set_cwd(fb, "./tests"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  file_browser_remove(fb, "fbtest");
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);

  file_browser_create_dir(fb, "fbtest");
  file_browser_destroy(fb);
}

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
  ASSERT_STREQ(item->name, "a");
  ASSERT_EQ(item->is_dir, TRUE);
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
  ASSERT_STREQ(item->name, "a");
  ASSERT_EQ(item->is_reg_file, TRUE);
  ASSERT_EQ(file_browser_remove(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  file_browser_destroy(fb);
}

TEST(FileBrowser, basic) {
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

TEST(FileBrowser, filter_funcs) {
  fs_item_t item;
  memset(&item, 0x00, sizeof(item));

  strcpy(item.name, "test.txt");
  item.is_reg_file = TRUE;
  ASSERT_EQ(fb_filter_files_only(NULL, &item), TRUE);

  item.is_reg_file = FALSE;
  ASSERT_EQ(fb_filter_files_only(NULL, &item), FALSE);

  item.is_dir = TRUE;
  ASSERT_EQ(fb_filter_directories_only(NULL, &item), TRUE);

  item.is_dir = FALSE;
  ASSERT_EQ(fb_filter_directories_only(NULL, &item), FALSE);

  ASSERT_EQ(fb_filter_by_ext_names((void*)".txt", &item), TRUE);
  ASSERT_EQ(fb_filter_by_ext_names((void*)".png", &item), FALSE);
}

TEST(FileBrowser, filter) {
  fb_item_t* item = NULL;
  file_browser_t* fb = file_browser_create(os_fs());

  ASSERT_EQ(file_browser_set_cwd(fb, "./tests/fbtest"), RET_OK);
  ASSERT_EQ(file_browser_create_dir(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "a.txt", "hello", 5), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 2);

  ASSERT_EQ(file_browser_set_filter(fb, fb_filter_files_only, NULL), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 1);
  item = file_browser_get_item(fb, 0);
  ASSERT_EQ(item->is_reg_file, TRUE);

  ASSERT_EQ(file_browser_set_filter(fb, fb_filter_directories_only, NULL), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 1);
  item = file_browser_get_item(fb, 0);
  ASSERT_EQ(item->is_dir, TRUE);

  ASSERT_EQ(file_browser_set_filter(fb, NULL, NULL), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "a"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "a.txt"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);

  file_browser_destroy(fb);
}

static void file_browser_prepare(file_browser_t* fb) {
  ASSERT_EQ(file_browser_set_cwd(fb, "./tests/fbtest"), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "a.txt", "aaa", 3), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "c.txt", "ccccc", 5), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "d.txt", "dddddd", 6), RET_OK);
  ASSERT_EQ(file_browser_create_file(fb, "b.txt", "bbbb", 4), RET_OK);
  ASSERT_EQ(file_browser_create_dir(fb, "dir2"), RET_OK);
  ASSERT_EQ(file_browser_create_dir(fb, "dir1"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 6);
}

static void file_browser_cleanup(file_browser_t* fb) {
  ASSERT_EQ(file_browser_remove(fb, "a.txt"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "b.txt"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "c.txt"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "d.txt"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "dir1"), RET_OK);
  ASSERT_EQ(file_browser_remove(fb, "dir2"), RET_OK);
  ASSERT_EQ(file_browser_refresh(fb), RET_OK);
  ASSERT_EQ(file_browser_get_items_nr(fb), 0);
  file_browser_destroy(fb);
}

TEST(FileBrowser, sort_by_name) {
  fb_item_t* iter = NULL;
  file_browser_t* fb = file_browser_create(os_fs());
  file_browser_prepare(fb);

  ASSERT_EQ(file_browser_sort_by_name(fb, TRUE), RET_OK);
  iter = file_browser_get_item(fb, 0);
  ASSERT_STREQ(iter->name, "dir1");
  iter = file_browser_get_item(fb, 1);
  ASSERT_STREQ(iter->name, "dir2");
  iter = file_browser_get_item(fb, 2);
  ASSERT_STREQ(iter->name, "a.txt");
  iter = file_browser_get_item(fb, 3);
  ASSERT_STREQ(iter->name, "b.txt");
  iter = file_browser_get_item(fb, 4);
  ASSERT_STREQ(iter->name, "c.txt");
  iter = file_browser_get_item(fb, 5);
  ASSERT_STREQ(iter->name, "d.txt");

  ASSERT_EQ(file_browser_sort_by_name(fb, FALSE), RET_OK);
  iter = file_browser_get_item(fb, 0);
  ASSERT_STREQ(iter->name, "dir2");
  iter = file_browser_get_item(fb, 1);
  ASSERT_STREQ(iter->name, "dir1");
  iter = file_browser_get_item(fb, 2);
  ASSERT_STREQ(iter->name, "d.txt");
  iter = file_browser_get_item(fb, 3);
  ASSERT_STREQ(iter->name, "c.txt");
  iter = file_browser_get_item(fb, 4);
  ASSERT_STREQ(iter->name, "b.txt");
  iter = file_browser_get_item(fb, 5);
  ASSERT_STREQ(iter->name, "a.txt");

  file_browser_cleanup(fb);
}

TEST(FileBrowser, compare_by_size) {
  fb_item_t a;
  fb_item_t b;
  memset(&a, 0x00, sizeof(a));
  memset(&b, 0x00, sizeof(b));

  a.size = 100;
  a.name = "a.txt";
  a.is_reg_file = TRUE;

  b.size = 200;
  b.name = "b.txt";
  b.is_reg_file = TRUE;

  ASSERT_EQ(fb_compare_by_size(&a, &b) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_size(&b, &a) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_size(&a, &a) == 0, TRUE);

  ASSERT_EQ(fb_compare_by_size_dec(&a, &b) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_size_dec(&b, &a) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_size_dec(&a, &a) == 0, TRUE);
}

TEST(FileBrowser, compare_by_mtime) {
  fb_item_t a;
  fb_item_t b;
  memset(&a, 0x00, sizeof(a));
  memset(&b, 0x00, sizeof(b));

  a.mtime = 100;
  a.name = "a.txt";
  a.is_reg_file = TRUE;

  b.mtime = 200;
  b.name = "b.txt";
  b.is_reg_file = TRUE;

  ASSERT_EQ(fb_compare_by_mtime(&a, &b) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_mtime(&b, &a) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_mtime(&a, &a) == 0, TRUE);

  ASSERT_EQ(fb_compare_by_mtime_dec(&a, &b) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_mtime_dec(&b, &a) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_mtime_dec(&a, &a) == 0, TRUE);
}

TEST(FileBrowser, compare_by_name) {
  fb_item_t a;
  fb_item_t b;
  memset(&a, 0x00, sizeof(a));
  memset(&b, 0x00, sizeof(b));

  a.name = "a.jpg";
  a.is_reg_file = TRUE;

  b.name = "b.txt";
  b.is_reg_file = TRUE;

  ASSERT_EQ(fb_compare_by_name(&a, &b) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_name(&b, &a) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_name(&a, &a) == 0, TRUE);
  ASSERT_EQ(fb_compare_by_name_dec(&a, &b) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_name_dec(&b, &a) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_name_dec(&a, &a) == 0, TRUE);
}

TEST(FileBrowser, compare_by_type) {
  fb_item_t a;
  fb_item_t b;
  memset(&a, 0x00, sizeof(a));
  memset(&b, 0x00, sizeof(b));

  a.name = "a.jpg";
  a.is_reg_file = TRUE;

  b.name = "b.txt";
  b.is_reg_file = TRUE;

  ASSERT_EQ(fb_compare_by_type(&a, &b) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_type(&b, &a) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_type(&a, &a) == 0, TRUE);
  ASSERT_EQ(fb_compare_by_type_dec(&a, &b) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_type_dec(&b, &a) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_type_dec(&a, &a) == 0, TRUE);
}

TEST(FileBrowser, compare_dir_file) {
  fb_item_t a;
  fb_item_t b;
  memset(&a, 0x00, sizeof(a));
  memset(&b, 0x00, sizeof(b));

  a.name = "b";
  a.is_dir = TRUE;

  b.name = "a";
  b.is_reg_file = TRUE;

  ASSERT_EQ(fb_compare_by_type(&a, &b) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_type(&b, &a) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_type(&a, &a) == 0, TRUE);

  ASSERT_EQ(fb_compare_by_type_dec(&a, &b) < 0, TRUE);
  ASSERT_EQ(fb_compare_by_type_dec(&b, &a) > 0, TRUE);
  ASSERT_EQ(fb_compare_by_type_dec(&a, &a) == 0, TRUE);
}
