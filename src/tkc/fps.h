/**
 * File:   fps.h
 * Author: AWTK Develop Team
 * Brief:  calculate fps
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
#ifndef TK_FPS_H
#define TK_FPS_H

#include "tkc/time_now.h"

BEGIN_C_DECLS

/**
 * @class fps_t
 * 用于计算FPS(帧率)。
 */
typedef struct _fps_t {
  /*private*/
  uint64_t time;
  uint32_t count;
  uint32_t fps;
} fps_t;

/**
 * @method fps_init
 * 初始化。
 * @export none
 * @param {fps_t*} fps fps对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t fps_init(fps_t* fps) {
  return_value_if_fail(fps != NULL, RET_BAD_PARAMS);

  memset(fps, 0x00, sizeof(fps_t));
  fps->time = time_now_ms();

  return RET_OK;
}

/**
 * @method fps_inc
 * 增加帧数。
 * @export none
 * @param {fps_t*} fps fps对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t fps_inc(fps_t* fps) {
  return_value_if_fail(fps != NULL, RET_BAD_PARAMS);
  fps->count++;

  return RET_OK;
}

/**
 * @method fps_get
 * 获取FPS(帧率)。
 * @export none
 * @param {fps_t*} fps fps对象。
 *
 * @return {uint32_t} 返回FPS(帧率)。
 */
static inline uint32_t fps_get(fps_t* fps) {
  uint32_t elapse = 0;
  uint64_t now = time_now_ms();
  return_value_if_fail(fps != NULL, 0);

  elapse = now - fps->time;
  if (elapse > 200) {
    float value = (fps->count * 1000.0f) / (float)elapse;
    fps->count = 0;
    fps->time = now;
    fps->fps = tk_roundi(value);
  }

  return fps->fps;
}

END_C_DECLS

#endif /*TK_FPS_H*/
