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

static ret_t object_evt_proxy_register_info_destroy(object_evt_proxy_register_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (info->publisher != NULL) {
    tk_object_unref(info->publisher);
  }
  TKMEM_FREE(info);
  return RET_OK;
}

static object_evt_proxy_register_info_t* object_evt_proxy_register_info_create(
    tk_object_t* publisher, int32_t evt_type, const object_evt_proxy_register_opt_t* opt) {
  object_evt_proxy_register_info_t* ret = NULL;
  return_value_if_fail(publisher != NULL, NULL);

  ret = TKMEM_ZALLOC(object_evt_proxy_register_info_t);
  return_value_if_fail(ret != NULL, NULL);

  ret->publisher = tk_object_ref(publisher);
  goto_error_if_fail(ret->publisher != NULL);

  ret->evt_type = evt_type;
  if (opt != NULL) {
    ret->opt = *opt;
  }

  return ret;
error:
  object_evt_proxy_register_info_destroy(ret);
  return NULL;
}

inline static int object_evt_proxy_register_opt_cmp(const object_evt_proxy_register_opt_t* opt1,
                                                    const object_evt_proxy_register_opt_t* opt2) {
  int ret = pointer_compare(opt1->evt_filter, opt2->evt_filter);
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(opt1->evt_filter_ctx, opt2->evt_filter_ctx);
}

inline static int object_evt_proxy_register_info_cmp(
    const object_evt_proxy_register_info_t* info1, const object_evt_proxy_register_info_t* info2) {
  int ret = info1->evt_type - info2->evt_type;
  if (0 != ret) {
    return ret;
  }
  ret = pointer_compare(info1->publisher, info2->publisher);
  if (0 != ret) {
    return ret;
  }
  return object_evt_proxy_register_opt_cmp(&info1->opt, &info2->opt);
}

inline static ret_t object_evt_proxy_register_infos_destroy(darray_t* infos) {
  return_value_if_fail(infos != NULL, RET_BAD_PARAMS);
  return darray_destroy(infos);
}

inline static darray_t* object_evt_proxy_register_infos_create(void) {
  return darray_create(4, (tk_destroy_t)object_evt_proxy_register_info_destroy,
                       (tk_compare_t)object_evt_proxy_register_info_cmp);
}

/*********************************************************************************************/

typedef struct _object_evt_proxy_subscribe_info_t {
  object_evt_proxy_subscribe_callback_t callback;
  object_evt_proxy_subscribe_opt_t opt;
} object_evt_proxy_subscribe_info_t;

static ret_t object_evt_proxy_subscribe_info_destroy(object_evt_proxy_subscribe_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (info->opt.subscriber != NULL) {
    tk_object_unref(info->opt.subscriber);
  }
  TKMEM_FREE(info);
  return RET_OK;
}

static object_evt_proxy_subscribe_info_t* object_evt_proxy_subscribe_info_create(
    object_evt_proxy_subscribe_callback_t callback, const object_evt_proxy_subscribe_opt_t* opt) {
  object_evt_proxy_subscribe_info_t* ret = NULL;
  return_value_if_fail(callback != NULL, NULL);

  ret = TKMEM_ZALLOC(object_evt_proxy_subscribe_info_t);
  return_value_if_fail(ret != NULL, NULL);

  ret->callback = callback;

  if (opt != NULL) {
    ret->opt = *opt;
    if (opt->subscriber != NULL) {
      ret->opt.subscriber = tk_object_ref(opt->subscriber);
    }
  }

  return ret;
}

inline static int object_evt_proxy_subscribe_opt_cmp(const object_evt_proxy_subscribe_opt_t* opt1,
                                                     const object_evt_proxy_subscribe_opt_t* opt2) {
  int ret = pointer_compare(opt1->subscriber, opt2->subscriber);
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(opt1->callback_ctx, opt2->callback_ctx);
}

