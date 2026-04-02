/**
 * File:   object_evt_router.c
 * Author: AWTK Develop Team
 * Brief:  对象事件路由器。
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

#include "object_evt_router.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_hash.h"
#include "tkc/darray.h"
#include "tkc/named_value.h"

typedef struct _object_evt_router_register_info_t {
  tk_object_t* publisher;
  int32_t evt_type;
  object_evt_router_register_opt_t opt;
} object_evt_router_register_info_t;

static ret_t object_evt_router_register_info_destroy(object_evt_router_register_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (info->publisher != NULL) {
    tk_object_unref_by_lifecycle(info->publisher, info->opt.publisher_lifecycle);
  }
  TKMEM_FREE(info);
  return RET_OK;
}

static object_evt_router_register_info_t* object_evt_router_register_info_create(
    tk_object_t* publisher, int32_t evt_type, const object_evt_router_register_opt_t* opt) {
  object_evt_router_register_info_t* ret = NULL;
  return_value_if_fail(publisher != NULL, NULL);

  ret = TKMEM_ZALLOC(object_evt_router_register_info_t);
  return_value_if_fail(ret != NULL, NULL);

  if (opt != NULL) {
    ret->opt = *opt;
  }

  ret->publisher = tk_object_ref_by_lifecycle(publisher, ret->opt.publisher_lifecycle);
  goto_error_if_fail(ret->publisher != NULL);

  ret->evt_type = evt_type;

  return ret;
error:
  object_evt_router_register_info_destroy(ret);
  return NULL;
}

inline static int object_evt_router_register_opt_cmp(const object_evt_router_register_opt_t* opt1,
                                                     const object_evt_router_register_opt_t* opt2) {
  int ret = pointer_compare(opt1->evt_filter, opt2->evt_filter);
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(opt1->evt_filter_ctx, opt2->evt_filter_ctx);
}

inline static int object_evt_router_register_info_cmp(
    const object_evt_router_register_info_t* info1,
    const object_evt_router_register_info_t* info2) {
  int ret = info1->evt_type - info2->evt_type;
  if (0 != ret) {
    return ret;
  }
  ret = pointer_compare(info1->publisher, info2->publisher);
  if (0 != ret) {
    return ret;
  }
  return object_evt_router_register_opt_cmp(&info1->opt, &info2->opt);
}

inline static ret_t object_evt_router_register_infos_destroy(darray_t* infos) {
  return_value_if_fail(infos != NULL, RET_BAD_PARAMS);
  return darray_destroy(infos);
}

inline static darray_t* object_evt_router_register_infos_create(void) {
  return darray_create(4, (tk_destroy_t)object_evt_router_register_info_destroy,
                       (tk_compare_t)object_evt_router_register_info_cmp);
}

#define OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_FORMAT(reg_info, FORMAT)                    \
  TK_STR_IS_NOT_EMPTY((reg_info)->publisher->name) ? "[publisher: \"%s\"] " FORMAT "\n" \
                                                   : "[publisher: %p] " FORMAT "\n"

#define OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_ARGS(reg_info)                                    \
  TK_STR_IS_NOT_EMPTY((reg_info)->publisher->name) ? (const char*)(reg_info)->publisher->name \
                                                   : (void*)(reg_info)->publisher

/*********************************************************************************************/

typedef struct _object_evt_router_subscribe_info_t {
  object_evt_router_subscribe_callback_t callback;
  object_evt_router_subscribe_opt_t opt;
  bool_t unsubscribed : 1;
} object_evt_router_subscribe_info_t;

static ret_t object_evt_router_subscribe_info_destroy(object_evt_router_subscribe_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  if (info->opt.subscriber != NULL) {
    tk_object_unref_by_lifecycle(info->opt.subscriber, info->opt.subscriber_lifecycle);
  }
  TKMEM_FREE(info);
  return RET_OK;
}

