/**
 * File:   widget_pool.c
 * Author: AWTK Develop Team
 * Brief:  widget pool
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/darray.h"
#include "base/widget.h"
#include "base/widget_pool.h"

static widget_pool_t* s_widget_pool;

static ret_t widget_pool_reset_widget(widget_t* widget) {
  if (widget->name != NULL) {
    widget->name[0] = '\0';
  }

  if (widget->animation != NULL) {
    widget->animation[0] = '\0';
  }

  if (widget->style != NULL) {
    widget->style[0] = '\0';
  }

  if (widget->tr_text != NULL) {
    widget->tr_text[0] = '\0';
  }

  widget->floating = FALSE;
  widget->auto_created = FALSE;

  widget->parent = NULL;
  widget->target = NULL;
  widget->emitter = NULL;
  widget->children = NULL;
  widget->key_target = NULL;
  widget->self_layout = NULL;
  widget->grab_widget = NULL;
  widget->custom_props = NULL;
  widget->children_layout = NULL;

  return RET_OK;
}

static ret_t widget_pool_real_destroy_widget(widget_t* widget) {
  if (widget->vt->on_destroy) {
    widget->vt->on_destroy(widget);
  }

  TKMEM_FREE(widget->name);
  TKMEM_FREE(widget->style);
  TKMEM_FREE(widget->tr_text);
  TKMEM_FREE(widget->animation);

  memset(widget, 0x00, sizeof(widget_t));
  TKMEM_FREE(widget);

  return RET_OK;
}

static widget_t* widget_pool_real_create_widget(const widget_vtable_t* vt) {
  widget_t* widget = TKMEM_ALLOC(vt->size);
  return_value_if_fail(widget != NULL, NULL);

  memset(widget, 0x00, vt->size);
  widget->vt = vt;

  return widget;
}

widget_pool_t* widget_pool(void) {
  return s_widget_pool;
}

ret_t widget_pool_set(widget_pool_t* pool) {
  s_widget_pool = pool;

  return RET_OK;
}

widget_pool_t* widget_pool_create(uint32_t max_cache_nr) {
  widget_pool_t* pool = TKMEM_ZALLOC(widget_pool_t);
  return_value_if_fail(pool != NULL, NULL);

  pool->max_cache_nr = max_cache_nr;
  pool->cache = object_default_create();

  if (pool->cache == NULL) {
    TKMEM_FREE(pool);
    pool = NULL;
  }

  return pool;
}

static widget_t* widget_pool_get(widget_pool_t* pool, const char* type) {
  value_t v;
  if (object_get_prop(pool->cache, type, &v) == RET_OK) {
    darray_t* arr = (darray_t*)value_pointer(&v);

    return (widget_t*)darray_pop(arr);
  }

  return NULL;
}

widget_t* widget_pool_create_widget(widget_pool_t* pool, const widget_vtable_t* vt) {
  widget_t* widget = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(widget_t), NULL);

  if (pool != NULL && vt->enable_pool) {
    widget = widget_pool_get(pool, vt->type);
    if (widget != NULL) {
      pool->cache_nr--;
    }
  }

  if (widget == NULL) {
    widget = widget_pool_real_create_widget(vt);
  }

  return widget;
}

static ret_t widget_pool_put(widget_pool_t* pool, widget_t* widget) {
  if (pool->cache_nr < pool->max_cache_nr && widget->vt->enable_pool) {
    value_t v;
    darray_t* arr = NULL;
    const char* type = widget->vt->type;

    if (object_get_prop(pool->cache, type, &v) == RET_OK) {
      arr = (darray_t*)value_pointer(&v);
    }

    if (arr == NULL) {
      arr = darray_create(10, (tk_destroy_t)widget_pool_real_destroy_widget, NULL);
      return_value_if_fail(arr != NULL, RET_OOM);

      value_set_pointer(&v, arr);
      if (object_set_prop(pool->cache, type, &v) != RET_OK) {
        darray_destroy(arr);
        arr = NULL;
      }
    }

    return darray_push(arr, widget);
  } else {
    return RET_FAIL;
  }
}

ret_t widget_pool_destroy_widget(widget_pool_t* pool, widget_t* widget) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (pool != NULL) {
    ret = widget_pool_put(pool, widget);
    if (ret == RET_OK) {
      pool->cache_nr++;
      widget_pool_reset_widget(widget);
    }
  }

  if (ret != RET_OK) {
    ret = widget_pool_real_destroy_widget(widget);
  }

  return ret;
}

static ret_t widget_pool_on_visit_remove_cache(void* ctx, named_value_t* nv) {
  darray_t* arr = (darray_t*)value_pointer(&(nv->value));

  darray_destroy(arr);
  value_set_pointer(&(nv->value), NULL);

  return RET_REMOVE;
}

ret_t widget_pool_clear_cache(widget_pool_t* pool) {
  return_value_if_fail(pool != NULL, RET_BAD_PARAMS);

  object_foreach_prop(pool->cache, (tk_visit_t)widget_pool_on_visit_remove_cache, NULL);
  pool->cache_nr = 0;

  return RET_OK;
}

static ret_t widget_pool_on_visit_count(void* ctx, named_value_t* nv) {
  int32_t* n = (int32_t*)ctx;
  darray_t* arr = NULL;
  return_value_if_fail(nv != NULL, RET_OK);

  arr = (darray_t*)value_pointer(&(nv->value));
  return_value_if_fail(arr != NULL, RET_OK);

  *n += arr->size;

  return RET_OK;
}

int32_t widget_pool_count(widget_pool_t* pool) {
  int32_t n = 0;
  return_value_if_fail(pool != NULL, 0);

  object_foreach_prop(pool->cache, (tk_visit_t)widget_pool_on_visit_count, &n);

  return n;
}

ret_t widget_pool_destroy(widget_pool_t* pool) {
  return_value_if_fail(pool != NULL && pool->cache != NULL, RET_BAD_PARAMS);

  widget_pool_clear_cache(pool);
  object_unref(pool->cache);
  TKMEM_FREE(pool);

  return RET_OK;
}