inline static int object_evt_proxy_subscribe_info_cmp(
    const object_evt_proxy_subscribe_info_t* info1,
    const object_evt_proxy_subscribe_info_t* info2) {
  int ret = pointer_compare(info1->callback, info2->callback);
  if (0 != ret) {
    return ret;
  }
  return object_evt_proxy_subscribe_opt_cmp(&info1->opt, &info2->opt);
}

inline static int object_evt_proxy_subscribe_info_cmp_by_priority(
    const object_evt_proxy_subscribe_info_t* info1,
    const object_evt_proxy_subscribe_info_t* info2) {
  if (info1->opt.priority < info2->opt.priority) {
    return 1;
  } else if (info1->opt.priority > info2->opt.priority) {
    return -1;
  } else {
    return 0;
  }
}

inline static ret_t object_evt_proxy_subscribe_infos_destroy(darray_t* infos) {
  return_value_if_fail(infos != NULL, RET_BAD_PARAMS);
  return darray_destroy(infos);
}

inline static darray_t* object_evt_proxy_subscribe_infos_create(void) {
  return darray_create(4, (tk_destroy_t)object_evt_proxy_subscribe_info_destroy,
                       (tk_compare_t)object_evt_proxy_subscribe_info_cmp);
}

/*********************************************************************************************/

typedef ret_t (*object_evt_proxy_infos_group_foreach_after_infos_foreach_done_t)(darray_t* infos,
                                                                                 ret_t result,
                                                                                 void* ctx);

typedef struct _object_evt_proxy_infos_group_foreach_on_visit_ctx_t {
  tk_visit_t visit;
  void* ctx;
  object_evt_proxy_infos_group_foreach_after_infos_foreach_done_t after_infos_foreach_done;
} object_evt_proxy_infos_group_foreach_on_visit_ctx_t;

