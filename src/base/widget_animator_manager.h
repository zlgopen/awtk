/**
 * File:   widget_animator_manager.h
 * Author: AWTK Develop Team
 * Brief:  animator manager
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
 * 2018-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ANIMATOR_MANAGER_H
#define TK_ANIMATOR_MANAGER_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_manager_t
 * 控件动画管理器。
 */
struct _widget_animator_manager_t {
  /**
   * @property {float_t} time_scale
   * 时间倍率，可以让时间变快或变慢。
   */
  float_t time_scale;

  /*private*/
  uint32_t timer_id;
  uint32_t last_dispatch_time;
  widget_animator_t* first;
};

/**
 * @method widget_animator_manager
 * 获取缺省的控件动画管理器。
 * @alias widget_animator_manager_instance
 * @annotation ["constructor"]
 * @return {widget_animator_manager_t*} 返回控件动画管理器。
 */
widget_animator_manager_t* widget_animator_manager(void);

/**
 * @method widget_animator_manager_set
 * 设置缺省的控件动画管理器。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_set(widget_animator_manager_t* am);

/**
 * @method widget_animator_manager_create
 * 创建控件动画管理器。
 * @annotation ["constructor"]
 *
 * @return {widget_animator_manager_t*} 返回控件动画管理器。
 */
widget_animator_manager_t* widget_animator_manager_create(void);

/**
 * @method widget_animator_manager_init
 * 初始化控件动画管理器。
 * @annotation ["constructor"]
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 *
 * @return {widget_animator_manager_t*} 返回控件动画管理器。
 */
widget_animator_manager_t* widget_animator_manager_init(widget_animator_manager_t* am);

/**
 * @method widget_animator_manager_deinit
 * 析构控件动画管理器。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_deinit(widget_animator_manager_t* am);

/**
 * @method widget_animator_manager_destroy
 * 析构并释放控件动画管理器。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_destroy(widget_animator_manager_t* am);

/**
 * @method widget_animator_manager_add
 * 添加指定的动画，动画在创建时调用本函数。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_animator_t*} animator 动画对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_add(widget_animator_manager_t* am, widget_animator_t* animator);

/**
 * @method widget_animator_manager_remove
 * 移出指定的动画，动画在销毁时调用本函数。。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_animator_t*} animator 动画对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_remove(widget_animator_manager_t* am, widget_animator_t* animator);

/**
 * @method widget_animator_manager_count
 * 获取动画个数。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 *
 * @return {ret_t} 返回动画个数。
 */
uint32_t widget_animator_manager_count(widget_animator_manager_t* am);

/**
 * @method widget_animator_manager_start
 * 播放满足条件的动画。
 * 1.widget为NULL时，播放所有名称为name的动画。
 * 2.name为NULL时，播放所有widget相关的动画。
 * 3.widget和name均为NULL，播放所有动画。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_start(widget_animator_manager_t* am, widget_t* widget,
                                    const char* name);
/**
 * @method widget_animator_manager_stop
 * 停止满足条件的动画。
 * 1.widget为NULL时，停止所有名称为name的动画。
 * 2.name为NULL时，停止所有widget相关的动画。
 * 3.widget和name均为NULL，停止所有动画。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_stop(widget_animator_manager_t* am, widget_t* widget,
                                   const char* name);
/**
 * @method widget_animator_manager_pause
 * 暂停满足条件的动画。
 * 1.widget为NULL时，暂停所有名称为name的动画。
 * 2.name为NULL时，暂停所有widget相关的动画。
 * 3.widget和name均为NULL，暂停所有动画。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_pause(widget_animator_manager_t* am, widget_t* widget,
                                    const char* name);

/**
 * @method widget_animator_manager_find
 * 查找满足条件的动画。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_animator_manager_find(widget_animator_manager_t* am, widget_t* widget,
                                                const char* name);
/**
 * @method widget_animator_manager_remove_all
 * 销毁满足条件的动画。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_remove_all(widget_animator_manager_t* am, widget_t* widget,
                                         const char* name);
/**
 * @method widget_animator_manager_set_time_scale
 * 设置时间倍率。
 * 设置动画的时间倍率，<0: 时间倒退，<1: 时间变慢，>1 时间变快。
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 * @param {float_t} time_scale 时间倍率。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_set_time_scale(widget_animator_manager_t* am, widget_t* widget,
                                             const char* name, float_t time_scale);

/**
 * @method widget_animator_manager_time_elapse
 * 时间流失，调用本函数更新动画(public for test)。
 *
 * @param {widget_animator_manager_t*} am 控件动画管理器。
 * @param {uint32_t} delta_time 流失的时间(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_manager_time_elapse(widget_animator_manager_t* am, uint32_t delta_time);

END_C_DECLS

#endif /*TK_ANIMATOR_MANAGER_H*/
