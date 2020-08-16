/**
 * File:   image_animation.h
 * Author: AWTK Develop Team
 * Brief:  image_animation
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
 * @annotation ["scriptable","design","widget"]
 * 图片动画控件，指定一个图片前缀，依次显示指定序列的图片，从而形成动画效果。
 *
 * 图片序列可以用sequence指定，也可以用start\_index和end\_index指定一个范围。
 *
 * image\_animation\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于image\_animation\_t控件。
 *
 * 在xml中使用"image\_animation"标签创建图片动画控件。如：
 *
 * ```xml
 * <image_animation image="ani" start_index="1" end_index="9" auto_play="true" interval="50"
 * delay="100"/>
 * ```
 *
 * > 更多用法请参考：
 * [image_animation.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/image_animation.xml)
 *
 * 在c代码中使用函数image\_animation\_create创建图片动画控件。如：
 *
 * ```c
 * image_animation = image_animation_create(win, 10, 10, 200, 200);
 * image_animation_set_image(image_animation, "ani");
 * image_animation_set_interval(image_animation, 50);
 * image_animation_set_range_sequence(image_animation, 1, 9);
 * image_animation_play(image_animation);
 * ```
 *
 * > 完整示例请参考：
 * [image_animation
 * demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/image_animation.c)
 *
 * 可用通过style来设置控件的显示风格，如背景颜色和边框等等，不过一般情况并不需要。
 *
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
   * 播放的序列，字符可选值为数字和英文大小写字母，字符可以重复。如：0123456789或者123123abcd。
   *
   */
  char* sequence;
  /**
   * @property {uint32_t} start_index
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片起始序数。
   */
  uint32_t start_index;
  /**
   * @property {uint32_t} end_index
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片结束序数。
   */
  uint32_t end_index;
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
   * @property {bool_t} unload_after_paint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 绘制完成后unload图片，以释放内存空间。
   */
  bool_t unload_after_paint;
  /**
   * @property {char*} format
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 索引到图片名转换时的格式，缺省为"%s%d"。
   */
  char* format;
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
  bool_t inited;
  int32_t index;
  uint32_t timer_id;
  void* image_buffer;
  char image_name[TK_NAME_LEN + 1];
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
 * 设置播放序列。比如image为"fire"，sequence为"12223", 将依次播放"fire1", "fire2", "fire2", "fire2",
 * "fire3"。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {const char*} sequence 播放序列。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_sequence(widget_t* widget, const char* sequence);

/**
 * @method image_animation_set_range_sequence
 * 设置播放序列。比如image为"fire"，start_index为0, end_index为99, 将依次播放"fire0", ...,
 * "fire99"。
 *
 *若指定的图片不存在，则重复上一张图片。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {uint32_t} start_index 图片起始序数。
 * @param {uint32_t} end_index 图片结束序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_range_sequence(widget_t* widget, uint32_t start_index,
                                         uint32_t end_index);

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
 * 停止(并重置index为-1)。
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
 * @method image_animation_next
 * 手动切换到下一张图片。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_next(widget_t* widget);

/**
 * @method image_animation_set_format
 * 设置生成图片名的格式。
 *
 * XXX:生成图片名时，第一个参数是图片名前缀，第二个是序数，只能在此前提下设置格式。
 *
 * ```
 *  const char* format = image_animation->format ? image_animation->format : "%s%d";
 *  tk_snprintf(name, TK_NAME_LEN, format, image_animation->image, image_animation->index);
 * ```
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_format(widget_t* widget, const char* format);

/**
 * @method image_animation_set_unload_after_paint
 * 设置绘制完成后unload图片，以释放内存空间。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image_animation对象。
 * @param {bool_t} unload_after_paint 是否绘制完成后unload图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_animation_set_unload_after_paint(widget_t* widget, bool_t unload_after_paint);

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
#define IMAGE_ANIMATION_PROP_START_INDEX "start_index"
#define IMAGE_ANIMATION_PROP_END_INDEX "end_index"
#define IMAGE_ANIMATION_PROP_INTERVAL "interval"
#define IMAGE_ANIMATION_PROP_AUTO_PLAY "auto_play"
#define IMAGE_ANIMATION_PROP_UNLOAD_AFTER_PAINT "unload_after_paint"

#define WIDGET_TYPE_IMAGE_ANIMATION "image_animation"
#define IMAGE_ANIMATION(widget) ((image_animation_t*)(image_animation_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(image_animation);

/*public for test*/
ret_t image_animation_update(widget_t* widget);
ret_t image_animation_get_image_name(image_animation_t* image_animation,
                                     char name[TK_NAME_LEN + 1]);

END_C_DECLS

#endif /*TK_IMAGE_ANIMATION_H*/