static object_evt_router_subscribe_info_t* object_evt_router_subscribe_info_create(
    object_evt_router_subscribe_callback_t callback, const object_evt_router_subscribe_opt_t* opt) {
  object_evt_router_subscribe_info_t* ret = NULL;
  return_value_if_fail(callback != NULL, NULL);

  ret = TKMEM_ZALLOC(object_evt_router_subscribe_info_t);
  return_value_if_fail(ret != NULL, NULL);

  ret->callback = callback;

  if (opt != NULL) {
    ret->opt = *opt;
    if (opt->subscriber != NULL) {
      ret->opt.subscriber = tk_object_ref_by_lifecycle(opt->subscriber, opt->subscriber_lifecycle);
    }
  }

  return ret;
}

inline static int object_evt_router_subscribe_opt_cmp(
    const object_evt_router_subscribe_opt_t* opt1, const object_evt_router_subscribe_opt_t* opt2) {
  int ret = pointer_compare(opt1->subscriber, opt2->subscriber);
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(opt1->callback_ctx, opt2->callback_ctx);
}

inline static int object_evt_router_subscribe_info_cmp(
    const object_evt_router_subscribe_info_t* info1,
    const object_evt_router_subscribe_info_t* info2) {
  int ret = pointer_compare(info1->callback, info2->callback);
  if (0 != ret) {
    return ret;
  }
  return object_evt_router_subscribe_opt_cmp(&info1->opt, &info2->opt);
}

inline static int object_evt_router_subscribe_info_cmp_by_priority(
    const object_evt_router_subscribe_info_t* info1,
    const object_evt_router_subscribe_info_t* info2) {
  if (info1->opt.priority < info2->opt.priority) {
    return 1;
  } else if (info1->opt.priority > info2->opt.priority) {
    return -1;
  } else {
    return 0;
  }
}

inline static ret_t object_evt_router_subscribe_infos_destroy(darray_t* infos) {
  return_value_if_fail(infos != NULL, RET_BAD_PARAMS);
  return darray_destroy(infos);
}

inline static darray_t* object_evt_router_subscribe_infos_create(void) {
  return darray_create(4, (tk_destroy_t)object_evt_router_subscribe_info_destroy,
                       (tk_compare_t)object_evt_router_subscribe_info_cmp);
}

#define OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_FORMAT(sub_info, FORMAT)                           \
  ((sub_info)->opt.subscriber != NULL) ? (TK_STR_IS_NOT_EMPTY((sub_info)->opt.subscriber->name) \
                                              ? "[subscriber: \"%s\"] " FORMAT "\n"             \
                                              : "[subscriber: %p] " FORMAT "\n")                \
                                       : "[subscriber callback: %p] " FORMAT "\n"

#define OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_ARGS(sub_info)                                     \
  ((sub_info)->opt.subscriber != NULL) ? (TK_STR_IS_NOT_EMPTY((sub_info)->opt.subscriber->name) \
                                              ? (const char*)(sub_info)->opt.subscriber->name   \
                                              : (void*)(sub_info)->opt.subscriber)              \
                                       : (void*)(sub_info)->callback

/*********************************************************************************************/

typedef struct _object_evt_router_infos_group_foreach_on_visit_ctx_t {
  tk_visit_t visit;
  void* ctx;
} object_evt_router_infos_group_foreach_on_visit_ctx_t;

