/**
 * File:   image_animation.h
 * Author: AWTK Develop Team
 * Brief:  image_animation
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-09-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_ANIMATION_H
#define TK_IMAGE_ANIMATION_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class image_animation_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 图片动画控件，指定一个图片前缀，依次显示指定序列的图片，从而形成动画效果。
 */
typedef struct _image_animation_t {
  widget_t widget;

  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片名称的前缀。
   */
  char* image;
  /**
   * @property {char*} sequence
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 播放的序列，字符可选值为:0-9,a-z,A-Z。
   */
  char* sequence;
  /**
   * @property {bool_t} loop
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否循环播放。
   */
  bool_t loop;
  /**
   * @property {bool_t} auto_play
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否自动播放。
   */
  bool_t auto_play;
  /**
   * @property {uint32_t} interval
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 每张图片播放的时间(毫秒)。
   */
  uint32_t interval;
  /**
   * @property {uint32_t} delay
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 自动播放时延迟播放的时间(毫秒)。
   */
  uint32_t delay;

  /*private*/
  int32_t index;
  uint32_t timer_id;
} image_animation_t;

/**
 * @method image_animation_create
 * @annotation ["constructor", "scriptable"]
 * 创建image_animation对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* image_animation_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method image_animation_set_loop
 * 设置是否循环播放。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {bool_t} loop 是否循环播放。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_loop(widget_t* widget, bool_t loop);

/**
 * @method image_animation_set_image
 * 设置图片前缀。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {const char*} image 图片前缀。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_image(widget_t* widget, const char* image);

/**
 * @method image_animation_set_interval
 * 设置播放间隔时间。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {uint32_t} interval 间隔时间(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_interval(widget_t* widget, uint32_t interval);

/**
 * @method image_animation_set_delay
 * 设置延迟播放时间(仅适用于自动播放)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {uint32_t} delay 延迟播放时间(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_delay(widget_t* widget, uint32_t delay);

/**
 * @method image_animation_set_auto_play
 * 设置是否自动播放。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {bool_t} auto_play 是否自动播放。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_auto_play(widget_t* widget, bool_t auto_play);

/**
 * @method image_animation_set_sequence
 * 设置播放序列。比如image为"fire"，sequence为"123", 将依次播放"fire1", "fire2", "fire3"。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {const char*} sequence 播放序列。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_sequence(widget_t* widget, const char* sequence);

/**
 * @method image_animation_play
 * 播放。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_play(widget_t* widget);

/**
 * @method image_animation_stop
 * 停止(并重置index为0)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_stop(widget_t* widget);

/**
 * @method image_animation_pause
 * 暂停。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_pause(widget_t* widget);

/**
 * @method image_animation_cast
 * 转换为image_animation对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget image_animation对象。
 *
 * @return {widget_t*} image_animation对象。
 */
widget_t* image_animation_cast(widget_t* widget);

#define IMAGE_ANIMATION_PROP_LOOP "loop"
#define IMAGE_ANIMATION_PROP_SEQUENCE "sequence"
#define IMAGE_ANIMATION_PROP_INTERVAL "interval"
#define IMAGE_ANIMATION_PROP_AUTO_PLAY "auto_play"

#define WIDGET_TYPE_IMAGE_ANIMATION "image_animation"
#define IMAGE_ANIMATION(widget) ((image_animation_t*)(widget))

END_C_DECLS

#endif /*TK_IMAGE_ANIMATION_H*/
