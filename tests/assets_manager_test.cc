#include "base/assets_manager.h"
#include "gtest/gtest.h"
#include "tkc/str.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "base/system_info.h"

TEST(AssetsManager, basic) {
  const asset_info_t* null_res = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  asset_info_t img1 = {ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BMP, TRUE, 100, 0, "img1"};
  asset_info_t img2 = {ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, TRUE, 101, 0, "img2"};
  asset_info_t img3 = {ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BSVG, TRUE, 102, 0, "img2"};
  asset_info_t ui1 = {ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, TRUE, 103, 0, "ui1"};
  asset_info_t ui2 = {ASSET_TYPE_UI, ASSET_TYPE_UI_XML, TRUE, 104, 0, "ui2"};
  asset_info_t script1 = {ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_JS, TRUE, 105, 0, "script1"};
  asset_info_t script2 = {ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_LUA, TRUE, 106, 0, "script1"};
  asset_info_t font1 = {ASSET_TYPE_FONT, ASSET_TYPE_FONT_TTF, TRUE, 107, 0, "font1"};
  asset_info_t font2 = {ASSET_TYPE_FONT, ASSET_TYPE_FONT_BMP, TRUE, 108, 0, "font1"};

  ASSERT_EQ(assets_manager_add(rm, &img1), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &img2), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &img3), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &ui1), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &ui2), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &script1), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &script2), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &font1), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &font2), RET_OK);

  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_IMAGE, "img1"), &img1);
  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_IMAGE, "img2"), &img2);
  ASSERT_EQ(assets_manager_ref_ex(rm, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, "img2"), &img2);
  ASSERT_EQ(assets_manager_ref_ex(rm, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BSVG, "img2"), &img3);
  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_IMAGE, "img3"), null_res);

  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_UI, "ui1"), &ui1);
  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_UI, "ui2"), &ui2);

  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_SCRIPT, "script1"), &script1);
  ASSERT_EQ(assets_manager_ref_ex(rm, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_JS, "script1"),
            &script1);
  ASSERT_EQ(assets_manager_ref_ex(rm, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_LUA, "script1"),
            &script2);

  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_FONT, "font1"), &font1);
  ASSERT_EQ(assets_manager_ref_ex(rm, ASSET_TYPE_FONT, ASSET_TYPE_FONT_TTF, "font1"), &font1);
  ASSERT_EQ(assets_manager_ref_ex(rm, ASSET_TYPE_FONT, ASSET_TYPE_FONT_BMP, "font1"), &font2);

  assets_manager_destroy(rm);
}

static bool_t load_assets_refcount_is_no_problem(assets_manager_t* rm, asset_type_t type,
                                                 const char* name) {
  const asset_info_t* r = NULL;
  r = assets_manager_ref(rm, type, name);

  if (r == NULL) {
    return FALSE;
  }
  if (!((r->refcount == 1 && !assets_manager_is_save_assets_list(type)) ||
        (r->refcount == 2 && assets_manager_is_save_assets_list(type)))) {
    return FALSE;
  }
  if (assets_manager_unref(rm, r) != RET_OK) {
    return FALSE;
  }

  return TRUE;
}

static bool_t file_path_load_assets_refcount_is_no_problem(assets_manager_t* rm, asset_type_t type,
                                                           const char* name) {
  asset_info_t* r = NULL;
  const char* ratio = "x1";
  char path[MAX_PATH] = {0};
  const char* type_dir = NULL;
  system_info_t* sysinfo = system_info();
  uint32_t len = tk_strlen(STR_SCHEMA_FILE);
  const char* res_root = assets_manager_get_res_root(rm);

  if (sysinfo->device_pixel_ratio >= 3) {
    ratio = "x3";
  } else if (sysinfo->device_pixel_ratio >= 2) {
    ratio = "x2";
  }

  switch (type) {
    case ASSET_TYPE_FONT: {
      type_dir = "fonts";
      break;
    }
    case ASSET_TYPE_SCRIPT: {
      type_dir = "scripts";
      break;
    }
    case ASSET_TYPE_STYLE: {
      type_dir = "styles";
      break;
    }
    case ASSET_TYPE_STRINGS: {
      type_dir = "strings";
      break;
    }
    case ASSET_TYPE_IMAGE: {
      type_dir = "images";
      break;
    }
    case ASSET_TYPE_UI: {
      type_dir = "ui";
      break;
    }
    case ASSET_TYPE_XML: {
      type_dir = "xml";
      break;
    }
    case ASSET_TYPE_DATA: {
      type_dir = "data";
      break;
    }
    default:
      break;
  }
  if (type_dir == NULL) {
    return FALSE;
  }

  tk_str_append(path, MAX_PATH, STR_SCHEMA_FILE);
  if (type == ASSET_TYPE_IMAGE) {
    path_build(path + len, MAX_PATH - len, res_root, "assets", "default", "raw", type_dir, ratio,
               NULL);
  } else {
    path_build(path + len, MAX_PATH - len, res_root, "assets", "default", "raw", type_dir, NULL);
  }
  tk_str_append(path, MAX_PATH, "/");
  tk_str_append(path, MAX_PATH, name);

  r = assets_manager_load(rm, type, path);

  if (r == NULL) {
    return FALSE;
  }
  if (!((r->refcount == 1 && !assets_manager_is_save_assets_list(type)) ||
        (r->refcount == 2 && assets_manager_is_save_assets_list(type)))) {
    return FALSE;
  }
  if (assets_manager_unref(rm, r) != RET_OK) {
    return FALSE;
  }

  return TRUE;
}

