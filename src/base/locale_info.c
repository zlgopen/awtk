/**
 * File:   locale_info.c
 * Author: AWTK Develop Team
 * Brief:  locale_info
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/locale_info.h"

static locale_info_t* s_locale = NULL;

locale_info_t* locale_info() {
  return s_locale;
}

ret_t locale_info_set(locale_info_t* locale_info) {
  s_locale = locale_info;

  return RET_OK;
}

static locale_info_t* locale_info_init(locale_info_t* locale_info, const char* language, const char* country, assets_manager_t* am) {
  return_value_if_fail(locale_info != NULL, NULL);

  locale_info->assets_manager = am;
  locale_info->emitter = emitter_create();
  locale_info_change(locale_info, language, country);

  return locale_info;
}

static locale_info_t* locale_info_create_internal(const char* language, const char* country, assets_manager_t* am) {
  locale_info_t* locale_info = TKMEM_ZALLOC(locale_info_t);
  return locale_info_init(locale_info, language, country, am);
}

locale_info_t* locale_info_create(const char* language, const char* country) {
  return locale_info_create_internal(language, country, NULL);
}

const char* locale_info_tr(locale_info_t* locale_info, const char* text) {
  str_table_t* table = NULL;
  const char* tr_text = NULL;
  return_value_if_fail(locale_info != NULL && text != NULL, text);
  return_value_if_fail(locale_info->strs != NULL, text);

  table = (str_table_t*)(locale_info->strs->data);

  tr_text = str_table_lookup(table, text);
  if (tr_text == NULL && locale_info->fallback_tr != NULL) {
    tr_text = locale_info->fallback_tr(text);
  }

  return tr_text != NULL ? tr_text : text;
}

#define DEFAULT_COUNTRY "US"
#define DEFAULT_LANGUAGE "en"

static assets_manager_t* locale_info_get_assets_manager(locale_info_t* locale_info) {
  return_value_if_fail(locale_info != NULL, NULL);

  return locale_info->assets_manager != NULL ? locale_info->assets_manager : assets_manager();
}

ret_t locale_info_reload(locale_info_t* locale_info) {
  char name[16];
  assets_manager_t* am = locale_info_get_assets_manager(locale_info);

  if (locale_info->strs != NULL) {
    assets_manager_unref(am, locale_info->strs);
    locale_info->strs = NULL;

    /*清除字符串缓存*/
    assets_manager_clear_cache(am, ASSET_TYPE_STRINGS);
  }

  tk_snprintf(name, sizeof(name) - 1, "%s_%s", locale_info->language, locale_info->country);
  locale_info->strs = assets_manager_ref(am, ASSET_TYPE_STRINGS, name);
  if (locale_info->strs == NULL) {
    locale_info->strs = assets_manager_ref(am, ASSET_TYPE_STRINGS, locale_info->language);
  }

  return RET_OK;
}

ret_t locale_info_change(locale_info_t* locale_info, const char* language, const char* country) {
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);

  if (country == NULL) {
    country = DEFAULT_COUNTRY;
  }

  if (language == NULL) {
    language = DEFAULT_LANGUAGE;
  }

  if (strcmp(locale_info->country, country) != 0 || strcmp(locale_info->language, language) != 0) {
    event_t e = event_init(EVT_LOCALE_CHANGED, locale_info);

    tk_strncpy(locale_info->country, country, sizeof(locale_info->country) - 1);
    tk_strncpy(locale_info->language, language, sizeof(locale_info->language) - 1);

    locale_info_reload(locale_info);

    emitter_dispatch(locale_info->emitter, &e);
  }

  return RET_OK;
}

uint32_t locale_info_on(locale_info_t* locale_info, event_type_t type, event_func_t on_event,
                        void* ctx) {
  return_value_if_fail(locale_info != NULL && on_event != NULL, 0);

  type = EVT_LOCALE_CHANGED;

  return emitter_on(locale_info->emitter, type, on_event, ctx);
}

ret_t locale_info_off(locale_info_t* locale_info, uint32_t id) {
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);

  return emitter_off(locale_info->emitter, id);
}

ret_t locale_info_set_assets_manager(locale_info_t* locale_info, assets_manager_t* assets_manager) {
  return_value_if_fail(locale_info != NULL && assets_manager != NULL, RET_BAD_PARAMS);

  locale_info->assets_manager = assets_manager;

  return RET_OK;
}