static ret_t object_evt_proxy_infos_group_foreach_on_visit(void* ctx, const void* data) {
  object_evt_proxy_infos_group_foreach_on_visit_ctx_t* actx =
      (object_evt_proxy_infos_group_foreach_on_visit_ctx_t*)(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  darray_t* infos = (darray_t*)value_pointer(&nv->value);
  ret_t ret = darray_foreach(infos, actx->visit, actx->ctx);
  if (actx->after_infos_foreach_done != NULL) {
    return actx->after_infos_foreach_done(infos, ret, actx->ctx);
  }
  return ret;
}

inline static ret_t object_evt_proxy_infos_group_foreach(
    tk_object_t* infos_group, tk_visit_t visit, void* ctx,
    object_evt_proxy_infos_group_foreach_after_infos_foreach_done_t after_infos_foreach_done) {
  object_evt_proxy_infos_group_foreach_on_visit_ctx_t actx = {
      .visit = visit,
      .ctx = ctx,
      .after_infos_foreach_done = after_infos_foreach_done,
  };
  return_value_if_fail(infos_group != NULL, RET_BAD_PARAMS);
  return tk_object_foreach_prop(infos_group, object_evt_proxy_infos_group_foreach_on_visit, &actx);
}

typedef struct _object_evt_proxy_infos_group_remove_on_visit_ctx_t {
  tk_compare_t compare;
  void* ctx;
  bool_t removed : 1;
} object_evt_proxy_infos_group_remove_on_visit_ctx_t;

static ret_t object_evt_proxy_infos_group_remove_on_visit(void* ctx, const void* data) {
  object_evt_proxy_infos_group_remove_on_visit_ctx_t* actx =
      (object_evt_proxy_infos_group_remove_on_visit_ctx_t*)(ctx);
  if (actx->removed) {
    return RET_STOP;
  }
  if (0 == actx->compare(data, actx->ctx)) {
    actx->removed = TRUE;
    return RET_REMOVE;
  }
  return RET_OK;
}

static ret_t object_evt_proxy_infos_group_remove_after_infos_foreach_done(darray_t* infos,
                                                                          ret_t result, void* ctx) {
  object_evt_proxy_infos_group_remove_on_visit_ctx_t* actx =
      (object_evt_proxy_infos_group_remove_on_visit_ctx_t*)(ctx);
  if (0 == infos->size) {
    return RET_REMOVE;
  }
  if (actx->removed) {
    return RET_STOP;
  }
  return result;
}

inline static ret_t object_evt_proxy_infos_group_remove(tk_object_t* infos_group,
                                                        tk_compare_t compare, void* ctx) {
  object_evt_proxy_infos_group_remove_on_visit_ctx_t actx = {
      .compare = (compare != NULL) ? compare : pointer_compare,
      .ctx = ctx,
  };
  return_value_if_fail(infos_group != NULL, RET_BAD_PARAMS);
  object_evt_proxy_infos_group_foreach(
      infos_group, object_evt_proxy_infos_group_remove_on_visit, &actx,
      object_evt_proxy_infos_group_remove_after_infos_foreach_done);
  return actx.removed ? RET_OK : RET_NOT_FOUND;
}

/*********************************************************************************************/

struct _object_evt_proxy_t {
  tk_object_t object;
  tk_object_t* register_infos_group;
  tk_object_t* subscribe_infos_group;
  /* cache */
  darray_t* matched_subscribe_infos;
  bool_t publishing : 1;
};

static ret_t object_evt_proxy_publisher_event_off_on_visit(void* ctx, const void* data) {
  const object_evt_proxy_register_info_t* info = (const object_evt_proxy_register_info_t*)(data);
  emitter_off_by_ctx(EMITTER(info->publisher), ctx);
  return RET_OK;
}

static ret_t object_evt_proxy_on_destroy(tk_object_t* obj) {
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL, RET_BAD_PARAMS);

  object_evt_proxy_infos_group_foreach(evt_proxy->register_infos_group,
                                       object_evt_proxy_publisher_event_off_on_visit, evt_proxy,
                                       NULL);
  if (evt_proxy->matched_subscribe_infos != NULL) {
    darray_destroy(evt_proxy->matched_subscribe_infos);
    evt_proxy->matched_subscribe_infos = NULL;
  }
  TK_OBJECT_UNREF(evt_proxy->subscribe_infos_group);
  TK_OBJECT_UNREF(evt_proxy->register_infos_group);

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

  ret->register_infos_group = object_hash_create_ex(FALSE);
  goto_error_if_fail(ret->register_infos_group != NULL);

  ret->subscribe_infos_group = object_hash_create_ex(FALSE);
  goto_error_if_fail(ret->subscribe_infos_group != NULL);

  return TK_OBJECT(ret);
error:
  tk_object_unref(TK_OBJECT(ret));
  return NULL;
}

/*********************************************************************************************/

static void object_evt_proxy_publish_topic_to_matched(object_evt_proxy_t* evt_proxy, event_t* e,
                                                      const char* topic) {
  darray_t* sub_infos =
      (darray_t*)tk_object_get_prop_pointer(evt_proxy->subscribe_infos_group, topic);
  if (sub_infos != NULL) {
    uint32_t i = 0;
    for (i = 0; i < sub_infos->size; i++) {
      object_evt_proxy_subscribe_info_t* sub_info =
          (object_evt_proxy_subscribe_info_t*)darray_get(sub_infos, i);
      int32_t exist_sub_info_index =
          darray_find_index_ex(evt_proxy->matched_subscribe_infos,
                               (tk_compare_t)object_evt_proxy_subscribe_info_cmp, sub_info);
      if (exist_sub_info_index >= 0) {
        object_evt_proxy_subscribe_info_t* exist_sub_info =
            (object_evt_proxy_subscribe_info_t*)darray_get(evt_proxy->matched_subscribe_infos,
                                                           exist_sub_info_index);
        if (object_evt_proxy_subscribe_info_cmp_by_priority(sub_info, exist_sub_info) < 0) {
          darray_replace(evt_proxy->matched_subscribe_infos, exist_sub_info_index, sub_info);
        }
      } else {
        darray_push(evt_proxy->matched_subscribe_infos, sub_info);
      }
    }
  }
}

