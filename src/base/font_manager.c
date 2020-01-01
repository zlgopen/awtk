/**
 * File:   font_manager.c
 * Author: AWTK Develop Team
 * Brief:  font manager
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/system_info.h"
#include "base/font_manager.h"

static font_manager_t* s_font_manager = NULL;

typedef struct _font_cmp_info_t {
  const char* name;
  uint32_t size;
} font_cmp_info_t;

static int32_t font_cmp(font_t* font, font_cmp_info_t* info) {
  if (font_match(font, info->name, info->size)) {
    return 0;
  }

  return -1;
}

font_manager_t* font_manager(void) {
  return s_font_manager;
}

ret_t font_manager_set(font_manager_t* fm) {
  s_font_manager = fm;

  return RET_OK;
}

font_manager_t* font_manager_create(font_loader_t* loader) {
  font_manager_t* fm = TKMEM_ZALLOC(font_manager_t);

  return font_manager_init(fm, loader);
}

font_manager_t* font_manager_init(font_manager_t* fm, font_loader_t* loader) {
  return_value_if_fail(fm != NULL, NULL);
  darray_init(&(fm->fonts), 2, (tk_destroy_t)font_destroy, (tk_compare_t)font_cmp);

  fm->loader = loader;

  return fm;
}

ret_t font_manager_set_assets_manager(font_manager_t* fm, assets_manager_t* am) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  fm->assets_manager = am;

  return RET_OK;
}

ret_t font_manager_add_font(font_manager_t* fm, font_t* font) {
  return_value_if_fail(fm != NULL && font != NULL, RET_BAD_PARAMS);

  return darray_push(&(fm->fonts), font);
}

font_t* font_manager_lookup(font_manager_t* fm, const char* name, font_size_t size) {
  font_cmp_info_t info = {name, size};
  return_value_if_fail(fm != NULL, NULL);

  return darray_find(&(fm->fonts), &info);
}

font_t* font_manager_load(font_manager_t* fm, const char* name, uint32_t size) {
  font_t* font = NULL;
  if (fm->loader != NULL) {
    const asset_info_t* info = assets_manager_ref(assets_manager(), ASSET_TYPE_FONT, name);

    if (info != NULL) {
      if (info->subtype == fm->loader->type) {
        font = font_loader_load(fm->loader, name, info->data, info->size);
      }
      assets_manager_unref(assets_manager(), info);
    }
  }

  return font;
}

font_t* font_manager_get_font(font_manager_t* fm, const char* name, font_size_t size) {
  font_t* font = NULL;

  name = system_info_fix_font_name(name);
  return_value_if_fail(fm != NULL, NULL);

  font = font_manager_lookup(fm, name, size);
  if (font == NULL) {
    font = font_manager_load(fm, name, size);
    if (font != NULL) {
      font_manager_add_font(fm, font);
    }
  }

  if (font == NULL) {
    font_t** fonts = (font_t**)fm->fonts.elms;
    font = fonts[0];
  }

  return font;
}

ret_t font_manager_unload_font(font_manager_t* fm, const char* name, font_size_t size) {
  font_t* font = NULL;
  font_cmp_info_t info = {name, size};

  name = system_info_fix_font_name(name);
  return_value_if_fail(fm != NULL, RET_FAIL);

  font = font_manager_lookup(fm, name, size);
  return_value_if_fail(font != NULL, RET_NOT_FOUND);

  return darray_remove(&(fm->fonts), &info);
}

ret_t font_manager_unload_all(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_FAIL);

  return darray_clear(&(fm->fonts));
}

ret_t font_manager_deinit(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  return darray_deinit(&(fm->fonts));
}

ret_t font_manager_destroy(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);
  font_manager_deinit(fm);
  TKMEM_FREE(fm);

  return RET_OK;
}
