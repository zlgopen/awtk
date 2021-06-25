/**
 * File:   layer.h
 * Author: AWTK Develop Team
 * Brief:  LCD layer。
 *
 * Copyright (c) Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#ifndef TK_LAYER_H
#define TK_LAYER_H

#include "tkc/object.h"
#include "base/widget.h"
#include "base/dirty_rects.h"

BEGIN_C_DECLS

/**
 * @class layer_t 
 * @parent object
 * LCD layer。
 */
typedef struct _layer_t {
  object_t object;
  
  /**
   * @property {char*} name
   * @annotation ["readable"]
   * 名称
   */
  char* name;

  /**
   * @property {uint32_t} x
   * @annotation ["readable"]
   * x坐标
   */
  uint32_t x;

  /**
   * @property {uint32_t} y
   * @annotation ["readable"]
   * y坐标
   */
  uint32_t y;

  /**
   * @property {uint32_t} max_fps
   * @annotation ["readable"]
   * 最大帧率
   */
  uint32_t max_fps;

  /*private*/
  canvas_t* canvas;
  uint32_t timer_id;
  darray_t windows;
  dirty_rects_t dirty_rects;
} layer_t;


/**
 * @method layer_set_name
 * 设置 名称
 * @param {layer_t*} layer layer对象。
 * @param {const char*} name 名称
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_set_name(layer_t* layer, const char* name);

/**
 * @method layer_set_x
 * 设置 x坐标
 * @param {layer_t*} layer layer对象。
 * @param {uint32_t} x x坐标
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_set_x(layer_t* layer, uint32_t x);

/**
 * @method layer_set_y
 * 设置 y坐标
 * @param {layer_t*} layer layer对象。
 * @param {uint32_t} y y坐标
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_set_y(layer_t* layer, uint32_t y);

/**
 * @method layer_set_max_fps
 * 设置 最大帧率
 * @param {layer_t*} layer layer对象。
 * @param {uint32_t} max_fps 最大帧率
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_set_max_fps(layer_t* layer, uint32_t max_fps);

#define LAYER_PROP_X "x"
#define LAYER_PROP_Y "y"
#define LAYER_PROP_W "w"
#define LAYER_PROP_H "h"
#define LAYER_PROP_NAME "name"
#define LAYER_PROP_MAX_FPS "max_fps"

/**
 * @method layer_create
 * 创建对象。
 * @param {const char*} name 名称。
 * @param {canvas_t*} canvas canvas对象。
 * @param {uint32_t} x x坐标。
 * @param {uint32_t} y y坐标。
 * @param {uint32_t} max_fps max_fps坐标。
 *
 * @return {layer_t*} 返回非NULL表示成功，否则表示失败。
 */
layer_t* layer_create(const char* name, canvas_t* canvas, uint32_t x, uint32_t y, uint32_t max_fps);

/**
 * @method layer_add_layer_window
 * 增加layer window。
 * @param {layer_t*} layer layer对象。
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_add_layer_window(layer_t* layer, widget_t* widget);

/**
 * @method layer_remove_layer_window
 * 移除layer window。
 * @param {layer_t*} layer layer对象。
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_remove_layer_window(layer_t* layer, widget_t* widget);

/**
 * @method layer_invalidate
 * 使指定区域无效。
 * @param {layer_t*} layer layer对象。
 * @param {rect_t*} rect rect对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_invalidate(layer_t* layer, const rect_t* rect);

/**
 * @method layer_start
 * 开始运行。
 * @param {layer_t*} layer layer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_start(layer_t* layer);

/*public for test*/
ret_t layer_paint(layer_t* layer);

END_C_DECLS

#endif/*TK_LAYER_H*/
