/**
 * File:   emitter.h
 * Author: AWTK Develop Team
 * Brief:  emitter dispatcher
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EMITTER_H
#define TK_EMITTER_H

#include "tkc/event.h"

BEGIN_C_DECLS

struct _emitter_item_t;
typedef struct _emitter_item_t emitter_item_t;

struct _emitter_item_t {
  void* ctx;
  uint32_t id;
  uint32_t type;
  event_func_t handler;

  uint32_t tag;
  tk_destroy_t on_destroy;
  void* on_destroy_ctx;
  emitter_item_t* next;
};

/**
 * @class emitter_t
 * @order -10
 * @annotation ["scriptable"]
 *
 * 事件分发器, 用于实现观察者模式。
 */
typedef struct _emitter_t {
  /**
   * @property {emitter_item_t*} items
   * @annotation ["private"]
   * 注册的回调函数集合。
   */
  emitter_item_t* items;
  /**
   * @property {uint32_t} next_id
   * @annotation ["private"]
   * 下一次emitter_on返回的ID，emitter_on成功后会自动增加next_id。
   */
  uint32_t next_id;
  /**
   * @property {bool_t} enable
   * @annotation ["readable"]
   * 禁用标志。禁用时dispatch无效。
   */
  bool_t enable;
  /**
   * @property {bool_t} remove_curr_iter
   * @annotation ["private"]
   * 如果在回调函数中，emitter_off当前正在dispatch的回调函数，
   * 我们只是设置remove_curr_iter为TRUE，在分发完成后才执行。
   * XXX: 如果要注销当前正在dispatch的回调函数，直接返回RET_REMOVE是最好的选择。
   */
  bool_t remove_curr_iter;
  /**
   * @property {emitter_item_t*} curr_iter
   * @annotation ["private"]
   * 当前正在dispatch的项。
   */
  emitter_item_t* curr_iter;
} emitter_t;

/**
 * @method emitter_create
 * @annotation ["constructor", "scriptable", "gc"]
 * 创建emitter对象。
 *
 * @return {emitter_t*} 对象。
 */
emitter_t* emitter_create(void);

/**
 * @method emitter_init
 * @annotation ["constructor"]
 * 初始化emitter对象。
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {emitter_t*} 对象。
 */
emitter_t* emitter_init(emitter_t* emitter);

/**
 * @method emitter_dispatch
 * 分发事件。如果当前分发的回调函数返回RET_REMOVE，该回调函数将被移出。
 * 禁用状态下，本函数不做任何事情。
 * @annotation ["scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 * @param {event_t*} e 事件对象。
 * @return {ret_t} 如果当前分发的回调函数返回RET_STOP，dispatch中断分发，并返回RET_STOP，否则返回RET_OK。
 */
ret_t emitter_dispatch(emitter_t* emitter, event_t* e);

/**
 * @method emitter_dispatch_simple_event
 * 分发事件。
 * > 对emitter_dispatch的包装，分发一个简单的事件。
 * @annotation ["scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 * @param {event_type_t} type 事件类型。
 * @return {ret_t}
 *  如果当前分发的回调函数返回RET_STOP，dispatch中断分发，并返回RET_STOP，否则返回RET_OK。
 */
ret_t emitter_dispatch_simple_event(emitter_t* emitter, uint32_t type);

/**
 * @method emitter_on
 * 注册指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {emitter_t*} emitter emitter对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于emitter_off。
 */
uint32_t emitter_on(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx);

/**
 * @method emitter_exist
 * 判断指定的事件和回调函数是否已经注册。
 * @param {emitter_t*} emitter emitter对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {bool_t} 返回TRUE表示已经注册，否则表示没有注册。
 */
bool_t emitter_exist(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx);

/**
 * @method emitter_on_with_tag
 * 注册指定事件的处理函数。
 * @param {emitter_t*} emitter emitter对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 * @param {uint32_t} tag tag。
 *
 * @return {uint32_t} 返回id，用于emitter_off。
 */
uint32_t emitter_on_with_tag(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx,
                             uint32_t tag);

/**
 * @method emitter_off
 * 注销指定事件的处理函数。
 * @annotation ["scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 * @param {uint32_t} id emitter_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_off(emitter_t* emitter, uint32_t id);

/**
 * @method emitter_off_by_func
 * 注销指定事件的处理函数。
 * @param {emitter_t*} emitter emitter对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_off_by_func(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx);

/**
 * @method emitter_off_by_ctx
 * 注销指定事件的处理函数。
 * @param {emitter_t*} emitter emitter对象。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_off_by_ctx(emitter_t* emitter, void* ctx);

/**
 * @method emitter_off_by_tag
 * 注销指定事件的处理函数。
 * @param {emitter_t*} emitter emitter对象。
 * @param {uint32_t} tag tag。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_off_by_tag(emitter_t* emitter, uint32_t tag);

/**
 * @method emitter_set_on_destroy
 * 设置一个回调函数，在emitter被销毁时调用(方便脚本语言去释放回调函数)。
 * @param {emitter_t*} emitter emitter对象。
 * @param {uint32_t} id emitter_on返回的ID。
 * @param {tk_destroy_t} on_destroy 回调函数。
 * @param {void*} on_destroy_ctx 回调函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_set_on_destroy(emitter_t* emitter, uint32_t id, tk_destroy_t on_destroy,
                             void* on_destroy_ctx);

/**
 * @method emitter_find
 * 通过ID查找emitter_item_t，主要用于辅助测试。
 * @param {emitter_t*} emitter emitter对象。
 * @param {uint32_t} id emitter_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
emitter_item_t* emitter_find(emitter_t* emitter, uint32_t id);

/**
 * @method emitter_enable
 * 启用。
 * @annotation ["scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_enable(emitter_t* emitter);

/**
 * @method emitter_disable
 * 禁用。
 *
 * 禁用后emitter_dispatch无效，但可以注册和注销。
 *
 * @annotation ["scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_disable(emitter_t* emitter);

/**
 * @method emitter_size
 * 获取注册的回调函数个数，主要用于辅助测试。
 * @annotation ["scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {uint32_t} 回调函数个数。
 */
uint32_t emitter_size(emitter_t* emitter);

/**
 * @method emitter_deinit
 * 析构。
 *
 * @annotation ["deconstructor"]
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_deinit(emitter_t* emitter);

/**
 * @method emitter_destroy
 * 销毁。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_destroy(emitter_t* emitter);

/**
 * @method emitter_cast
 * 转换为emitter对象(供脚本语言使用)。
 *
 * 主要给脚本语言使用。
 * @annotation ["cast", "scriptable"]
 * @param {emitter_t*} emitter emitter对象。
 *
 * @return {emitter_t*} 对象。
 */
emitter_t* emitter_cast(emitter_t* emitter);

/**
 * @method emitter_forward
 * 分发事件
 *
 * @param {void*} ctx emitter对象。
 * @param {event_t*} e 分发的事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t emitter_forward(void* ctx, event_t* e);

#define EMITTER(emitter) ((emitter_t*)(emitter))

/*public for test*/
ret_t emitter_remove_item(emitter_t* emitter, emitter_item_t* item);
emitter_item_t* emitter_get_item(emitter_t* emitter, uint32_t index);

END_C_DECLS

#endif /*TK_EMITTER_H*/
