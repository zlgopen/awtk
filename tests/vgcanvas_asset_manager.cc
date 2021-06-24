#include "tkc/utils.h"
#include "base/bitmap.h"
#include "base/image_loader.h"
#include "base/assets_manager.h"
#include "base/vgcanvas_asset_manager.h"
#include "gtest/gtest.h"

#define TEST_VGCANAS1_PTR 0xe0
#define TEST_VGCANAS2_PTR 0xeb
#define TEST_VGCANAS3_PTR 0xef

static int32_t s_specific_by_vg1 = 0;
static int32_t s_specific_by_vg2 = 0;
static int32_t s_specific_by_vg3 = 0;

static ret_t vgcanvas_asset_manager_log_bitmap_destroy(void* vg, void* specific) {
  int32_t ptr = tk_pointer_to_int(vg);
  switch (ptr) {
    case TEST_VGCANAS1_PTR:
      s_specific_by_vg1 = tk_pointer_to_int(specific);
      break;
    case TEST_VGCANAS2_PTR:
      s_specific_by_vg2 = tk_pointer_to_int(specific);
      break;
    case TEST_VGCANAS3_PTR:
      s_specific_by_vg3 = tk_pointer_to_int(specific);
      break;
  }
  return RET_OK;
}

static ret_t vgcanvas_asset_manager_log_font_destroy(void* vg, const char* font_name,
                                                     void* specific) {
  int32_t ptr = tk_pointer_to_int(vg);
  switch (ptr) {
    case TEST_VGCANAS1_PTR:
      s_specific_by_vg1 = tk_pointer_to_int(specific);
      break;
    case TEST_VGCANAS2_PTR:
      s_specific_by_vg2 = tk_pointer_to_int(specific);
      break;
    case TEST_VGCANAS3_PTR:
      s_specific_by_vg3 = tk_pointer_to_int(specific);
      break;
  }
  return RET_OK;
}

TEST(Vgcanvas_asset_manager, image) {
  ret_t ret;
  int32_t ctx;
  bitmap_t image;
  int32_t image_ctx1 = 10;
  int32_t image_ctx2 = 100;
  int32_t image_ctx3 = 1000;
  void* vg1 = tk_pointer_from_int(TEST_VGCANAS1_PTR);
  void* vg2 = tk_pointer_from_int(TEST_VGCANAS2_PTR);
  void* vg3 = tk_pointer_from_int(TEST_VGCANAS3_PTR);
  assets_manager_t* rm = assets_manager_create(10);
  const asset_info_t* res = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");

  memset(&image, 0x00, sizeof(bitmap_t));
  ASSERT_EQ(vgcanvas_asset_manager_set(vgcanvas_asset_manager_create()), RET_OK);

  ASSERT_EQ(vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg1,
                                          vgcanvas_asset_manager_log_bitmap_destroy,
                                          vgcanvas_asset_manager_log_font_destroy),
            RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg2,
                                          vgcanvas_asset_manager_log_bitmap_destroy,
                                          vgcanvas_asset_manager_log_font_destroy),
            RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg3,
                                          vgcanvas_asset_manager_log_bitmap_destroy,
                                          vgcanvas_asset_manager_log_font_destroy),
            RET_OK);

  image_loader_load_image(res, &image);
  image.name = "earth";

  ASSERT_EQ(vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), vg1, &image,
                                             tk_pointer_from_int(image_ctx1)),
            RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), vg2, &image,
                                             tk_pointer_from_int(image_ctx2)),
            RET_OK);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg1, &image, &ret));
  ASSERT_EQ(ctx, image_ctx1);
  ASSERT_EQ(ret, RET_OK);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg2, &image, &ret));
  ASSERT_EQ(ctx, image_ctx2);
  ASSERT_EQ(ret, RET_OK);

  vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg3, &image, &ret);
  ASSERT_EQ(ret, RET_FAIL);

  ASSERT_EQ(vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager(), vg2), RET_OK);
  vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg2, &image, &ret);
  ASSERT_EQ(ret, RET_FAIL);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg1, &image, &ret));
  ASSERT_EQ(ctx, image_ctx1);
  ASSERT_EQ(ret, RET_OK);

  ASSERT_EQ(vgcanvas_asset_manager_remove_image(vgcanvas_asset_manager(), vg1, NULL), RET_OK);
  ASSERT_EQ(s_specific_by_vg1, image_ctx1);
  s_specific_by_vg1 = 0;

  vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg1, &image, &ret);
  ASSERT_EQ(ret, RET_FAIL);

  ASSERT_EQ(vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), vg1, &image,
                                             tk_pointer_from_int(image_ctx1)),
            RET_OK);

  ASSERT_EQ(vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), vg3, &image,
                                             tk_pointer_from_int(image_ctx3)),
            RET_OK);
  bitmap_destroy(&image);
  ASSERT_EQ(s_specific_by_vg1, image_ctx1);
  ASSERT_EQ(s_specific_by_vg2, 0);
  ASSERT_EQ(s_specific_by_vg3, image_ctx3);
  s_specific_by_vg1 = 0;
  s_specific_by_vg3 = 0;

  vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg1, &image, &ret);
  ASSERT_EQ(ret, RET_FAIL);

  vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg2, &image, &ret);
  ASSERT_EQ(ret, RET_FAIL);

  vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), vg3, &image, &ret);
  ASSERT_EQ(ret, RET_FAIL);

  ASSERT_EQ(vgcanvas_asset_manager_destroy(vgcanvas_asset_manager()), RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_set(NULL), RET_OK);

  assets_manager_unref(rm, res);
  assets_manager_destroy(rm);
}

