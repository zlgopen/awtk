/**
 * File:   object_evt_router.h
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

#ifndef TK_OBJECT_EVT_ROUTER_H
#define TK_OBJECT_EVT_ROUTER_H

#include "tkc/object.h"

BEGIN_C_DECLS

typedef bool_t (*object_evt_router_register_filter_t)(tk_object_t* publisher, event_t* e,
                                                      void* ctx);
typedef ret_t (*object_evt_router_subscribe_callback_t)(tk_object_t* subscriber, event_t* e,
                                                        void* ctx);

/**
 * @typedef object_evt_router_register_opt_t
 * 事件路由器注册选项。
 */
typedef struct _object_evt_router_register_opt_t {
  /**
   * @property {object_evt_router_register_filter_t} evt_filter
   * 事件过滤器。
   */
  object_evt_router_register_filter_t evt_filter;
  /**
   * @property {void*} evt_filter_ctx
   * 事件过滤器上下文。
   */
  void* evt_filter_ctx;
} object_evt_router_register_opt_t;

/**
 * @typedef object_evt_router_subscribe_opt_t
 * 事件路由器订阅选项。
 */
typedef struct _object_evt_router_subscribe_opt_t {
  /**
   * @property {tk_object_t*} subscriber
   * 订阅者。
   */
  tk_object_t* subscriber;
  /**
   * @property {void*} callback_ctx
   * 回调上下文。
   */
  void* callback_ctx;
  /**
   * @property {int16_t} priority
   * 优先级。（值越大，优先级越高）。
   */
  int16_t priority;
} object_evt_router_subscribe_opt_t;

/**
 * @class object_evt_router_t
 * @parent tk_object_t
 * 事件路由器对象。
 */
typedef struct _object_evt_router_t object_evt_router_t;

/**
 * @method object_evt_router_create
 * 创建事件路由器对象。
 * @annotation ["constructor", "gc"]
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* object_evt_router_create(void);

/**
 * @method object_evt_router_register
 * 注册。
 *
 * @param {tk_object_t*} obj object_evt_router对象。
 * @param {const char*} topic 主题。
 * @param {tk_object_t*} publisher 发布者。
 * @param {int32_t} evt_type 事件类型。
 * @param {const object_evt_router_register_opt_t*} opt 事件路由器注册选项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_evt_router_register(tk_object_t* obj, const char* topic, tk_object_t* publisher,
                                 int32_t evt_type, const object_evt_router_register_opt_t* opt);

/**
 * @method object_evt_router_unregister
 * 注销。
 *
 * @param {tk_object_t*} obj object_evt_router对象。
 * @param {const char*} topic 主题。
 * @param {tk_object_t*} publisher 发布者。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_evt_router_unregister(tk_object_t* obj, const char* topic, tk_object_t* publisher);

/**
 * @method object_evt_router_subscribe
 * 订阅。
 *
 * @param {tk_object_t*} obj object_evt_router对象。
 * @param {const char*} topic 主题（为NULL或空字符串时表示订阅所有主题）。
 * @param {object_evt_router_subscribe_callback_t} callback 事件路由器回调函数。
 * @param {const object_evt_router_subscribe_opt_t*} opt 事件路由器订阅选项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_evt_router_subscribe(tk_object_t* obj, const char* topic,
                                  object_evt_router_subscribe_callback_t callback,
                                  const object_evt_router_subscribe_opt_t* opt);

/**
 * @method object_evt_router_unsubscribe
 * 退订。
 *
 * @param {tk_object_t*} obj object_evt_router对象。
 * @param {const char*} topic 主题（为NULL或空字符串时表示订阅所有主题）。
 * @param {object_evt_router_subscribe_callback_t} callback 事件路由器回调函数。
 * @param {const object_evt_router_subscribe_opt_t*} opt 事件路由器订阅选项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_evt_router_unsubscribe(tk_object_t* obj, const char* topic,
                                    object_evt_router_subscribe_callback_t callback,
                                    const object_evt_router_subscribe_opt_t* opt);

/**
 * @method object_evt_router_cast
 * 转换为object_evt_router对象。
 * @annotation ["cast"]
 *
 * @param {tk_object_t*} obj object_evt_router对象。
 *
 * @return {object_evt_router_t*} object_evt_router对象。
 */
object_evt_router_t* object_evt_router_cast(tk_object_t* obj);

#define OBJECT_EVT_ROUTER(obj) object_evt_router_cast(obj)

#define OBJECT_EVT_ROUTER_TYPE "object_evt_router"

END_C_DECLS

#endif /*TK_OBJECT_EVT_ROUTER_H*/
