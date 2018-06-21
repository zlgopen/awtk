/**
 * File:   locale.c
 * Author: AWTK Develop Team
 * Brief:  locale
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/mem.h"
#include "base/utils.h"
#include "base/locale.h"

static tklocale_t* s_locale = NULL;

tklocale_t* locale() {
  return s_locale;
}

ret_t locale_set(tklocale_t* locale) {
  s_locale = locale;

  return RET_OK;
}

tklocale_t* locale_create(const char* language, const char* country) {
  tklocale_t* locale = TKMEM_ZALLOC(tklocale_t);

  return locale_init(locale, language, country);
}

tklocale_t* locale_init(tklocale_t* locale, const char* language, const char* country) {
  return_value_if_fail(locale != NULL, NULL);
  return_value_if_fail(emitter_init(&(locale->emitter)) != NULL, NULL);

  locale_change(locale, language, country);

  return locale;
}

const char* locale_tr(tklocale_t* locale, const char* text) {
  str_table_t* table = NULL;
  const char* tr_text = NULL;
  return_value_if_fail(locale != NULL && text != NULL, text);
  return_value_if_fail(locale->strs != NULL, text);

  table = (str_table_t*)(locale->strs->data);

  tr_text = str_table_lookup(table, text);

  return tr_text != NULL ? tr_text : text;
}

#define DEFAULT_COUNTRY "US"
#define DEFAULT_LANGUAGE "en"

ret_t locale_change(tklocale_t* locale, const char* language, const char* country) {
  return_value_if_fail(locale != NULL, RET_BAD_PARAMS);
  if (country == NULL) {
    country = DEFAULT_COUNTRY;
  }

  if (language == NULL) {
    language = DEFAULT_LANGUAGE;
  }

  if (strcmp(locale->country, country) != 0 || strcmp(locale->language, language) != 0) {
    char name[16];
    event_t e = event_init(EVT_LOCALE_CHANGED, locale);

    tk_strncpy(locale->country, country, sizeof(locale->country) - 1);
    tk_strncpy(locale->language, language, sizeof(locale->language) - 1);

    if (locale->strs != NULL) {
      resource_manager_unref(resource_manager(), locale->strs);
      locale->strs = NULL;

      /*清除字符串缓存*/
      resource_manager_clear_cache(resource_manager(), RESOURCE_TYPE_STRINGS);
    }

    tk_snprintf(name, sizeof(name) - 1, "%s_%s", locale->language, locale->country);
    locale->strs = resource_manager_ref(resource_manager(), RESOURCE_TYPE_STRINGS, name);
    if (locale->strs == NULL) {
      locale->strs =
          resource_manager_ref(resource_manager(), RESOURCE_TYPE_STRINGS, locale->language);
    }

    emitter_dispatch(&(locale->emitter), &e);
  }

  return RET_OK;
}

uint32_t locale_on(tklocale_t* locale, event_type_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(locale != NULL && on_event != NULL, 0);

  type = EVT_LOCALE_CHANGED;

  return emitter_on(&(locale->emitter), type, on_event, ctx);
}

ret_t locale_off(tklocale_t* locale, uint32_t id) {
  return_value_if_fail(locale != NULL, RET_BAD_PARAMS);

  return emitter_off(&(locale->emitter), id);
}

ret_t locale_deinit(tklocale_t* locale) {
  return_value_if_fail(locale != NULL, RET_OK);

  emitter_deinit(&(locale->emitter));
  if (locale->strs != NULL) {
    resource_manager_unref(resource_manager(), locale->strs);
  }
  memset(locale, 0x00, sizeof(tklocale_t));

  return RET_OK;
}

ret_t locale_destroy(tklocale_t* locale) {
  return_value_if_fail(locale != NULL, RET_BAD_PARAMS);

  locale_deinit(locale);
  TKMEM_FREE(locale);

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
