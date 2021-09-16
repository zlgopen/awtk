#include <stdlib.h>
#include "gtest/gtest.h"
#include "base/image_manager.h"
#include "image_loader/image_loader_stb.h"
#include <string>

using std::string;

TEST(ImageManager, basic) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));
  ASSERT_EQ(image_manager_get_bitmap(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_get_bitmap(image_manager(), "not found", &bmp), RET_NOT_FOUND);
  ASSERT_EQ(image_manager_lookup(image_manager(), "unchecked", &bmp), RET_NOT_FOUND);

  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}

TEST(ImageManager, allloc) {
  bitmap_t* bmp = bitmap_create();
  ASSERT_EQ(image_manager_get_bitmap(image_manager(), "checked", bmp), RET_OK);
  bitmap_destroy_with_self(bmp);

  bmp = bitmap_create();
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", bmp), RET_OK);
  ASSERT_EQ(image_manager_get_bitmap(image_manager(), "not found", bmp), RET_NOT_FOUND);
  bitmap_destroy_with_self(bmp);

  bmp = bitmap_create();
  ASSERT_EQ(image_manager_lookup(image_manager(), "unchecked", bmp), RET_NOT_FOUND);
  bitmap_destroy_with_self(bmp);

  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}

TEST(ImageManager, unload) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));

  ASSERT_EQ(image_manager_get_bitmap(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_unload_bitmap(image_manager(), &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_NOT_FOUND);
}

TEST(ImageManager, unload_unused) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));

  ASSERT_EQ(image_manager_get_bitmap(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_NOT_FOUND);

  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}

TEST(ImageManager, add) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));

  bmp.w = 10;
  bmp.h = 10;

  ASSERT_EQ(image_manager_add(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}

TEST(ImageManager, locale) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));
  image_manager_t* imm = image_manager_create();
  assets_manager_t* am = assets_manager_create(0);

  assets_manager_set_res_root(am, "tests/testdata");
  image_manager_set_assets_manager(imm, am);

  ASSERT_EQ(image_manager_get_bitmap(imm, "locale_$locale$", &bmp), RET_OK);
  ASSERT_EQ(string(bmp.name), string("locale_en"));

  ASSERT_EQ(image_manager_get_bitmap(imm, "locale1_$locale$", &bmp), RET_OK);
  ASSERT_EQ(string(bmp.name), string("locale1_en_US"));

  ASSERT_EQ(image_manager_get_bitmap(imm, "locale_$locale$", &bmp), RET_OK);
  ASSERT_EQ(string(bmp.name), string("locale_en"));

  ASSERT_EQ(image_manager_get_bitmap(imm, "locale1_$locale$", &bmp), RET_OK);
  ASSERT_EQ(string(bmp.name), string("locale1_en_US"));
  ASSERT_EQ(image_manager_unload_unused(imm, 0), RET_OK);

  assets_manager_destroy(am);
  image_manager_destroy(imm);
}

#ifdef WITH_FS_RES
TEST(ImageManager, fs) {
  bitmap_t bmp;
  const char* filename = "file://./res/assets/default/raw/images/xx/flag_CN.png";

  memset(&bmp, 0x00, sizeof(bmp));
  ASSERT_EQ(image_manager_get_bitmap(image_manager(), filename, &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), filename, &bmp), RET_OK);
  ASSERT_EQ(image_manager_unload_bitmap(image_manager(), &bmp), RET_OK);
}
#endif /*WITH_FS_RES*/

TEST(ImageManager, limit) {
  bitmap_t b1;
  bitmap_t b2;
  bitmap_t b3;
  image_manager_t* imm = image_manager();
  memset(&b1, 0x00, sizeof(b1));
  memset(&b2, 0x00, sizeof(b1));
  memset(&b3, 0x00, sizeof(b1));

  bitmap_init(&b1, 10, 20, BITMAP_FMT_RGBA8888, NULL);
  bitmap_init(&b2, 10, 30, BITMAP_FMT_RGBA8888, NULL);
  bitmap_init(&b3, 10, 40, BITMAP_FMT_RGBA8888, NULL);

  image_manager_unload_all(imm);

  ASSERT_EQ(image_manager_add(imm, "b1", &b1), RET_OK);
  ASSERT_EQ(imm->images.size, 1u);

  ASSERT_EQ(image_manager_add(imm, "b2", &b2), RET_OK);
  ASSERT_EQ(imm->images.size, 2u);

  image_manager_set_max_mem_size_of_cached_images(imm, bitmap_get_mem_size(&b1));

  ASSERT_EQ(image_manager_add(imm, "b3", &b3), RET_OK);
  ASSERT_EQ(imm->images.size, 1u);
}
