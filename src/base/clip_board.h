/**
 * File:   clip_board.h
 * Author: AWTK Develop Team
 * Brief:  clip_board interface
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
 * 2018-11-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CLIP_BOARD_H
#define TK_CLIP_BOARD_H

#include "base/types_def.h"

BEGIN_C_DECLS

struct _clip_board_t;
typedef struct _clip_board_t clip_board_t;

/**
 * @enum clip_board_data_type_t
 * @annotation ["scriptable"]
 * @prefix CLIP_BOARD_DATA_TYPE_
 * 剪切板数据类型定义。
 */
typedef enum _clip_board_data_type_t {
  /**
   * @const CLIP_BOARD_DATA_TYPE_NONE
   * 无数据。
   */
  CLIP_BOARD_DATA_TYPE_NONE,

  /**
   * @const CLIP_BOARD_DATA_TYPE_TEXT
   * UTF8文本。
   */
  CLIP_BOARD_DATA_TYPE_TEXT
} clip_board_data_type_t;

typedef ret_t (*clip_board_clear_t)(clip_board_t* cl);
typedef ret_t (*clip_board_get_data_t)(clip_board_t* cl, clip_board_data_type_t* type,
                                       const void** data, uint32_t* size);
typedef ret_t (*clip_board_set_data_t)(clip_board_t* cl, clip_board_data_type_t type,
                                       const void* data, uint32_t size);
typedef ret_t (*clip_board_destroy_t)(clip_board_t* cl);

typedef struct _clip_board_vtable_t {
  clip_board_clear_t clear;
  clip_board_get_data_t get_data;
  clip_board_set_data_t set_data;
  clip_board_destroy_t destroy;
} clip_board_vtable_t;

/**
 * @class clip_board_t
 * @annotation ["scriptable"]
 * 剪切板接口。
 */
struct _clip_board_t {
  const clip_board_vtable_t* vt;
};

/**
 * @method clip_board
 * 获取缺省的剪切板对象。
 * @alias clip_board_instance
 * @annotation ["constructor"]
 *
 * @return {ret_t} 返回缺省剪切板对象。
 */
clip_board_t* clip_board(void);

/**
 * @method clip_board_set
 * 设置缺省的剪切板对象。
 * @param {clip_board_t*} cl 剪切板对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t clip_board_set(clip_board_t* cl);

/**
 * @method clip_board_clear
 * 清空剪切板中的数据。
 * @param {clip_board_t*} cl 剪切板对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t clip_board_clear(clip_board_t* cl);

/**
 * @method clip_board_set_data
 * 设置数据到剪切板中。
 * @param {clip_board_t*} cl 剪切板对象。
 * @param {clip_board_data_type_t} type 数据类型。
 * @param {const void* data} data 数据。
 * @param {uint32_t} size 数据长度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t clip_board_set_data(clip_board_t* cl, clip_board_data_type_t type, const void* data,
                          uint32_t size);

/**
 * @method clip_board_get_data
 * 从剪切板中获取数据。
 * @param {clip_board_t*} cl 剪切板对象。
 * @param {clip_board_data_type_t*} type 返回数据类型(可为NULL)。
 * @param {void** data} data 返回数据(可为NULL)。
 * @param {uint32_t*} size 返回数据长度(可为NULL)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t clip_board_get_data(clip_board_t* cl, clip_board_data_type_t* type, const void** data,
                          uint32_t* size);

/**
 * @method clip_board_destroy
 * 销毁剪切板对象。
 * @param {clip_board_t*} cl 剪切板对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t clip_board_destroy(clip_board_t* cl);

/**
 * @method clip_board_set_text
 * 设置文本(UTF8)数据到剪切板。
 * @annotation ["static", "scriptable"]
 * @param {const char*} text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t clip_board_set_text(const char* text);

/**
 * @method clip_board_get_text
 * 从剪切板中获取文本(UTF8)数据。
 * @annotation ["static", "scriptable"]
 *
 * @return {const char*} 返回文本数据。
 */
const char* clip_board_get_text(void);

END_C_DECLS

#endif /*TK_CLIP_BOARD_H*/
