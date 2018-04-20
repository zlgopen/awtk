#include "base/mem.h"
#include "gtest/gtest.h"
#include "tools/common/utils.h"
#include "base/image_manager.h"
#include "base/resource_manager.h"
#include "tools/image_gen/image_gen.h"
#include "image_loader/image_loader_stb.h"

#define PNG_NAME LFTK_ROOT "/tests/testdata/test.png"
#define JPG_NAME LFTK_ROOT "/tests/testdata/test.jpg"
#define PNG_OPAQUE_NAME LFTK_ROOT "/tests/testdata/test_opaque.png"

static ret_t load_image(const char* filename, bitmap_t* image) {
  uint32_t size = 0;
  ret_t ret = RET_OK;
  image_loader_t* loader = image_loader_stb();
  printf("%s\n", filename);
  uint8_t* buff = (uint8_t*)read_file(filename, &size);
  ret = image_loader_load(loader, buff, size, image);
  MEM_FREE(buff);

  return ret;
}

TEST(ImageLoaderStb, basic) {
  bitmap_t image;
  ret_t ret = load_image(PNG_NAME, &image);

  ASSERT_EQ(ret, RET_OK);
  ASSERT_EQ(32, image.w);
  ASSERT_EQ(32, image.h);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_IMMUTABLE), true);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_OPAQUE), false);

  ret = load_image(JPG_NAME, &image);
  ASSERT_EQ(ret, RET_OK);
  ASSERT_EQ(32, image.w);
  ASSERT_EQ(32, image.h);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_IMMUTABLE), true);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_OPAQUE), true);

  ret = load_image(PNG_OPAQUE_NAME, &image);
  ASSERT_EQ(ret, RET_OK);
  ASSERT_EQ(32, image.w);
  ASSERT_EQ(32, image.h);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_IMMUTABLE), true);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_OPAQUE), true);
}

static ret_t add_image_res(const char* filename, const char* name) {
  bitmap_t image;
  static uint8_t buff[8092];
  ret_t ret = load_image(filename, &image);
  resource_info_t* r = (resource_info_t*)buff;
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  strcpy(r->name, name);
  r->type = RESOURCE_TYPE_IMAGE;
  r->subtype = RESOURCE_TYPE_IMAGE_RAW;
  r->size = image_gen_buff(&image, r->data, sizeof(buff) - sizeof(resource_info_t));

  return resource_manager_add(buff);
}

TEST(ImageLoaderStb, gen) {
  bitmap_t image;
  ASSERT_EQ(add_image_res(PNG_OPAQUE_NAME, "test.png"), RET_OK);
  ASSERT_EQ(image_manager_load(default_im(), "test.png", &image), RET_OK);

  ASSERT_EQ(32, image.w);
  ASSERT_EQ(32, image.h);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_IMMUTABLE), true);
  ASSERT_EQ(!!(image.flags & BITMAP_FLAG_OPAQUE), true);
}