TEST(AssetsManager, load_assets_refcount) {
  assets_manager_t* rm = assets_manager_create(10);

  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_UI, "main"), TRUE);
  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_IMAGE, "earth"), TRUE);
  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_SCRIPT, "dummy"), TRUE);
  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_XML, "test"), TRUE);
  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_DATA, "test.dat"), TRUE);

  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_FONT, "default"), TRUE);
  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_STYLE, "default"), TRUE);
  ASSERT_EQ(load_assets_refcount_is_no_problem(rm, ASSET_TYPE_STRINGS, "zh_CN"), TRUE);

  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_UI, "main.bin"), TRUE);
  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_IMAGE, "earth.png"), TRUE);
  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_SCRIPT, "dummy.js"), TRUE);
  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_XML, "test.xml"), TRUE);
  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_DATA, "test.dat"), TRUE);

  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_FONT, "default.ttf"), TRUE);
  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_STYLE, "default.bin"),
            TRUE);
  ASSERT_EQ(file_path_load_assets_refcount_is_no_problem(rm, ASSET_TYPE_STRINGS, "zh_CN.bin"),
            TRUE);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, file_image) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  r = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(r->refcount, 1);
  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, long_name) {
  asset_info_t* r = NULL;
  const char* ratio = "x1";
  char path[MAX_PATH] = {0};
  const char* name = "earth.png";
  system_info_t* sysinfo = system_info();
  uint32_t len = tk_strlen(STR_SCHEMA_FILE);
  assets_manager_t* am = assets_manager_create(10);
  const char* res_root = assets_manager_get_res_root(am);

  if (sysinfo->device_pixel_ratio >= 3) {
    ratio = "x3";
  } else if (sysinfo->device_pixel_ratio >= 2) {
    ratio = "x2";
  }

  tk_str_append(path, MAX_PATH, STR_SCHEMA_FILE);
  path_build(path + len, MAX_PATH - len, res_root, "assets", "default", "raw", "images", ratio,
             NULL);
  tk_str_append(path, MAX_PATH, "/");
  tk_str_append(path, MAX_PATH, name);

  r = assets_manager_load(am, ASSET_TYPE_IMAGE, path);
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(assets_manager_add(am, r), RET_OK);
  ASSERT_EQ(assets_manager_find_in_cache(am, ASSET_TYPE_IMAGE, 0, path) != NULL, true);
  ASSERT_EQ(assets_manager_find_in_cache(am, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, path) != NULL,
            true);
  ASSERT_EQ(asset_info_unref(r), RET_OK);

  assets_manager_destroy(am);
}

TEST(AssetsManager, file_script) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  r = assets_manager_ref(rm, ASSET_TYPE_SCRIPT, "dummy");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(r->refcount, 1);

  ASSERT_EQ(assets_manager_add(rm, r), RET_OK);
  ASSERT_EQ(r->refcount, 2);

  r = assets_manager_find_in_cache(rm, ASSET_TYPE_SCRIPT, 0, "dummy");
  ASSERT_EQ(r != NULL, true);

  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_SCRIPT, 0, "dummy");
  ASSERT_EQ(r->refcount, 1);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, clearCache) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  r = assets_manager_find_in_cache(rm, ASSET_TYPE_IMAGE, 0, "earth");
  ASSERT_EQ(r == NULL, true);

  r = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);

  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_IMAGE, 0, "earth");
  ASSERT_EQ(r == NULL, true);

  r = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);

  ASSERT_EQ(assets_manager_clear_cache(rm, ASSET_TYPE_IMAGE), RET_OK);

  r = assets_manager_find_in_cache(rm, ASSET_TYPE_IMAGE, 0, "earth");
  ASSERT_EQ(r == NULL, true);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, xml) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  r = assets_manager_ref(rm, ASSET_TYPE_XML, "test");
  ASSERT_EQ(r != NULL, true);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_XML, 0, "test");
  ASSERT_EQ(r != NULL, true);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, data) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();

#ifdef WITH_FS_RES
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "test.dat");
  ASSERT_EQ(r != NULL, true);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, 0, "test.dat");
  ASSERT_EQ(r != NULL, true);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, json) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  str_t s;
  str_init(&s, 0);
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "com.zlg.app.json");
  ASSERT_EQ(r != NULL, true);
  str_set_with_len(&s, (const char*)(r->data), r->size);
  str_replace(&s, "\r\n", "\n");
  ASSERT_STREQ(s.str, "{}\n");
  str_reset(&s);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, 0, "com.zlg.app.json");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(strncmp((const char*)(r->data), "{}\n", 3), 0);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, json_dupname) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "test.json");
  ASSERT_EQ(r != NULL, true);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, 0, "test.json");
  ASSERT_EQ(r != NULL, true);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, any) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  str_t s;
  str_init(&s, 0);
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "a-b-c.any");
  ASSERT_EQ(r != NULL, true);
  str_set_with_len(&s, (const char*)(r->data), r->size);
  str_replace(&s, "\r\n", "\n");
  ASSERT_STREQ(s.str, "abc\n");
  str_reset(&s);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, 0, "a-b-c.any");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(strncmp((const char*)(r->data), "abc\n", 4), 0);
#endif /*WITH_FS_RES*/
}

static asset_info_t* custom_load_asset(assets_manager_t* am, asset_type_t type, uint16_t subtype,
                                       const char* name) {
  asset_info_t* info = asset_info_create(ASSET_TYPE_DATA, 0, "test.any", 5);
  memcpy(info->data, "abcd", 5);

  return info;
}

TEST(AssetsManager, custom_load_asset) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();

  assets_manager_set_custom_load_asset(rm, custom_load_asset, NULL);
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "test.any");

  ASSERT_EQ(r != NULL, true);
  ASSERT_STREQ((const char*)(r->data), "abcd");
  assets_manager_set_custom_load_asset(rm, NULL, NULL);

  assets_manager_unref(rm, r);
}
