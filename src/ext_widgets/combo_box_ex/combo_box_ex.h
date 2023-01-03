/**
 * File:   combo_box_ex.h
 * Author: AWTK Develop Team
 * Brief:  combo_box_ex
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-21 Li DaHeng <linany1990@163.com> created
 *
 */

#ifndef TK_COMBO_BOX_EX_H
#define TK_COMBO_BOX_EX_H

#include "widgets/combo_box.h"

BEGIN_C_DECLS

/**
 * @class combo_box_ex_t
 * @parent combo_box_t
 * @annotation ["scriptable","design","widget"]
 * 扩展combo_box控件。支持以下功能：
 * * 支持滚动。项目比较多时显示滚动条。
 * * 自动调整弹出窗口的宽度。根据最长文本自动调整弹出窗口的宽度。
 * * 支持分组显示。如果item的文本以"seperator."开头，视为一个分组开始，其后的文本为分组的标题。比如: "seperator.basic"，会创建一个basic为标题的分组。
 */
typedef struct _combo_box_ex_t {
  combo_box_t combo_box;
} combo_box_ex_t;

/**
 * @method combo_box_ex_create
 * 创建combo_box_ex对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* combo_box_ex_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define WIDGET_TYPE_COMBO_BOX_EX "combo_box_ex"

#define COMBO_BOX_EX_SEPERATOR_PREFIX "seperator."

#define COMBO_BOX_EX(widget) ((combo_box_ex_t*)(combo_box_cast(WIDGET(widget))))

END_C_DECLS
#endif