typedef struct _object_evt_proxy_on_publish_on_visit_ctx_t {
  object_evt_proxy_t* evt_proxy;
  event_t* e;
} object_evt_proxy_on_publish_on_visit_ctx_t;

static ret_t object_evt_proxy_on_publish_on_visit(void* ctx, const void* data) {
  object_evt_proxy_on_publish_on_visit_ctx_t* actx =
      (object_evt_proxy_on_publish_on_visit_ctx_t*)(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  darray_t* infos = (darray_t*)value_pointer(&nv->value);
  uint32_t i = 0;
  for (i = 0; i < infos->size; i++) {
    const object_evt_proxy_register_info_t* info =
        (const object_evt_proxy_register_info_t*)darray_get(infos, i);
    if (info->evt_type == actx->e->type && info->publisher == actx->e->target) {
      if (NULL == info->opt.evt_filter ||
          info->opt.evt_filter(info->publisher, actx->e, info->opt.evt_filter_ctx)) {
        object_evt_proxy_publish_topic_to_matched(actx->evt_proxy, actx->e, nv->name);
        break;
      }
    }
  }

  return RET_OK;
}

inline static void object_evt_proxy_print_debug_log_when_stop_publish(
    object_evt_proxy_subscribe_info_t* sub_info) {
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) {
    return_if_fail(sub_info != NULL);
    if (sub_info->opt.subscriber != NULL) {
      if (TK_STR_IS_NOT_EMPTY(sub_info->opt.subscriber->name)) {
        log_debug("%s: stop publish subscriber is %p, name is \"%s\".\n", __FUNCTION__,
                  sub_info->opt.subscriber, sub_info->opt.subscriber->name);
      } else {
        log_debug("%s: stop publish subscriber is %p.\n", __FUNCTION__, sub_info->opt.subscriber);
      }
    } else {
      log_debug("%s: stop publish callback is %p.\n", __FUNCTION__, sub_info->callback);
    }
  }
}

static ret_t object_evt_proxy_on_publish(void* ctx, event_t* e) {
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY((tk_object_t*)ctx);
  object_evt_proxy_on_publish_on_visit_ctx_t actx = {
      .evt_proxy = evt_proxy,
      .e = e,
  };
  uint32_t i = 0;
  ret_t result = RET_OK;
  object_evt_proxy_subscribe_info_t* sub_info = NULL;
  return_value_if_fail(evt_proxy != NULL, RET_BAD_PARAMS);

  if (NULL == evt_proxy->matched_subscribe_infos) {
    evt_proxy->matched_subscribe_infos =
        darray_create(4, NULL, (tk_compare_t)object_evt_proxy_subscribe_info_cmp);
    return_value_if_fail(evt_proxy->matched_subscribe_infos != NULL, RET_OOM);
  }

  darray_clear(evt_proxy->matched_subscribe_infos);
  tk_object_foreach_prop(evt_proxy->register_infos_group, object_evt_proxy_on_publish_on_visit,
                         &actx);
  object_evt_proxy_publish_topic_to_matched(evt_proxy, e, "");
  darray_sort(evt_proxy->matched_subscribe_infos,
              (tk_compare_t)object_evt_proxy_subscribe_info_cmp_by_priority);

  evt_proxy->publishing = TRUE;
  for (i = 0; i < evt_proxy->matched_subscribe_infos->size; i++) {
    sub_info =
        (object_evt_proxy_subscribe_info_t*)darray_get(evt_proxy->matched_subscribe_infos, i);
    result = sub_info->callback(sub_info->opt.subscriber, e, sub_info->opt.callback_ctx);
    TK_FOREACH_VISIT_RESULT_PROCESSING(
        result,
        object_evt_proxy_infos_group_remove(evt_proxy->subscribe_infos_group, NULL, sub_info));
  }
  evt_proxy->publishing = FALSE;

  if (RET_OK != result) {
    object_evt_proxy_print_debug_log_when_stop_publish(sub_info);
  }

  return RET_OK;
}

