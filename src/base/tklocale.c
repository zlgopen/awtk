/**
 * File:   tklocale.c
 * Author: AWTK Develop Team
 * Brief:  tklocale
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
#include "base/tklocale.h"

static tklocale_t* s_locale = NULL;

tklocale_t* tklocale() {
  return s_locale;
}

ret_t tklocale_set(tklocale_t* tklocale) {
  s_locale = tklocale;

  return RET_OK;
}

tklocale_t* tklocale_create(const char* language, const char* country) {
  tklocale_t* tklocale = TKMEM_ZALLOC(tklocale_t);

  return tklocale_init(tklocale, language, country);
}

tklocale_t* tklocale_init(tklocale_t* tklocale, const char* language, const char* country) {
  return_value_if_fail(tklocale != NULL, NULL);

  tklocale->emitter = emitter_create(); 
  tklocale_change(tklocale, language, country);

  return tklocale;
}

const char* tklocale_tr(tklocale_t* tklocale, const char* text) {
  str_table_t* table = NULL;
  const char* tr_text = NULL;
  return_value_if_fail(tklocale != NULL && text != NULL, text);
  return_value_if_fail(tklocale->strs != NULL, text);

  table = (str_table_t*)(tklocale->strs->data);

  tr_text = str_table_lookup(table, text);

  return tr_text != NULL ? tr_text : text;
}

#define DEFAULT_COUNTRY "US"
#define DEFAULT_LANGUAGE "en"

ret_t tklocale_change(tklocale_t* tklocale, const char* language, const char* country) {
  return_value_if_fail(tklocale != NULL, RET_BAD_PARAMS);
  if (country == NULL) {
    country = DEFAULT_COUNTRY;
  }

  if (language == NULL) {
    language = DEFAULT_LANGUAGE;
  }

  if (strcmp(tklocale->country, country) != 0 || strcmp(tklocale->language, language) != 0) {
    char name[16];
    event_t e = event_init(EVT_LOCALE_CHANGED, tklocale);

    tk_strncpy(tklocale->country, country, sizeof(tklocale->country) - 1);
    tk_strncpy(tklocale->language, language, sizeof(tklocale->language) - 1);

    if (tklocale->strs != NULL) {
      resource_manager_unref(resource_manager(), tklocale->strs);
      tklocale->strs = NULL;

      /*清除字符串缓存*/
      resource_manager_clear_cache(resource_manager(), RESOURCE_TYPE_STRINGS);
    }

    tk_snprintf(name, sizeof(name) - 1, "%s_%s", tklocale->language, tklocale->country);
    tklocale->strs = resource_manager_ref(resource_manager(), RESOURCE_TYPE_STRINGS, name);
    if (tklocale->strs == NULL) {
      tklocale->strs =
          resource_manager_ref(resource_manager(), RESOURCE_TYPE_STRINGS, tklocale->language);
    }

    emitter_dispatch(tklocale->emitter, &e);
  }

  return RET_OK;
}

uint32_t tklocale_on(tklocale_t* tklocale, event_type_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(tklocale != NULL && on_event != NULL, 0);

  type = EVT_LOCALE_CHANGED;

  return emitter_on(tklocale->emitter, type, on_event, ctx);
}

ret_t tklocale_off(tklocale_t* tklocale, uint32_t id) {
  return_value_if_fail(tklocale != NULL, RET_BAD_PARAMS);

  return emitter_off(tklocale->emitter, id);
}

ret_t tklocale_deinit(tklocale_t* tklocale) {
  return_value_if_fail(tklocale != NULL, RET_OK);

  emitter_destroy(tklocale->emitter);
  if (tklocale->strs != NULL) {
    resource_manager_unref(resource_manager(), tklocale->strs);
  }
  memset(tklocale, 0x00, sizeof(tklocale_t));

  return RET_OK;
}

ret_t tklocale_destroy(tklocale_t* tklocale) {
  return_value_if_fail(tklocale != NULL, RET_BAD_PARAMS);

  tklocale_deinit(tklocale);
  TKMEM_FREE(tklocale);

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
