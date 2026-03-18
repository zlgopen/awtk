/**
 * File:   object_evt_proxy.c
 * Author: AWTK Develop Team
 * Brief:  事件代理对象。
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-03-18 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "object_evt_proxy.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_hash.h"
#include "tkc/darray.h"
#include "tkc/named_value.h"

typedef struct _object_evt_proxy_register_info_t {
  tk_object_t* publisher;
  int32_t evt_type;
  object_evt_proxy_register_opt_t opt;
} object_evt_proxy_register_info_t;

static ret_t object_evt_proxy_register_info_deinit(object_evt_proxy_register_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (info->publisher != NULL) {
    tk_object_unref(info->publisher);
  }
  return RET_OK;
}

static ret_t object_evt_proxy_register_info_destroy(object_evt_proxy_register_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  object_evt_proxy_register_info_deinit(info);
  TKMEM_FREE(info);
  return RET_OK;
}

static ret_t object_evt_proxy_register_info_init(
    object_evt_proxy_register_info_t* info, tk_object_t* publisher, int32_t evt_type,
    const object_evt_proxy_register_opt_t* opt) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  memset(info, 0, sizeof(object_evt_proxy_register_info_t));
  info->publisher = tk_object_ref(publisher);
  return_value_if_fail(info->publisher != NULL, RET_FAIL);
  info->evt_type = evt_type;
  if (opt != NULL) {
    info->opt = *opt;
  }
  return RET_OK;
}

static object_evt_proxy_register_info_t* object_evt_proxy_register_info_create(
    tk_object_t* publisher, int32_t evt_type, const object_evt_proxy_register_opt_t* opt) {
  object_evt_proxy_register_info_t* ret = NULL;
  return_value_if_fail(publisher != NULL, NULL);

  ret = (object_evt_proxy_register_info_t*)TKMEM_ALLOC(sizeof(object_evt_proxy_register_info_t));
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(RET_OK == object_evt_proxy_register_info_init(ret, publisher, evt_type, opt));

  return ret;
error:
  object_evt_proxy_register_info_destroy(ret);
  return NULL;
}

typedef struct _object_evt_proxy_subscribe_info_t {
  char* topic;
  object_evt_proxy_subscribe_callback_t callback;
  object_evt_proxy_subscribe_opt_t opt;
} object_evt_proxy_subscribe_info_t;

static ret_t object_evt_proxy_subscribe_info_destroy(object_evt_proxy_subscribe_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (info->opt.subscriber != NULL) {
    tk_object_unref(info->opt.subscriber);
  }
  TKMEM_FREE(info->topic);
  TKMEM_FREE(info);
  return RET_OK;
}

static object_evt_proxy_subscribe_info_t* object_evt_proxy_subscribe_info_create(
    const char* topic, object_evt_proxy_subscribe_callback_t callback,
    const object_evt_proxy_subscribe_opt_t* opt) {
  object_evt_proxy_subscribe_info_t* ret = NULL;
  return_value_if_fail(callback != NULL, NULL);

  ret = TKMEM_ZALLOC(object_evt_proxy_subscribe_info_t);
  return_value_if_fail(ret != NULL, NULL);

  if (topic != NULL) {
    ret->topic = tk_strdup(topic);
    goto_error_if_fail(ret->topic != NULL);
  }

  ret->callback = callback;

  if (opt != NULL) {
    ret->opt = *opt;
    if (opt->subscriber != NULL) {
      ret->opt.subscriber = tk_object_ref(opt->subscriber);
    }
  }

  return ret;
error:
  object_evt_proxy_subscribe_info_destroy(ret);
  return NULL;
}

static int object_evt_proxy_subscribe_info_cmp(const object_evt_proxy_subscribe_info_t* info1,
                                               const object_evt_proxy_subscribe_info_t* info2) {
  int ret = pointer_compare(info1->callback, info2->callback);
  if (0 != ret) {
    return ret;
  }
  ret = pointer_compare(info1->opt.subscriber, info2->opt.subscriber);
  if (0 != ret) {
    return ret;
  }
  ret = pointer_compare(info1->opt.callback_ctx, info2->opt.callback_ctx);
  if (0 != ret) {
    return ret;
  }
  return tk_strcmp(info1->topic, info2->topic);
}

struct _object_evt_proxy_t {
  tk_object_t object;
  tk_object_t* register_infos;
  darray_t subscribe_infos;
  /* cache */
  darray_t* matched_subscribe_infos;
};

