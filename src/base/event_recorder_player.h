/**
 * File:   event_recorder_player.h
 * Author: AWTK Develop Team
 * Brief:  event_recorder_player
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EVENT_RECORDER_PLAYER_H
#define TK_EVENT_RECORDER_PLAYER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class event_recorder_player_t
 * @annotation ["fake"]
 * 事件录制与重放，用于辅助自动测试和压力测试。
 *
 */

/**
 * @method event_recorder_player_start_record
 * 开始事件记录。
 * @annotation ["static"]
 * @param {const char*} filename 用于保存事件的文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t event_recorder_player_start_record(const char* filename);

/**
 * @method event_recorder_player_start_play
 * 开始事件重放。
 * @annotation ["static"]
 * @param {const char*} filename 存放事件的文件名。
 * @param {uint32_t} times 循环播放的次数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t event_recorder_player_start_play(const char* filename, uint32_t times);

/**
 * @method event_recorder_player_stop_record
 * 停止事件记录。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t event_recorder_player_stop_record(void);

/**
 * @method event_recorder_player_stop_play
 * 停止事件重放。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t event_recorder_player_stop_play(void);

END_C_DECLS

#endif /*TK_EVENT_RECORDER_PLAYER_H*/
