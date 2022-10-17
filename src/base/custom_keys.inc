#include "tkc.h"
#include "enums.h"
#include "conf_io/conf_json.h"

#if defined(WITH_SDL) || defined(LINUX)

#ifndef CUSTOM_KEYS_FILEPATH
#define CUSTOM_KEYS_FILEPATH "asset://custom_keys.json"
#endif/*CUSTOM_KEYS_FILEPATH*/

#endif /*WITH_SDL*/

#ifdef CUSTOM_KEYS_FILEPATH

static uint32_t s_custom_keys_nr = 0;
static key_type_value_t* s_custom_keys = NULL;

static key_type_value_t* keys_type_custom_keys_load(uint32_t* nr) {
  key_type_value_t* ret = NULL;
  tk_object_t* conf = NULL;
  return_value_if_fail(nr != NULL, NULL);

  conf = conf_json_load(CUSTOM_KEYS_FILEPATH, FALSE);
  return_value_if_fail(conf != NULL, NULL);

  *nr = tk_object_get_prop_uint32(conf, "#size", 0);
  if (*nr > 0) {
    uint32_t i = 0;
    char key[TK_NAME_LEN + 1] = {0};
    ret = TKMEM_ZALLOCN(key_type_value_t, *nr);

    for (i = 0; i < *nr; i++) {
      tk_snprintf(key, sizeof(key), "[%d].#name", i);
      ret[i].name = tk_strdup(tk_object_get_prop_str(conf, key));

      tk_snprintf(key, sizeof(key), "[%d].type", i);
      ret[i].type = tk_object_get_prop_uint32(conf, key, 0);

      tk_snprintf(key, sizeof(key), "[%d].value", i);
      ret[i].value = tk_object_get_prop_uint32(conf, key, 0);
    }
  }
  TK_OBJECT_UNREF(conf);

  return ret;
}

static ret_t keys_type_custom_keys_unload(key_type_value_t* table, uint32_t nr) {
  return_value_if_fail(table != NULL || nr == 0, RET_BAD_PARAMS);

  if (table != NULL) {
    uint32_t i = 0;
    for (i = 0; i < nr; i++) {
      TKMEM_FREE(table[i].name);
    }
    TKMEM_FREE(table);
  }

  return RET_OK;
}

static ret_t custom_keys_init(void) {
  s_custom_keys = keys_type_custom_keys_load(&s_custom_keys_nr);
  keys_type_set_custom_keys(s_custom_keys, s_custom_keys_nr);

  return RET_OK;
}

static ret_t custom_keys_deinit(void) {
  keys_type_set_custom_keys(NULL, 0);
  keys_type_custom_keys_unload(s_custom_keys, s_custom_keys_nr);

  return RET_OK;
}
#else
#define custom_keys_init()
#define custom_keys_deinit()
#endif /*CUSTOM_KEYS_FILEPATH*/