ret_t locale_info_set_fallback_tr(locale_info_t* locale_info, locale_info_tr_t tr) {
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);

  locale_info->fallback_tr = tr;

  return RET_OK;
}

static ret_t locale_info_deinit(locale_info_t* locale_info) {
  assets_manager_t* am = locale_info_get_assets_manager(locale_info);
  return_value_if_fail(locale_info != NULL, RET_OK);

  emitter_destroy(locale_info->emitter);
  if (locale_info->strs != NULL) {
    assets_manager_unref(am, locale_info->strs);
  }
  memset(locale_info, 0x00, sizeof(locale_info_t));

  return RET_OK;
}

ret_t locale_info_destroy(locale_info_t* locale_info) {
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);

  locale_info_deinit(locale_info);
  TKMEM_FREE(locale_info);

  return RET_OK;
}

static darray_t* s_locale_infos = NULL;

static int locale_info_cmp_by_name(locale_info_t* locale_info, const char* name) {
  if (tk_str_eq(locale_info->name, name)) {
    return 0;
  }
  return -1;
}

static const char* locale_info_fallback_tr_default(const char* text) {
  return locale_info_tr(locale_info(), text);
}

locale_info_t* locale_infos_ref(const char* name) {
  locale_info_t* info = NULL;
  return_value_if_fail(name != NULL, NULL);

  if (s_locale_infos == NULL) {
    s_locale_infos =
        darray_create(3, (tk_destroy_t)locale_info_destroy, (tk_compare_t)locale_info_cmp_by_name);
  }
  return_value_if_fail(s_locale_infos != NULL, NULL);

  info = (locale_info_t*)darray_find(s_locale_infos, (void*)name);
  if (info == NULL) {
    assets_manager_t* am;
    locale_info_t* ginfo = locale_info();
    return_value_if_fail(ginfo != NULL, NULL);

    am = assets_managers_ref(name);
    info = locale_info_create_internal(ginfo->language, ginfo->country, am);
    return_value_if_fail(info != NULL, NULL);

    info->name = tk_strdup(name);
    locale_info_set_fallback_tr(info, locale_info_fallback_tr_default);
    assets_manager_set_locale_info(am, info);
    darray_push(s_locale_infos, info);
  } else {
    info->refcount++;
  }

  return info;
}

ret_t locale_infos_unref(locale_info_t* locale_info) {
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_locale_infos != NULL, RET_BAD_PARAMS);
  assert(locale_info->refcount > 0);

  if (locale_info->refcount == 1) {
    assets_manager_t* am = locale_info->assets_manager;
    assets_manager_set_locale_info(am, NULL);
    assets_managers_unref(am);
    darray_remove(s_locale_infos, locale_info);
    if (s_locale_infos->size == 0) {
      darray_destroy(s_locale_infos);
      s_locale_infos = NULL;
    }
  } else {
    locale_info->refcount--;
  }

  return RET_OK;
}

ret_t locale_infos_change(const char* language, const char* country) {
  locale_info_change(locale_info(), language, country);

  if (s_locale_infos != NULL) {
    uint32_t i = 0;
    for (i = 0; i < s_locale_infos->size; i++) {
      locale_info_t* info = (locale_info_t*)darray_get(s_locale_infos, i);
      locale_info_change(info, language, country);
    }
  }

  return RET_OK;
}

ret_t locale_infos_reload_all(void) {
  locale_info_reload(locale_info());

  if (s_locale_infos != NULL) {
    uint32_t i = 0;
    for (i = 0; i < s_locale_infos->size; i++) {
      locale_info_t* info = (locale_info_t*)darray_get(s_locale_infos, i);
      locale_info_reload(info);
    }
  }

  return RET_OK;
}

const char* str_table_lookup(const str_table_t* table, const char* key) {
  int low = 0;
  int mid = 0;
  int high = 0;
  int result = 0;
  const char* iter = NULL;
  const char* start = (const char*)table;
  return_value_if_fail(table != NULL && table->nr > 0 && key != NULL, NULL);

  high = table->nr - 1;
  while (low <= high) {
    mid = low + ((high - low) >> 1);
    iter = start + table->strs[mid].key;

    result = strcmp(iter, key);

    if (result == 0) {
      return start + table->strs[mid].value;
    } else if (result < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return NULL;
}
