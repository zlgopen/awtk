/**
 * File:   widget_animator.h
 * Author: AWTK Develop Team
 * Brief:  widget animator interface
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
 * 2018-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_H
#define TK_WIDGET_ANIMATOR_H

#include "base/easing.h"
#include "base/widget.h"
#include "base/emitter.h"

BEGIN_C_DECLS

struct _widget_animator_t;
typedef struct _widget_animator_t widget_animator_t;

typedef ret_t (*widget_animator_update_t)(widget_animator_t* animator, float_t percent);
typedef ret_t (*widget_animator_destroy_t)(widget_animator_t* animator);

/**
 * @class widget_animator_t
 * 控件动画接口。
 */
struct _widget_animator_t {
  /**
   * @property {widget_t*} widget
   * @annotation ["private"]
   * 控件对象。
   */
  widget_t* widget;

  /**
   * @property {bool_t} reversed
   * @annotation ["private"]
   * 逆向运行，在yoyo模式下自动设置。
   */
  bool_t reversed;

  /**
   * @property {uint32_t} yoyo_times
   * @annotation ["private"]
   * yoyo剩余次数。
   */
  uint32_t yoyo_times;

  /**
   * @property {uint32_t} repeat_times
   * @annotation ["private"]
   * 重复剩余次数。
   */
  uint32_t repeat_times;

  /**
   * @property {uint32_t} now
   * @annotation ["private"]
   * 当前时间(毫秒)。
   */
  uint32_t now;

  /**
   * @property {uint32_t} start_time
   * @annotation ["private"]
   * 起始时间(毫秒)。
   */
  uint32_t start_time;

  /**
   * @property {uint32_t} delay
   * @annotation ["private"]
   * 延迟执行时间(毫秒)。
   */
  uint32_t delay;

  /**
   * @property {uint32_t} duration
   * @annotation ["private"]
   * 单次动画执行时间(毫秒)。
   */
  uint32_t duration;

  /**
   * @property {uint32_t} emitter
   * @annotation ["private"]
   * emitter
   */
  emitter_t emitter;

  /**
   * @property {uint32_t} timer_id
   * @annotation ["private"]
   * 定时器ID。
   */
  uint32_t timer_id;

  /**
   * @property {uint32_t} widget_destroy_id
   * @annotation ["private"]
   * widget_destroy_id。
   */
  uint32_t widget_destroy_id;

  /**
   * @property {easing_func_t} easing
   * @annotation ["private"]
   * 插值函数。
   */
  easing_func_t easing;

  /**
   * @property {widget_animator_update_t} update
   * @annotation ["private"]
   * update函数，子类需要实现。
   */
  widget_animator_update_t update;

  /**
   * @property {widget_animator_destroy_t} destroy
   * @annotation ["private"]
   * destroy函数，子类需要实现。
   */
  widget_animator_destroy_t destroy;
};

/**
 * @method widget_animator_init
 * 初始化。仅供子类内部使用。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} duration 动画持续时间。
 * @param {uint32_t} delay 动画执行时间。
 * @param {easing_func_t} 插值函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_init(widget_animator_t* animator, widget_t* widget, uint32_t duration,
                           uint32_t delay, easing_func_t easing);

/**
 * @method widget_animator_start
 * 启动动画。
 * @param {widget_animator_t*} animator 动画对象本身。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_start(widget_animator_t* animator);

/**
 * @method widget_animator_set_yoyo
 * 设置为yoyo模式。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {uint32_t} yoyo_times yoyo的次数，往返视为两次。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_set_yoyo(widget_animator_t* animator, uint32_t yoyo_times);

/**
 * @method widget_animator_set_repeat
 * 设置为重复模式。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {uint32_t} repeat_times 重复的次数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_set_repeat(widget_animator_t* animator, uint32_t repeat_times);

/**
 * @method widget_animator_set_reversed
 * 设置为逆向模式。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {bool_t} value 是否为逆向模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_set_reversed(widget_animator_t* animator, bool_t value);

/**
 * @method widget_animator_on
 * 注册指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {event_type_t} type
 * 事件类型。目前支持：EVT_ANIM_START,EVT_ANIM_STOP,EVT_ANIM_ONCE和EVT_ANIM_END。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于widget_animator_off。
 */
uint32_t widget_animator_on(widget_animator_t* animator, event_type_t type, event_func_t on_event,
                            void* ctx);

/**
 * @method widget_animator_off
 * 注销指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {uint32_t} id widget_animator_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_off(widget_animator_t* animator, uint32_t id);

/**
 * @method widget_animator_destroy
 * 销毁animator对象。
 * @param {widget_animator_t*} animator 动画对象本身。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_destroy(widget_animator_t* animator);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_H*/
