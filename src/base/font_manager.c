/**
 * File:   font_manager.c
 * Author: AWTK Develop Team
 * Brief:  font manager
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/utils.h"
#include "base/events.h"
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
  memset(fm, 0x00, sizeof(font_manager_t));

  darray_init(&(fm->fonts), 2, (tk_destroy_t)font_destroy, (tk_compare_t)font_cmp);

  fm->loader = loader;
  fm->assets_manager = NULL;
  fm->refcount = 1;
  fm->name = NULL;

  return fm;
}

static ret_t font_manager_on_asset_events(void* ctx, event_t* e) {
  assets_event_t* evt = (assets_event_t*)e;
  font_manager_t* fm = (font_manager_t*)ctx;

  if (evt->type == ASSET_TYPE_FONT) {
    asset_info_t* info = evt->asset_info;
    if (e->type == EVT_ASSET_MANAGER_CLEAR_CACHE) {
      font_manager_unload_all(fm);
    } else if (e->type == EVT_ASSET_MANAGER_UNLOAD_ASSET) {
      font_manager_unload_font(fm, asset_info_get_name(info), 0);
    }
  }

  return RET_OK;
}

ret_t font_manager_set_assets_manager(font_manager_t* fm, assets_manager_t* am) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  if (fm->assets_manager != NULL && fm->assets_manager != am) {
    emitter_off_by_ctx(EMITTER(fm->assets_manager), fm);
  }

  if (am != NULL && fm->assets_manager != am) {
    emitter_on(EMITTER(am), EVT_ASSET_MANAGER_CLEAR_CACHE, font_manager_on_asset_events, fm);
    emitter_on(EMITTER(am), EVT_ASSET_MANAGER_UNLOAD_ASSET, font_manager_on_asset_events, fm);
  }

  fm->assets_manager = am;

  return RET_OK;
}

ret_t font_manager_add_font(font_manager_t* fm, font_t* font) {
  return_value_if_fail(fm != NULL && font != NULL, RET_BAD_PARAMS);
  
  if (font->fm == NULL) {
    font->fm = fm;
    darray_push(&(fm->fonts), font);
  }

  return RET_OK;
}

#if WITH_BITMAP_FONT
static const char* font_manager_fix_bitmap_font_name(char str[MAX_PATH], const char* name,
                                                     font_size_t size) {
  memset(str, 0, MAX_PATH);
  tk_snprintf(str, MAX_PATH, "%s_%d", name, size);
  return str;
}
#endif

font_t* font_manager_lookup(font_manager_t* fm, const char* name, font_size_t size) {
#if WITH_BITMAP_FONT
  font_t* font = NULL;
  char font_name[MAX_PATH];
  font_cmp_info_t info_bitmap;
#endif

  font_cmp_info_t info = {name, size};
  return_value_if_fail(fm != NULL, NULL);

#if WITH_BITMAP_FONT
  info_bitmap.name = font_manager_fix_bitmap_font_name(font_name, name, size);
  info_bitmap.size = size;
  font = darray_find(&(fm->fonts), &info_bitmap);
  if (font != NULL) {
    return font;
  }
#endif

  return darray_find(&(fm->fonts), &info);
}

font_t* font_manager_load(font_manager_t* fm, const char* name, uint32_t size) {
  font_t* font = NULL;
  if (fm->loader != NULL) {
    const asset_info_t* info = NULL;

#if WITH_BITMAP_FONT
    char font_name[MAX_PATH];
    font_manager_fix_bitmap_font_name(font_name, name, size);
    info = assets_manager_ref(fm->assets_manager, ASSET_TYPE_FONT, font_name);
    if (info != NULL) {
      name = font_name;
    }
#endif

    if (info == NULL) {
      info = assets_manager_ref(fm->assets_manager, ASSET_TYPE_FONT, name);
    }

    if (info != NULL) {
      if (info->subtype == fm->loader->type) {
        font = font_loader_load(fm->loader, name, info->data, info->size);
      }
      assets_manager_unref(fm->assets_manager, info);
    } else {
      if (fm->fallback_get_font != NULL) {
        return fm->fallback_get_font(fm->fallback_get_font_ctx, name, size);
      }
    }
  }

  return font;
}

font_t* font_manager_get_font(font_manager_t* fm, const char* name, font_size_t size) {
  font_t* font = NULL;
  const char* default_font = system_info()->default_font;
  name = system_info_fix_font_name(name);
  return_value_if_fail(fm != NULL, NULL);

  font = font_manager_lookup(fm, name, size);
  if (font == NULL) {
    font = font_manager_load(fm, name, size);
    if (font != NULL) {
      font_manager_add_font(fm, font);
    } else if (tk_str_cmp(name, default_font) != 0) {
      font = font_manager_get_font(fm, default_font, size);
    }
  }

  return font;
}

ret_t font_manager_unload_font(font_manager_t* fm, const char* name, font_size_t size) {
  ret_t ret = RET_OK;
  font_cmp_info_t info = {name, size};
  event_t e;

#if WITH_BITMAP_FONT
  char font_name[MAX_PATH];
  font_cmp_info_t info_bitmap;
#endif

  name = system_info_fix_font_name(name);
  return_value_if_fail(fm != NULL, RET_FAIL);

#if WITH_BITMAP_FONT
  info_bitmap.name = font_manager_fix_bitmap_font_name(font_name, name, size);
  info_bitmap.size = size;
  ret = darray_remove(&(fm->fonts), &info_bitmap);
#endif

  e = event_init(EVT_ASSET_MANAGER_UNLOAD_ASSET, (void*)name);
  emitter_dispatch(EMITTER(fm), &e);

  ret = darray_remove(&(fm->fonts), &info);
  if (ret == RET_OK) {
    assets_manager_clear_cache_ex(assets_manager(), ASSET_TYPE_FONT, name);
  }

  return ret;
}

ret_t font_manager_unload_all(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_FAIL);

  if (fm->fonts.elms != NULL) {
    uint32_t i = 0;
    void** elms = fm->fonts.elms;

    for (i = 0; i < fm->fonts.size; i++) {
      font_t* iter = (font_t*)(elms[i]);
      event_t e = event_init(EVT_ASSET_MANAGER_UNLOAD_ASSET, iter->name);
      emitter_dispatch(EMITTER(fm), &e);
    }
  }

  return darray_clear(&(fm->fonts));
}

ret_t font_manager_deinit(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(fm->name);
  return darray_deinit(&(fm->fonts));
}

ret_t font_manager_destroy(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);
  emitter_off_by_ctx(EMITTER(fm->assets_manager), fm);
  font_manager_deinit(fm);
  TKMEM_FREE(fm);

  return RET_OK;
}

ret_t font_manager_shrink_cache(font_manager_t* fm, uint32_t cache_size) {
  uint32_t i = 0;
  font_t* font = NULL;
  return_value_if_fail(fm != NULL, RET_FAIL);

  for (i = 0; i < fm->fonts.size; i++) {
    font = (font_t*)darray_get(&(fm->fonts), i);
    font_shrink_cache(font, cache_size);
  }

  return RET_OK;
}

ret_t font_manager_set_fallback_get_font(font_manager_t* fm,
                                         font_manager_get_font_t fallback_get_font, void* ctx) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  fm->fallback_get_font = fallback_get_font;
  fm->fallback_get_font_ctx = ctx;

  return RET_OK;
}

static darray_t* s_font_managers = NULL;

static int font_manager_cmp_by_name(font_manager_t* fm, const char* name) {
  if (tk_str_eq(fm->name, name)) {
    return 0;
  }

  return -1;
}

font_t* font_manager_fallback_get_font_default(void* ctx, const char* name,
                                                      font_size_t size) {
  font_manager_t* fm = (font_manager_t*)ctx;
  
  if (fm != font_manager()) {
    /*非默认的fm才需要fallback到默认的fm上*/
    return font_manager_get_font(font_manager(), name, size);
  } else {
    return NULL;
  }
}