TEST(Vgcanvas_asset_manager, font) {
  ret_t ret;
  int32_t ctx;
  int32_t font_ctx1 = 20;
  int32_t font_ctx2 = 200;
  int32_t font_ctx3 = 2000;
  void* vg1 = tk_pointer_from_int(TEST_VGCANAS1_PTR);
  void* vg2 = tk_pointer_from_int(TEST_VGCANAS2_PTR);
  void* vg3 = tk_pointer_from_int(TEST_VGCANAS3_PTR);

  ASSERT_EQ(vgcanvas_asset_manager_set(vgcanvas_asset_manager_create()), RET_OK);

  ASSERT_EQ(vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg1,
                                          vgcanvas_asset_manager_log_bitmap_destroy,
                                          vgcanvas_asset_manager_log_font_destroy),
            RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg2,
                                          vgcanvas_asset_manager_log_bitmap_destroy,
                                          vgcanvas_asset_manager_log_font_destroy),
            RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg3,
                                          vgcanvas_asset_manager_log_bitmap_destroy,
                                          vgcanvas_asset_manager_log_font_destroy),
            RET_OK);

  ASSERT_EQ(vgcanvas_asset_manager_add_font(vgcanvas_asset_manager(), vg1, "default",
                                            tk_pointer_from_int(font_ctx1)),
            RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_add_font(vgcanvas_asset_manager(), vg2, "default",
                                            tk_pointer_from_int(font_ctx2)),
            RET_OK);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg1, "default", &ret));
  ASSERT_EQ(ctx, font_ctx1);
  ASSERT_EQ(ret, RET_OK);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg2, "default", &ret));
  ASSERT_EQ(ctx, font_ctx2);
  ASSERT_EQ(ret, RET_OK);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg3, "default", &ret));
  ASSERT_EQ(ret, RET_FAIL);

  ASSERT_EQ(vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager(), vg2), RET_OK);
  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg2, "default", &ret));
  ASSERT_EQ(ret, RET_FAIL);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg1, "default", &ret));
  ASSERT_EQ(ctx, font_ctx1);
  ASSERT_EQ(ret, RET_OK);

  ASSERT_EQ(vgcanvas_asset_manager_add_font(vgcanvas_asset_manager(), vg3, "default",
                                            tk_pointer_from_int(font_ctx3)),
            RET_OK);

  vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager(), vg3, "default");
  ASSERT_EQ(s_specific_by_vg1, 0);
  ASSERT_EQ(s_specific_by_vg2, 0);
  ASSERT_EQ(s_specific_by_vg3, font_ctx3);
  s_specific_by_vg3 = 0;

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg1, "default", &ret));
  ASSERT_EQ(ctx, font_ctx1);
  ASSERT_EQ(ret, RET_OK);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg2, "default", &ret));
  ASSERT_EQ(ret, RET_FAIL);

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg3, "default", &ret));
  ASSERT_EQ(ret, RET_FAIL);

  vgcanvas_asset_manager_remove_font(vgcanvas_asset_manager(), vg1, NULL);
  ASSERT_EQ(s_specific_by_vg1, font_ctx1);
  s_specific_by_vg1 = 0;

  ctx = tk_pointer_to_int(
      vgcanvas_asset_manager_get_font_ctx(vgcanvas_asset_manager(), vg1, "default", &ret));
  ASSERT_EQ(ret, RET_FAIL);

  ASSERT_EQ(vgcanvas_asset_manager_destroy(vgcanvas_asset_manager()), RET_OK);
  ASSERT_EQ(vgcanvas_asset_manager_set(NULL), RET_OK);
}