ret_t object_evt_proxy_register(tk_object_t* obj, const char* topic, tk_object_t* publisher,
                                int32_t evt_type, const object_evt_proxy_register_opt_t* opt) {
  ret_t ret = RET_OK;
  darray_t* infos = NULL;
  object_evt_proxy_register_info_t* info = NULL;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && TK_STR_IS_NOT_EMPTY(topic) && publisher != NULL,
                       RET_BAD_PARAMS);

  infos = (darray_t*)tk_object_get_prop_pointer(evt_proxy->register_infos_group, topic);

  if (infos != NULL) {
    object_evt_proxy_register_info_t tmp = {
        .publisher = publisher,
        .evt_type = evt_type,
        .opt = opt != NULL ? *opt : (object_evt_proxy_register_opt_t){0},
    };
    return_value_if_fail(darray_find_index(infos, &tmp) < 0, RET_FAIL);
  } else {
    infos = object_evt_proxy_register_infos_create();
    return_value_if_fail(infos != NULL, RET_OOM);

    ret = tk_object_set_prop_pointer_ex(evt_proxy->register_infos_group, topic, infos,
                                        (tk_destroy_t)object_evt_proxy_register_infos_destroy);
    if (RET_OK != ret) {
      object_evt_proxy_register_infos_destroy(infos);
      return ret;
    }
  }

  info = object_evt_proxy_register_info_create(publisher, evt_type, opt);
  return_value_if_fail(info != NULL, RET_OOM);

  ret = darray_push(infos, info);
  if (RET_OK != ret) {
    object_evt_proxy_register_info_destroy(info);
    return ret;
  }

  if (!emitter_exist(EMITTER(publisher), evt_type, object_evt_proxy_on_publish, obj)) {
    if (TK_INVALID_ID ==
        emitter_on(EMITTER(publisher), evt_type, object_evt_proxy_on_publish, obj)) {
      darray_remove(infos, info);
      return RET_FAIL;
    }
  }

  return ret;
}

/*********************************************************************************************/

static int object_evt_proxy_unregister_cmp(const void* data, const void* ctx) {
  const object_evt_proxy_register_info_t* info = (const object_evt_proxy_register_info_t*)data;
  const object_evt_proxy_register_info_t* target = (const object_evt_proxy_register_info_t*)ctx;
  int ret = info->evt_type - target->evt_type;
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(info->publisher, target->publisher);
}

typedef struct _object_evt_proxy_unregister_group_cmp_ctx_t {
  object_evt_proxy_t* evt_proxy;
  tk_object_t* publisher;
  darray_t* infos;
  const object_evt_proxy_register_info_t* target;
} object_evt_proxy_unregister_group_cmp_ctx_t;

static int object_evt_proxy_unregister_group_cmp(const void* data, const void* ctx) {
  const named_value_t* nv = (const named_value_t*)data;
  darray_t* infos = (darray_t*)value_pointer(&nv->value);
  const object_evt_proxy_unregister_group_cmp_ctx_t* actx =
      (const object_evt_proxy_unregister_group_cmp_ctx_t*)ctx;
  if (infos == actx->infos) {
    return 1;
  }
  return (darray_find_index_ex(infos, object_evt_proxy_unregister_cmp, (void*)actx->target) >= 0)
             ? 0
             : -1;
}

static ret_t object_evt_proxy_unregister_on_visit(void* ctx, const void* data) {
  object_evt_proxy_unregister_group_cmp_ctx_t* actx =
      (object_evt_proxy_unregister_group_cmp_ctx_t*)ctx;
  const object_evt_proxy_register_info_t* info = (const object_evt_proxy_register_info_t*)(data);
  if (actx->publisher == info->publisher) {
    actx->target = info;
    if (NULL == tk_object_find_prop(actx->evt_proxy->register_infos_group,
                                    (tk_compare_t)object_evt_proxy_unregister_group_cmp, actx)) {
      emitter_off_by_func(EMITTER(info->publisher), info->evt_type, object_evt_proxy_on_publish,
                          actx->evt_proxy);
    }
    return RET_REMOVE;
  }
  return RET_OK;
}

