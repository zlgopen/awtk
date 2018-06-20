/**
 * File:   input_method.h
 * Author: AWTK Develop Team
 * Brief:  input method interface.
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
 * 2018-06-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_INPUT_METHOD_H
#define TK_INPUT_METHOD_H

#include "base/widget.h"

BEGIN_C_DECLS

struct _input_method_t;
typedef struct _input_method_t input_method_t;

typedef ret_t (*input_method_request_t)(input_method_t* im, widget_t* widget);

/**
 * @enum input_type_t
 * @scriptable
 * @prefix INPUT_
 * 输入类型常量定义。
 */
typedef enum _input_type_t {
  /**
   * @const INPUT_TEXT
   * 文本。
   */
  INPUT_TEXT = 0,
  /**
   * @const INPUT_INT
   * 整数。
   */
  INPUT_INT,
  /**
   * @const INPUT_UINT
   * 非负整数。
   */
  INPUT_UINT,
  /**
   * @const INPUT_HEX
   * 16进制整数。
   */
  INPUT_HEX,
  /**
   * @const INPUT_FLOAT
   * 浮点数。
   */
  INPUT_FLOAT,
  /**
   * @const INPUT_UFLOAT
   * 非负浮点数。
   */
  INPUT_UFLOAT,
  /**
   * @const INPUT_EMAIL
   * 邮件地址。
   */
  INPUT_EMAIL,
  /**
   * @const INPUT_PASSWORD
   * 密码。
   */
  INPUT_PASSWORD,
  /**
   * @const INPUT_PHONE
   * 电话号码。
   */
  INPUT_PHONE
} input_type_t;

/**
 * @class input_method_t
 * @scriptable no
 * 输入法接口。
 */
typedef struct _input_method_t {
  /**
   * @property {widget_t*} widget;
   * @readonly
   * 当前焦点控件。
   */
  widget_t* widget;

  widget_t* keyboard;

  emitter_t emitter;
  input_type_t input_type;
  input_method_request_t request;
} input_method_t;
/**
 * @class im_commit_event_t
 * @scriptable no
 * @parent event_t
 * 输入法提交输入的文本事件。
 */
typedef struct _im_commit_event_t {
  event_t e;
  /**
   * @property {char*} str
   * @readonly
   * 提交的文本。
   */
  const char* str;
} im_commit_event_t;

/**
 * @class im_candidates_event_t
 * @scriptable no
 * @parent event_t
 * 输入法提交输入的文本事件。
 */
typedef struct _im_candidates_event_t {
  event_t e;
  /**
   * @property {char*} candidates
   * @readonly
   * 可选的文本，多个文本以\0分隔。
   */
  const char* candidates;
  /**
   * @property {uint32_t} candidates_nr
   * @readonly
   * 可选的文本的个数。
   */
  uint32_t candidates_nr;
} im_candidates_event_t;

/**
 * @class im_done_info_event_t
 * @scriptable no
 * @parent event_t
 * 更新软键盘上Done按钮的信息事件。
 */
typedef struct _im_done_info_event_t {
  event_t e;
  /**
   * @property {char*} text
   * @readonly
   * Done按钮的文本。
   */
  const char* text;
  /**
   * @property {bool_t} enable
   * @readonly
   * Done是否可用。
   */
  bool_t enable;
} im_done_info_event_t;

/**
 * @method input_method_dispatch
 * 分发一个事件。
 * @param {input_method_t*} im 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_method_dispatch(input_method_t* im, event_t* e);

/**
 * @method input_method_dispatch_to_widget
 * 分发一个事件当前焦点控件。
 * @param {input_method_t*} im 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_method_dispatch_to_widget(input_method_t* im, event_t* e);

/**
 * @method input_method_on
 * 注册指定事件的处理函数。
 * @param {input_method_t*} im 输入法对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于input_method_off。
 */
uint32_t input_method_on(input_method_t* im, event_type_t type, event_func_t on_event, void* ctx);

/**
 * @method input_method_off
 * 注销指定事件的处理函数。
 * @param {input_method_t*} im 输入法对象。
 * @param {uint32_t} id input_method_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_method_off(input_method_t* im, uint32_t id);

/**
 * @method input_method_request
 * 让指定的控件启用输入法。
 * @param {input_method_t*} im 输入法对象。
 * @param {widget_t*} widget 焦点控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_method_request(input_method_t* im, widget_t* widget);

/**
 * @method input_method_create
 * 创建输入法对象。
 *
 * @return {input_method_t*} 成功返回输入法对象，失败返回NULL。
 */
input_method_t* input_method_create(void);

/**
 * @method input_method_destroy
 * 销毁输入法对象。
 * @param {input_method_t*} im 输入法对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_method_destroy(input_method_t* im);

/**
 * @method input_method
 * 获取全局输入法对象。
 *
 * @return {input_method_t*} 成功返回输入法对象，失败返回NULL。
 */
input_method_t* input_method(void);

/**
 * @method input_method_set
 * 设置全局输入法对象。
 * @param {input_method_t*} im 输入法对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t input_method_set(input_method_t* im);

END_C_DECLS

#endif /*TK_INPUT_METHOD_H*/