font_manager_t* font_managers_ref(const char* name) {
  font_manager_t* fm = NULL;
  return_value_if_fail(name != NULL, NULL);

  if (s_font_managers == NULL) {
    s_font_managers = darray_create(3, (tk_destroy_t)font_manager_destroy,
                                    (tk_compare_t)font_manager_cmp_by_name);
  }
  return_value_if_fail(s_font_managers != NULL, NULL);

  fm = (font_manager_t*)darray_find(s_font_managers, (void*)name);
  if (fm == NULL) {
    fm = font_manager_create(s_font_manager->loader);
    return_value_if_fail(fm != NULL, NULL);
    fm->name = tk_strdup(name);
    darray_push(s_font_managers, fm);
    font_manager_set_assets_manager(fm, assets_managers_ref(name));
    font_manager_set_fallback_get_font(fm, font_manager_fallback_get_font_default, fm);
  } else {
    fm->refcount++;
  }

  return fm;
}

ret_t font_managers_unref(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_font_managers != NULL, RET_BAD_PARAMS);

  assert(fm->refcount > 0);
  if (fm->refcount == 1) {
    assets_managers_unref(fm->assets_manager);
    darray_remove(s_font_managers, fm);
    if (s_font_managers->size == 0) {
      darray_destroy(s_font_managers);
      s_font_managers = NULL;
    }
  } else {
    fm->refcount--;
  }

  return RET_OK;
}

ret_t font_managers_unload_all(void) {
  font_manager_unload_all(font_manager());

  if (s_font_managers != NULL) {
    uint32_t i = 0;
    for (i = 0; i < s_font_managers->size; i++) {
      font_manager_t* fm = (font_manager_t*)darray_get(s_font_managers, i);
      font_manager_unload_all(fm);
    }
  }

  return RET_OK;
}
