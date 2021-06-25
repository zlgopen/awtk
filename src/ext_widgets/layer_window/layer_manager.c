
/**
 * File:   layer_manager.c
 * Author: AWTK Develop Team
 * Brief:  LCD layer manager。
 *
 * Copyright (c) Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/mem.h"
#include "layer_window/layer_manager.h"

layer_manager_t* layer_manager_create(void) {
  layer_manager_t* layer_manager = TKMEM_ZALLOC(layer_manager_t);
  return_value_if_fail(layer_manager != NULL, NULL);

  return layer_manager;
}

ret_t layer_manager_add(layer_manager_t* layer_manager, layer_t* layer) {
  return_value_if_fail(layer_manager != NULL && layer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(layer_manager->nr < TK_MAX_LAYERS_NR, RET_BAD_PARAMS)

      layer_manager->layers[layer_manager->nr++] = layer;

  return RET_OK;
}

layer_t* layer_manager_find(layer_manager_t* layer_manager, const char* name) {
  uint32_t i = 0;
  return_value_if_fail(layer_manager != NULL && name != NULL, NULL);
  for (i = 0; i < layer_manager->nr; i++) {
    layer_t* iter = layer_manager->layers[i];
    if (tk_str_eq(name, iter->name)) {
      return iter;
    }
  }

  return NULL;
}

ret_t layer_manager_destroy(layer_manager_t* layer_manager) {
  uint32_t i = 0;
  return_value_if_fail(layer_manager != NULL, RET_BAD_PARAMS);
  for (i = 0; i < layer_manager->nr; i++) {
    layer_t* iter = layer_manager->layers[i];
    OBJECT_UNREF(iter);
    layer_manager->layers[i] = NULL;
  }
  layer_manager->nr = 0;
  TKMEM_FREE(layer_manager);

  return RET_OK;
}

static layer_manager_t* s_layer_manager = NULL;
layer_manager_t* layer_manager(void) {
  return s_layer_manager;
}

ret_t layer_manager_set(layer_manager_t* layer_manager) {
  s_layer_manager = layer_manager;

  return RET_OK;
}