ret_t object_evt_proxy_unregister(tk_object_t* obj, const char* topic, tk_object_t* publisher) {
  darray_t* infos = NULL;
  ret_t ret = RET_NOT_FOUND;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && TK_STR_IS_NOT_EMPTY(topic) && publisher != NULL,
                       RET_BAD_PARAMS);

  infos = (darray_t*)tk_object_get_prop_pointer(evt_proxy->register_infos_group, topic);

  if (infos != NULL) {
    object_evt_proxy_unregister_group_cmp_ctx_t ctx = {
        .evt_proxy = evt_proxy,
        .publisher = publisher,
        .infos = infos,
    };
    tk_object_ref(publisher);
    ret = darray_foreach(infos, object_evt_proxy_unregister_on_visit, &ctx);
    if (RET_OK == ret && infos->size == 0) {
      tk_object_remove_prop(evt_proxy->register_infos_group, topic);
    }
    tk_object_unref(publisher);
  }

  return ret;
}

/*********************************************************************************************/

ret_t object_evt_proxy_subscribe(tk_object_t* obj, const char* topic,
                                 object_evt_proxy_subscribe_callback_t callback,
                                 const object_evt_proxy_subscribe_opt_t* opt) {
  ret_t ret = RET_OK;
  darray_t* infos = NULL;
  object_evt_proxy_subscribe_info_t* info = NULL;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && callback != NULL, RET_BAD_PARAMS);
  if (NULL == topic) {
    topic = "";
  }

  infos = (darray_t*)tk_object_get_prop_pointer(evt_proxy->subscribe_infos_group, topic);

  if (infos != NULL) {
    object_evt_proxy_subscribe_info_t tmp = {
        .callback = callback,
        .opt = opt != NULL ? *opt : (object_evt_proxy_subscribe_opt_t){0},
    };
    return_value_if_fail(darray_find_index(infos, &tmp) < 0, RET_FAIL);
  } else {
    infos = object_evt_proxy_subscribe_infos_create();
    return_value_if_fail(infos != NULL, RET_OOM);

    ret = tk_object_set_prop_pointer_ex(evt_proxy->subscribe_infos_group, topic, infos,
                                        (tk_destroy_t)object_evt_proxy_subscribe_infos_destroy);
    if (RET_OK != ret) {
      object_evt_proxy_subscribe_infos_destroy(infos);
      return ret;
    }
  }

  info = object_evt_proxy_subscribe_info_create(callback, opt);
  return_value_if_fail(info != NULL, RET_OOM);

  ret = darray_push(infos, info);
  if (RET_OK != ret) {
    object_evt_proxy_subscribe_info_destroy(info);
    return ret;
  }

  return ret;
}

ret_t object_evt_proxy_unsubscribe(tk_object_t* obj, const char* topic,
                                   object_evt_proxy_subscribe_callback_t callback,
                                   const object_evt_proxy_subscribe_opt_t* opt) {
  ret_t ret = RET_NOT_FOUND;
  darray_t* infos = NULL;
  object_evt_proxy_t* evt_proxy = OBJECT_EVT_PROXY(obj);
  return_value_if_fail(evt_proxy != NULL && callback != NULL, RET_BAD_PARAMS);
  return_value_if_fail(!evt_proxy->publishing, RET_BUSY);
  if (NULL == topic) {
    topic = "";
  }

  infos = (darray_t*)tk_object_get_prop_pointer(evt_proxy->subscribe_infos_group, topic);
  if (infos != NULL) {
    object_evt_proxy_subscribe_info_t tmp = {
        .callback = callback,
        .opt = (opt != NULL) ? *opt : (object_evt_proxy_subscribe_opt_t){0},
    };
    ret = darray_remove(infos, &tmp);
    if (RET_OK == ret && infos->size == 0) {
      tk_object_remove_prop(evt_proxy->subscribe_infos_group, topic);
    }
  }

  return ret;
}

object_evt_proxy_t* object_evt_proxy_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_evt_proxy_vtable, NULL);
  return (object_evt_proxy_t*)obj;
}