static ret_t object_evt_proxy_publisher_event_off_on_visit(void* ctx, const void* data) {
  const named_value_t* nv = (const named_value_t*)(data);
  const object_evt_proxy_register_info_t* info =
      (const object_evt_proxy_register_info_t*)value_pointer(&nv->value);
  emitter_off_by_ctx(EMITTER(info->publisher), ctx);
  return RET_OK;
}

static ret_t object_evt_proxy_on_destroy(tk_object_t* obj) {
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL, RET_BAD_PARAMS);

  tk_object_foreach_prop(evt_proxy->register_infos, object_evt_proxy_publisher_event_off_on_visit,
                         evt_proxy);
  if (evt_proxy->matched_subscribe_infos != NULL) {
    darray_destroy(evt_proxy->matched_subscribe_infos);
    evt_proxy->matched_subscribe_infos = NULL;
  }
  darray_deinit(&evt_proxy->subscribe_infos);
  TK_OBJECT_UNREF(evt_proxy->register_infos);

  return RET_OK;
}

static const object_vtable_t s_object_evt_proxy_vtable = {
    .type = OBJECT_EVT_PROXY_TYPE,
    .desc = OBJECT_EVT_PROXY_TYPE,
    .size = sizeof(object_evt_proxy_t),
    .on_destroy = object_evt_proxy_on_destroy,
    .compare = tk_object_compare_name_without_nullptr,
};

tk_object_t* object_evt_proxy_create(void) {
  object_evt_proxy_t* ret = (object_evt_proxy_t*)tk_object_create(&s_object_evt_proxy_vtable);
  return_value_if_fail(ret != NULL, NULL);

  ret->register_infos = object_hash_create_ex(FALSE);
  goto_error_if_fail(ret->register_infos != NULL);

  goto_error_if_fail(NULL != darray_init(&ret->subscribe_infos, 16,
                                         (tk_destroy_t)object_evt_proxy_subscribe_info_destroy,
                                         (tk_compare_t)object_evt_proxy_subscribe_info_cmp));

  return TK_OBJECT(ret);
error:
  tk_object_unref(TK_OBJECT(ret));
  return NULL;
}

typedef struct _object_evt_proxy_find_matched_subscribe_on_visit_ctx_t {
  object_evt_proxy_t* evt_proxy;
  event_t* e;
} object_evt_proxy_find_matched_subscribe_on_visit_ctx_t;

static ret_t object_evt_proxy_find_matched_subscribe_on_visit(void* ctx, const void* data) {
  object_evt_proxy_find_matched_subscribe_on_visit_ctx_t* actx =
      (object_evt_proxy_find_matched_subscribe_on_visit_ctx_t*)(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  const object_evt_proxy_register_info_t* info =
      (const object_evt_proxy_register_info_t*)value_pointer(&nv->value);

  if (info->evt_type == actx->e->type && info->publisher == actx->e->target) {
    if (NULL == info->opt.evt_filter ||
        info->opt.evt_filter(info->publisher, actx->e, info->opt.evt_filter_ctx)) {
      uint32_t i = 0;
      for (i = 0; i < actx->evt_proxy->subscribe_infos.size; i++) {
        object_evt_proxy_subscribe_info_t* sub_info =
            (object_evt_proxy_subscribe_info_t*)darray_get(&actx->evt_proxy->subscribe_infos, i);
        if (NULL == sub_info->topic || tk_str_eq(nv->name, sub_info->topic)) {
          darray_push_unique(actx->evt_proxy->matched_subscribe_infos, sub_info);
        }
      }
    }
  }

  return RET_OK;
}

static ret_t object_evt_proxy_on_publish(void* ctx, event_t* e) {
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY((tk_object_t*)ctx);
  object_evt_proxy_find_matched_subscribe_on_visit_ctx_t actx = {
      .evt_proxy = evt_proxy,
      .e = e,
  };
  uint32_t i = 0;
  return_value_if_fail(evt_proxy != NULL, RET_BAD_PARAMS);

  if (NULL == evt_proxy->matched_subscribe_infos) {
    evt_proxy->matched_subscribe_infos = darray_create(4, NULL, evt_proxy->subscribe_infos.compare);
    return_value_if_fail(evt_proxy->matched_subscribe_infos != NULL, RET_OOM);
  }

  darray_clear(evt_proxy->matched_subscribe_infos);
  tk_object_foreach_prop(evt_proxy->register_infos,
                         object_evt_proxy_find_matched_subscribe_on_visit, &actx);
  for (i = 0; i < evt_proxy->matched_subscribe_infos->size; i++) {
    object_evt_proxy_subscribe_info_t* sub_info =
        (object_evt_proxy_subscribe_info_t*)darray_get(evt_proxy->matched_subscribe_infos, i);
    sub_info->callback(sub_info->opt.subscriber, e, sub_info->opt.callback_ctx);
  }

  return RET_OK;
}

ret_t object_evt_proxy_register(tk_object_t* obj, const char* topic, tk_object_t* publisher,
                                int32_t evt_type, const object_evt_proxy_register_opt_t* opt) {
  ret_t ret = RET_OK;
  object_evt_proxy_register_info_t* info = NULL;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && TK_STR_IS_NOT_EMPTY(topic) && publisher != NULL,
                       RET_BAD_PARAMS);
  return_value_if_fail(!tk_object_has_prop(evt_proxy->register_infos, topic), RET_FAIL);

  info = object_evt_proxy_register_info_create(publisher, evt_type, opt);
  return_value_if_fail(info != NULL, RET_FAIL);

  ret = tk_object_set_prop_pointer_ex(evt_proxy->register_infos, topic, info,
                                      (tk_destroy_t)object_evt_proxy_register_info_destroy);
  if (RET_OK != ret) {
    object_evt_proxy_register_info_destroy(info);
    return ret;
  }

  if (!emitter_exist(EMITTER(publisher), evt_type, object_evt_proxy_on_publish, obj)) {
    if (TK_INVALID_ID == emitter_on(EMITTER(publisher), evt_type, object_evt_proxy_on_publish, obj)) {
      object_evt_proxy_unregister(obj, topic);
      return RET_FAIL;
    }
  }

  return ret;
}

