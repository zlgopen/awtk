/**
 * File:   grid.h
 * Author: AWTK Develop Team
 * Brief:  网格。
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-06-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GRID_H
#define TK_GRID_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class grid_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 网格。
 * 在xml中使用"grid"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <grid x="c" y="50" w="100" h="100"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * 
 * ```xml
 * <!-- style -->
 * <grid>
 *   <style name="default" grid_color="gray" border_color="black" odd_bg_color="#f5f5f5" even_bg_color="#eeeeee">
 *     <normal />
 *   </style>
 * </grid>
 * ```
 */
typedef struct _grid_t {
  widget_t widget;

  /**
   * @property {uint32_t} rows
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 行数。
   */
  uint32_t rows;

  /**
   * @property {char*} columns_definition
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 各列的参数。
   * 各列的参数之间用英文的分号(;)分隔，每列参数的格式为：
   *
   * col(w=?,left_margin=?,right_margin=?,top_maorgin=?,bottom_margin=?)
   *
   * * w 为列的宽度(必须存在)。取值在(0-1]区间时，视为grid控件宽度的比例，否则为像素宽度。
   *   (如果为负数，将计算结果加上控件的宽度)
   * * left_margin(可选，可缩写为l) 该列左边的边距。
   * * right_margin(可选，可缩写为r) 该列右边的边距。
   * * top_margin(可选，可缩写为t) 该列顶部的边距。
   * * bottom_margin(可选，可缩写为b) 该列底部的边距。
   * * margin(可选，可缩写为m) 同时指定上面4个边距。
   * * fill_available(可选，可缩写为f) 填充剩余宽度(只有一列可以指定)。
   * 
   */
  char* columns_definition;

  /**
   * @property {bool_t} show_grid
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示网格。
   */
  bool_t show_grid;

  /*private*/
  darray_t cols_definition;
} grid_t;

/**
 * @method grid_create
 * @annotation ["constructor", "scriptable"]
 * 创建grid对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} grid对象。
 */
widget_t* grid_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method grid_cast
 * 转换为grid对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget grid对象。
 *
 * @return {widget_t*} grid对象。
 */
widget_t* grid_cast(widget_t* widget);

/**
 * @method grid_set_rows
 * 设置 行数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} rows 行数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t grid_set_rows(widget_t* widget, uint32_t rows);

/**
 * @method grid_set_columns_definition
 * 设置 各列的参数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} columns_definition 各列的参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t grid_set_columns_definition(widget_t* widget, const char* columns_definition);

/**
 * @method grid_set_show_grid
 * 设置 是否显示网格。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} show_grid 是否显示网格。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t grid_set_show_grid(widget_t* widget, bool_t show_grid);

#define GRID(widget) ((grid_t*)(grid_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(grid);

END_C_DECLS

#endif /*TK_GRID_H*/
