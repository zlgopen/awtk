/**
 * File:   dialog_highlighter_default.h
 * Author: AWTK Develop Team
 * Brief:  default dialog_highlighter
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied highlighterrranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIALOG_HIGHLIGHTER_DEFAULT_H
#define TK_DIALOG_HIGHLIGHTER_DEFAULT_H

#include "base/dialog_highlighter.h"

BEGIN_C_DECLS

struct _dialog_highlighter_default_t;
typedef struct _dialog_highlighter_default_t dialog_highlighter_default_t;

/**
 * @class dialog_highlighter_default_t
 * 缺省对话框高亮策略。
 *
 *>本策略在背景上画一层半透明的蒙版来高亮(突出)对话框本身。
 *>对于性能不高的平台，建议将start\_alpha和end\_alpha设为相同。
 *
 */
struct _dialog_highlighter_default_t {
  dialog_highlighter_t dialog_highlighter;

  /**
   * @property {uint8_t} start_alpha
   * 起始alpha，打开对话框的动画开始时的alpha值。
   */
  uint8_t start_alpha;

  /**
   * @property {uint8_t} end_alpha
   * 结束alpha，打开对话框的动画结束(直到对话框被关闭)时的alpha值。
   */
  uint8_t end_alpha;
};

/**
 * @method dialog_highlighter_default_create
 * 创建缺省的对话框高亮策略。
 * @annotation ["constructor"]
 * @param {object_t*} args 参数。
 *
 * @return {dialog_highlighter_t*} 返回对话框高亮策略对象。
 */
dialog_highlighter_t* dialog_highlighter_default_create(object_t* args);

#define DIALOG_HIGHLIGHTER_DEFAULT_ARG_START_ALPHA "start_alpha"
#define DIALOG_HIGHLIGHTER_DEFAULT_ARG_END_ALPHA "end_alpha"
#define DIALOG_HIGHLIGHTER_DEFAULT_ARG_ALPHA "alpha"

END_C_DECLS

#endif /*TK_DIALOG_HIGHLIGHTER_DEFAULT_H*/
