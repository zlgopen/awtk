#include "tkc.h"
#include "enums.h"
#include "conf_io/conf_json.h"

#ifndef ENABLE_CUSTOM_KEYS
#define ENABLE_CUSTOM_KEYS FALSE
#endif /* ENABLE_CUSTOM_KEYS */

#if defined(WITH_SDL) || defined(LINUX)

#ifndef CUSTOM_KEYS_FILEPATH
#define CUSTOM_KEYS_FILEPATH "asset://custom_keys.json"
#endif /*CUSTOM_KEYS_FILEPATH*/

#endif /* defined(WITH_SDL) || defined(LINUX) */

#if ENABLE_CUSTOM_KEYS && defined(CUSTOM_KEYS_FILEPATH)

static uint32_t s_custom_keys_nr = 0;
static key_type_value_t* s_custom_keys = NULL;

static key_type_value_t* custom_keys_load(uint32_t* nr) {
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

static ret_t custom_keys_unload(key_type_value_t* table, uint32_t nr) {
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

static void custom_keys_init(bool_t set) {
  s_custom_keys = custom_keys_load(&s_custom_keys_nr);
  if (set) {
    keys_type_set_custom_keys(s_custom_keys, s_custom_keys_nr);
  }
}

static void custom_keys_deinit(bool_t set) {
  if (set) {
    keys_type_set_custom_keys(NULL, 0);
  }
  custom_keys_unload(s_custom_keys, s_custom_keys_nr);
  s_custom_keys = NULL;
  s_custom_keys_nr = 0;
}

static inline const key_type_value_t* custom_keys(void) {
  return s_custom_keys;
}

static inline uint32_t custom_keys_nr(void) {
  return s_custom_keys_nr;
}
#else
#define custom_keys_init(set)
#define custom_keys_deinit(set)
#define custom_keys() (NULL)
#define custom_keys_nr() (0)
#endif /* ENABLE_CUSTOM_KEYS && defined(CUSTOM_KEYS_FILEPATH) */