static int object_evt_proxy_unregister_cmp(const void* data, const void* ctx) {
  const named_value_t* nv = (const named_value_t*)data;
  const object_evt_proxy_register_info_t* info =
      (const object_evt_proxy_register_info_t*)value_pointer(&nv->value);
  const object_evt_proxy_register_info_t* target = (const object_evt_proxy_register_info_t*)ctx;
  int ret = info->evt_type - target->evt_type;
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(info->publisher, target->publisher);
}

ret_t object_evt_proxy_unregister(tk_object_t* obj, const char* topic) {
  ret_t ret = RET_OK;
  object_evt_proxy_register_info_t* info = NULL;
  object_evt_proxy_register_info_t tmp_info;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && TK_STR_IS_NOT_EMPTY(topic), RET_BAD_PARAMS);

  info = (object_evt_proxy_register_info_t*)tk_object_get_prop_pointer(evt_proxy->register_infos,
                                                                       topic);
  return_value_if_fail(info != NULL, RET_NOT_FOUND);

  ret = object_evt_proxy_register_info_init(&tmp_info, info->publisher, info->evt_type, &info->opt);
  return_value_if_fail(RET_OK == ret, ret);

  ret = tk_object_remove_prop(evt_proxy->register_infos, topic);

  if (RET_OK == ret) {
    if (NULL == tk_object_find_prop(evt_proxy->register_infos,
                                    (tk_compare_t)object_evt_proxy_unregister_cmp, &tmp_info)) {
      emitter_off_by_func(EMITTER(tmp_info.publisher), tmp_info.evt_type,
                          object_evt_proxy_on_publish, obj);
    }
  }

  object_evt_proxy_register_info_deinit(&tmp_info);

  return ret;
}

ret_t object_evt_proxy_subscribe(tk_object_t* obj, const char* topic,
                                 object_evt_proxy_subscribe_callback_t callback,
                                 const object_evt_proxy_subscribe_opt_t* opt) {
  object_evt_proxy_subscribe_info_t* info = NULL;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && callback != NULL, RET_BAD_PARAMS);

  info = object_evt_proxy_subscribe_info_create(topic, callback, opt);
  return_value_if_fail(info != NULL, RET_FAIL);

  return darray_push(&evt_proxy->subscribe_infos, info);
}

ret_t object_evt_proxy_unsubscribe(tk_object_t* obj, const char* topic,
                                   object_evt_proxy_subscribe_callback_t callback,
                                   const object_evt_proxy_subscribe_opt_t* opt) {
  object_evt_proxy_subscribe_info_t info = {
      .topic = (char*)topic,
      .callback = callback,
      .opt = (opt != NULL) ? *opt : (object_evt_proxy_subscribe_opt_t){0},
  };
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && callback != NULL, RET_BAD_PARAMS);

  return darray_remove(&evt_proxy->subscribe_infos, &info);
}

object_evt_proxy_t* object_evt_proxy_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_evt_proxy_vtable, NULL);
  return (object_evt_proxy_t*)obj;
}