static ret_t object_evt_router_infos_group_foreach_on_visit(void* ctx, const void* data) {
  object_evt_router_infos_group_foreach_on_visit_ctx_t* actx =
      (object_evt_router_infos_group_foreach_on_visit_ctx_t*)(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  darray_t* infos = (darray_t*)value_pointer(&nv->value);
  return darray_foreach(infos, actx->visit, actx->ctx);
}

inline static ret_t object_evt_router_infos_group_foreach(tk_object_t* infos_group,
                                                          tk_visit_t visit, void* ctx) {
  object_evt_router_infos_group_foreach_on_visit_ctx_t actx = {
      .visit = visit,
      .ctx = ctx,
  };
  return_value_if_fail(infos_group != NULL, RET_BAD_PARAMS);
  return tk_object_foreach_prop(infos_group, object_evt_router_infos_group_foreach_on_visit, &actx);
}

/*********************************************************************************************/

struct _object_evt_router_t {
  tk_object_t object;
  tk_object_t* register_infos_group;
  tk_object_t* subscribe_infos_group;
  /* cache */
  darray_t* matched_subscribe_infos;
  str_t* log;
  int8_t log_recursion_depth;
  bool_t publishing : 1;
};

static ret_t object_evt_router_dispatch_log(object_evt_router_t* evt_router, tk_log_level_t level,
                                            event_t* e, const char* format, ...) {
  char empty[1];
  int size = 0;
  ret_t ret = RET_OK;
  log_message_event_t evt;
  va_list args, args_copy;
  return_value_if_fail(evt_router != NULL && format != NULL, RET_BAD_PARAMS);

  /* 防止无限递归导致栈溢出 */
  if (evt_router->log_recursion_depth >= 2) {
    if (e != NULL) {
      if (EVT_LOG_MESSAGE == e->type && e->target == evt_router) {
        log_message_event_t* log_evt = log_message_event_cast(e);
        if (log_evt->level == level) {
          log_warn("%s: log recursion detected, skip log message: %s\n", __FUNCTION__,
                   log_evt->message);
          return RET_SKIP;
        }
      }
    }
  }

  if (NULL == evt_router->log) {
    evt_router->log = str_create(0);
    return_value_if_fail(evt_router->log != NULL, RET_OOM);
  }

  va_start(args, format);
  va_copy(args_copy, args);
  size = tk_vsnprintf(empty, sizeof(empty), format, args_copy);
  va_end(args_copy);

  str_clear(evt_router->log);
  ret = str_append_vformat(evt_router->log, size + 1, format, args);
  va_end(args);
  return_value_if_fail(RET_OK == ret, ret);

  evt_router->log_recursion_depth++;

  log_message_event_init(&evt, level, evt_router->log->str);
  ret = emitter_dispatch(EMITTER(evt_router), &evt.e);

  evt_router->log_recursion_depth--;

  return ret;
}

static ret_t object_evt_router_publisher_log_on_visit(void* ctx, const void* data) {
  const object_evt_router_register_info_t* info = (const object_evt_router_register_info_t*)(data);
  object_evt_router_t* evt_router = (object_evt_router_t*)(ctx);
  evt_router->register_infos_group->visiting = FALSE;
  object_evt_router_dispatch_log(
      evt_router, LOG_LEVEL_INFO, NULL,
      OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_FORMAT(info, "Not unregister on destroy."),
      OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_ARGS(info));
  evt_router->register_infos_group->visiting = TRUE;
  return RET_OK;
}

static ret_t object_evt_router_subscriber_log_on_visit(void* ctx, const void* data) {
  const object_evt_router_subscribe_info_t* info =
      (const object_evt_router_subscribe_info_t*)(data);
  object_evt_router_t* evt_router = (object_evt_router_t*)(ctx);
  evt_router->subscribe_infos_group->visiting = FALSE;
  object_evt_router_dispatch_log(
      evt_router, LOG_LEVEL_INFO, NULL,
      OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_FORMAT(info, "Not unsubscribe on destroy."),
      OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_ARGS(info));
  evt_router->subscribe_infos_group->visiting = TRUE;
  return RET_OK;
}

static ret_t object_evt_router_publisher_event_off_on_visit(void* ctx, const void* data) {
  const object_evt_router_register_info_t* info = (const object_evt_router_register_info_t*)(data);
  emitter_off_by_ctx(EMITTER(info->publisher), ctx);
  return RET_OK;
}

static ret_t object_evt_router_on_destroy(tk_object_t* obj) {
  object_evt_router_t* evt_router = OBJECT_EVT_ROUTER(obj);
  return_value_if_fail(evt_router != NULL, RET_BAD_PARAMS);

  object_evt_router_infos_group_foreach(evt_router->register_infos_group,
                                        object_evt_router_publisher_log_on_visit, evt_router);
  object_evt_router_infos_group_foreach(evt_router->subscribe_infos_group,
                                        object_evt_router_subscriber_log_on_visit, evt_router);

  object_evt_router_infos_group_foreach(evt_router->register_infos_group,
                                        object_evt_router_publisher_event_off_on_visit, evt_router);
  if (evt_router->log != NULL) {
    str_destroy(evt_router->log);
    evt_router->log = NULL;
  }
  if (evt_router->matched_subscribe_infos != NULL) {
    darray_destroy(evt_router->matched_subscribe_infos);
    evt_router->matched_subscribe_infos = NULL;
  }
  TK_OBJECT_UNREF(evt_router->subscribe_infos_group);
  TK_OBJECT_UNREF(evt_router->register_infos_group);

  return RET_OK;
}

static const object_vtable_t s_object_evt_router_vtable = {
    .type = OBJECT_EVT_ROUTER_TYPE,
    .desc = OBJECT_EVT_ROUTER_TYPE,
    .size = sizeof(object_evt_router_t),
    .on_destroy = object_evt_router_on_destroy,
    .compare = tk_object_compare_name_without_nullptr,
};

tk_object_t* object_evt_router_create(void) {
  object_evt_router_t* ret = (object_evt_router_t*)tk_object_create(&s_object_evt_router_vtable);
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

static ret_t object_evt_router_publish_topic_to_matched_on_visit(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  object_evt_router_subscribe_info_t* sub_info = (object_evt_router_subscribe_info_t*)(data);
  object_evt_router_t* evt_router = (object_evt_router_t*)(ctx);
  int32_t exist_sub_info_index = -1;
  if (sub_info->unsubscribed) {
    return RET_REMOVE;
  }
  exist_sub_info_index =
      darray_find_index_ex(evt_router->matched_subscribe_infos,
                           (tk_compare_t)object_evt_router_subscribe_info_cmp, sub_info);
  if (exist_sub_info_index >= 0) {
    object_evt_router_subscribe_info_t* exist_sub_info =
        (object_evt_router_subscribe_info_t*)darray_get(evt_router->matched_subscribe_infos,
                                                        exist_sub_info_index);
    if (object_evt_router_subscribe_info_cmp_by_priority(sub_info, exist_sub_info) < 0) {
      ret = darray_replace(evt_router->matched_subscribe_infos, exist_sub_info_index, sub_info);
    }
  } else {
    ret = darray_push(evt_router->matched_subscribe_infos, sub_info);
  }
  return ret;
}

inline static ret_t object_evt_router_publish_topic_to_matched(object_evt_router_t* evt_router,
                                                               const char* topic) {
  ret_t ret = RET_NOT_FOUND;
  darray_t* sub_infos =
      (darray_t*)tk_object_get_prop_pointer(evt_router->subscribe_infos_group, topic);
  if (sub_infos != NULL) {
    ret =
        darray_foreach(sub_infos, object_evt_router_publish_topic_to_matched_on_visit, evt_router);
    if (RET_OK == ret) {
      if (0 == sub_infos->size) {
        tk_object_remove_prop(evt_router->subscribe_infos_group, topic);
      }
    }
  }
  return ret;
}

typedef struct _object_evt_router_on_publish_on_visit_ctx_t {
  object_evt_router_t* evt_router;
  event_t* e;
} object_evt_router_on_publish_on_visit_ctx_t;

static ret_t object_evt_router_on_publish_on_visit(void* ctx, const void* data) {
  object_evt_router_on_publish_on_visit_ctx_t* actx =
      (object_evt_router_on_publish_on_visit_ctx_t*)(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  darray_t* infos = (darray_t*)value_pointer(&nv->value);
  uint32_t i = 0;
  for (i = 0; i < infos->size; i++) {
    const object_evt_router_register_info_t* info =
        (const object_evt_router_register_info_t*)darray_get(infos, i);
    if (info->evt_type == actx->e->type && info->publisher == actx->e->target) {
      if (NULL == info->opt.evt_filter ||
          info->opt.evt_filter(info->publisher, actx->e, info->opt.evt_filter_ctx)) {
        object_evt_router_publish_topic_to_matched(actx->evt_router, nv->name);
        break;
      }
    }
  }

  return RET_OK;
}

static ret_t object_evt_router_on_publish(void* ctx, event_t* e) {
  object_evt_router_t* evt_router = OBJECT_EVT_ROUTER((tk_object_t*)ctx);
  object_evt_router_on_publish_on_visit_ctx_t actx = {
      .evt_router = evt_router,
      .e = e,
  };
  uint32_t i = 0;
  ret_t result = RET_OK;
  object_evt_router_subscribe_info_t* sub_info = NULL;
  return_value_if_fail(evt_router != NULL, RET_BAD_PARAMS);

  if (NULL == evt_router->matched_subscribe_infos) {
    evt_router->matched_subscribe_infos =
        darray_create(4, NULL, (tk_compare_t)object_evt_router_subscribe_info_cmp);
    return_value_if_fail(evt_router->matched_subscribe_infos != NULL, RET_OOM);
  }

  darray_clear(evt_router->matched_subscribe_infos);
  tk_object_foreach_prop(evt_router->register_infos_group, object_evt_router_on_publish_on_visit,
                         &actx);
  object_evt_router_publish_topic_to_matched(evt_router, "");
  darray_sort(evt_router->matched_subscribe_infos,
              (tk_compare_t)object_evt_router_subscribe_info_cmp_by_priority);

  for (i = 0; i < evt_router->matched_subscribe_infos->size; i++) {
    sub_info =
        (object_evt_router_subscribe_info_t*)darray_get(evt_router->matched_subscribe_infos, i);
    evt_router->publishing = TRUE;
    result = sub_info->callback(sub_info->opt.subscriber, e, sub_info->opt.callback_ctx);
    evt_router->publishing = FALSE;
    TK_FOREACH_VISIT_RESULT_PROCESSING(
        result,
        (sub_info->unsubscribed = TRUE,
         object_evt_router_dispatch_log(
             evt_router, LOG_LEVEL_INFO, e,
             OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_FORMAT(sub_info, "Unsubscribe on publishing."),
             OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_ARGS(sub_info))));
  }

  if (RET_OK != result) {
    object_evt_router_dispatch_log(
        evt_router, LOG_LEVEL_INFO, e,
        OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_FORMAT(sub_info, "Publish stopped."),
        OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_ARGS(sub_info));
  }

  return RET_OK;
}

ret_t object_evt_router_register(tk_object_t* obj, const char* topic, tk_object_t* publisher,
                                 int32_t evt_type, const object_evt_router_register_opt_t* opt) {
  ret_t ret = RET_OK;
  darray_t* infos = NULL;
  object_evt_router_register_info_t* info = NULL;
  object_evt_router_t* evt_router = OBJECT_EVT_ROUTER(obj);
  return_value_if_fail(evt_router != NULL && TK_STR_IS_NOT_EMPTY(topic) && publisher != NULL,
                       RET_BAD_PARAMS);

  infos = (darray_t*)tk_object_get_prop_pointer(evt_router->register_infos_group, topic);

  if (infos != NULL) {
    object_evt_router_register_info_t tmp = {
        .publisher = publisher,
        .evt_type = evt_type,
        .opt = opt != NULL ? *opt : (object_evt_router_register_opt_t){0},
    };
    return_value_if_fail(darray_find_index(infos, &tmp) < 0, RET_FAIL);
  } else {
    infos = object_evt_router_register_infos_create();
    return_value_if_fail(infos != NULL, RET_OOM);

    ret = tk_object_set_prop_pointer_ex(evt_router->register_infos_group, topic, infos,
                                        (tk_destroy_t)object_evt_router_register_infos_destroy);
    if (RET_OK != ret) {
      object_evt_router_register_infos_destroy(infos);
      return ret;
    }
  }

  info = object_evt_router_register_info_create(publisher, evt_type, opt);
  return_value_if_fail(info != NULL, RET_OOM);

  ret = darray_push(infos, info);
  if (RET_OK != ret) {
    object_evt_router_register_info_destroy(info);
    return ret;
  }

  if (!emitter_exist(EMITTER(publisher), evt_type, object_evt_router_on_publish, obj)) {
    if (TK_INVALID_ID ==
        emitter_on(EMITTER(publisher), evt_type, object_evt_router_on_publish, obj)) {
      darray_remove(infos, info);
      return RET_FAIL;
    }
  }

  object_evt_router_dispatch_log(evt_router, LOG_LEVEL_INFO, NULL,
                                 OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_FORMAT(
                                     info, "Register topic: \"%s\", evt_type: %d, filter: %p."),
                                 OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_ARGS(info), topic,
                                 info->evt_type, info->opt.evt_filter);

  return ret;
}

/*********************************************************************************************/

static int object_evt_router_unregister_cmp(const void* data, const void* ctx) {
  const object_evt_router_register_info_t* info = (const object_evt_router_register_info_t*)data;
  const object_evt_router_register_info_t* target = (const object_evt_router_register_info_t*)ctx;
  int ret = info->evt_type - target->evt_type;
  if (0 != ret) {
    return ret;
  }
  return pointer_compare(info->publisher, target->publisher);
}

typedef struct _object_evt_router_unregister_group_cmp_ctx_t {
  object_evt_router_t* evt_router;
  tk_object_t* publisher;
  darray_t* infos;
  const object_evt_router_register_info_t* target;
} object_evt_router_unregister_group_cmp_ctx_t;

static int object_evt_router_unregister_group_cmp(const void* data, const void* ctx) {
  const named_value_t* nv = (const named_value_t*)data;
  darray_t* infos = (darray_t*)value_pointer(&nv->value);
  const object_evt_router_unregister_group_cmp_ctx_t* actx =
      (const object_evt_router_unregister_group_cmp_ctx_t*)ctx;
  if (infos == actx->infos) {
    return 1;
  }
  return (darray_find_index_ex(infos, object_evt_router_unregister_cmp, (void*)actx->target) >= 0)
             ? 0
             : -1;
}

static ret_t object_evt_router_unregister_on_visit(void* ctx, const void* data) {
  object_evt_router_unregister_group_cmp_ctx_t* actx =
      (object_evt_router_unregister_group_cmp_ctx_t*)ctx;
  const object_evt_router_register_info_t* info = (const object_evt_router_register_info_t*)(data);
  if (actx->publisher == info->publisher) {
    actx->target = info;
    if (NULL == tk_object_find_prop(actx->evt_router->register_infos_group,
                                    (tk_compare_t)object_evt_router_unregister_group_cmp, actx)) {
      emitter_off_by_func(EMITTER(info->publisher), info->evt_type, object_evt_router_on_publish,
                          actx->evt_router);
    }
    actx->target = NULL;
    return RET_REMOVE;
  }
  return RET_OK;
}

ret_t object_evt_router_unregister(tk_object_t* obj, const char* topic, tk_object_t* publisher) {
  darray_t* infos = NULL;
  ret_t ret = RET_NOT_FOUND;
  object_evt_router_t* evt_router = OBJECT_EVT_ROUTER(obj);
  return_value_if_fail(evt_router != NULL && TK_STR_IS_NOT_EMPTY(topic) && publisher != NULL,
                       RET_BAD_PARAMS);

  infos = (darray_t*)tk_object_get_prop_pointer(evt_router->register_infos_group, topic);

  if (infos != NULL) {
    object_evt_router_unregister_group_cmp_ctx_t ctx = {
        .evt_router = evt_router,
        .publisher = publisher,
        .infos = infos,
    };
    tk_object_ref(publisher);
    ret = darray_foreach(infos, object_evt_router_unregister_on_visit, &ctx);
    if (RET_OK == ret) {
      object_evt_router_register_info_t tmp = {
          .publisher = publisher,
      };
      object_evt_router_dispatch_log(
          evt_router, LOG_LEVEL_INFO, NULL,
          OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_FORMAT(&tmp, "Unregister topic: \"%s\"."),
          OBJECT_EVT_ROUTER_LOG_REGISTER_INFO_ARGS(&tmp), topic);

      if (0 == infos->size) {
        tk_object_remove_prop(evt_router->register_infos_group, topic);
      }
    }
    tk_object_unref(publisher);
  }

  return ret;
}

/*********************************************************************************************/

ret_t object_evt_router_subscribe(tk_object_t* obj, const char* topic,
                                  object_evt_router_subscribe_callback_t callback,
                                  const object_evt_router_subscribe_opt_t* opt) {
  ret_t ret = RET_OK;
  darray_t* infos = NULL;
  object_evt_router_subscribe_info_t* info = NULL;
  object_evt_router_t* evt_router = OBJECT_EVT_ROUTER(obj);
  return_value_if_fail(evt_router != NULL && callback != NULL, RET_BAD_PARAMS);
  if (NULL == topic) {
    topic = "";
  }

  infos = (darray_t*)tk_object_get_prop_pointer(evt_router->subscribe_infos_group, topic);

  if (infos != NULL) {
    object_evt_router_subscribe_info_t tmp = {
        .callback = callback,
        .opt = opt != NULL ? *opt : (object_evt_router_subscribe_opt_t){0},
    };
    return_value_if_fail(darray_find_index(infos, &tmp) < 0, RET_FAIL);
  } else {
    infos = object_evt_router_subscribe_infos_create();
    return_value_if_fail(infos != NULL, RET_OOM);

    ret = tk_object_set_prop_pointer_ex(evt_router->subscribe_infos_group, topic, infos,
                                        (tk_destroy_t)object_evt_router_subscribe_infos_destroy);
    if (RET_OK != ret) {
      object_evt_router_subscribe_infos_destroy(infos);
      return ret;
    }
  }

  info = object_evt_router_subscribe_info_create(callback, opt);
  return_value_if_fail(info != NULL, RET_OOM);

  ret = darray_push(infos, info);
  if (RET_OK != ret) {
    object_evt_router_subscribe_info_destroy(info);
    return ret;
  }

  object_evt_router_dispatch_log(
      evt_router, LOG_LEVEL_INFO, NULL,
      OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_FORMAT(info, "Subscribe topic: \"%s\", priority: %d."),
      OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_ARGS(info), topic, info->opt.priority);

  return ret;
}

ret_t object_evt_router_unsubscribe(tk_object_t* obj, const char* topic,
                                    object_evt_router_subscribe_callback_t callback,
                                    const object_evt_router_subscribe_opt_t* opt) {
  ret_t ret = RET_NOT_FOUND;
  darray_t* infos = NULL;
  object_evt_router_t* evt_router = OBJECT_EVT_ROUTER(obj);
  return_value_if_fail(evt_router != NULL && callback != NULL, RET_BAD_PARAMS);
  if (NULL == topic) {
    topic = "";
  }

  infos = (darray_t*)tk_object_get_prop_pointer(evt_router->subscribe_infos_group, topic);
  if (infos != NULL) {
    object_evt_router_subscribe_info_t tmp = {
        .callback = callback,
        .opt = (opt != NULL) ? *opt : (object_evt_router_subscribe_opt_t){0},
    };
    if (evt_router->publishing) {
      object_evt_router_subscribe_info_t* info =
          (object_evt_router_subscribe_info_t*)darray_find(infos, &tmp);
      if (info != NULL) {
        info->unsubscribed = TRUE;
        ret = RET_OK;
      }
    } else {
      ret = darray_remove(infos, &tmp);
      if (RET_OK == ret) {
        if (0 == infos->size) {
          tk_object_remove_prop(evt_router->subscribe_infos_group, topic);
        }
      }
    }
  }

  if (RET_OK == ret) {
    object_evt_router_subscribe_info_t tmp = {
        .callback = callback,
        .opt = (opt != NULL) ? *opt : (object_evt_router_subscribe_opt_t){0},
    };
    object_evt_router_dispatch_log(
        evt_router, LOG_LEVEL_INFO, NULL,
        OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_FORMAT(&tmp, "Unsubscribe topic: \"%s\"."),
        OBJECT_EVT_ROUTER_LOG_SUBSCRIBE_INFO_ARGS(&tmp), topic);
  }

  return ret;
}

object_evt_router_t* object_evt_router_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_evt_router_vtable, NULL);
  return (object_evt_router_t*)obj;
}
