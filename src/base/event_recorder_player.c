/**
 * File:   event_recorder_player.c
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

#include "tkc/fs.h"
#include "tkc/types_def.h"
#include "base/main_loop.h"
#include "base/window_manager.h"
#include "base/event_recorder_player.h"

#ifdef WITH_EVENT_RECORDER_PLAYER

typedef union _log_event_t {
  key_event_t key_event;
  wheel_event_t wheel_event;
  pointer_event_t pointer_event;
} log_event_t;

typedef struct _event_recorder_player_t {
  fs_file_t* fp;
  bool_t playing;
  bool_t recording;

  uint32_t times;
  uint32_t timer_id;
  uint64_t last_event_time;
  log_event_t pending_event;
} event_recorder_player_t;

static event_recorder_player_t s_event_recorder_player;

static ret_t event_recorder_player_stop(void);
static ret_t event_recorder_player_play(event_recorder_player_t* p);

static ret_t dispatch_event_in_timer(const timer_info_t* info) {
  event_recorder_player_t* p = (event_recorder_player_t*)(info->ctx);

  return event_recorder_player_play(p);
}

static ret_t event_recorder_player_play(event_recorder_player_t* p) {
  log_event_t event;
  event_t* e = NULL;
  int64_t duration = 0;

  if (p->fp == NULL) {
    return RET_REMOVE;
  }

  if (p->last_event_time > 0) {
    event_queue_req_t req;
    memset(&req, 0x00, sizeof(req));
    e = (event_t*)&(p->pending_event);
    memcpy(&req, e, sizeof(p->pending_event));
    main_loop_queue_event(main_loop(), &req);
  } else {
    log_debug("prepare first event\n");
  }

  e = (event_t*)&event;
  memset(&event, 0x00, sizeof(event));
  if (!fs_file_eof(p->fp) && fs_file_read(p->fp, &event, sizeof(event)) == sizeof(event)) {
    if (p->last_event_time == 0) {
      p->last_event_time = e->time;
    }

    memcpy(&(p->pending_event), &event, sizeof(event));
    duration = e->time - p->last_event_time;
    p->last_event_time = e->time;

    if (duration < 0) {
      duration = 0;
    }

    if (duration > 100 * 1000) {
      duration = 100;
    }

    timer_modify(p->timer_id, duration);

    return RET_REPEAT;
  } else {
    p->times--;
    if (p->times > 0) {
      fs_file_seek(p->fp, 0);
      p->last_event_time = 0;
      log_debug("play event remain times: %d\n", p->times);

      return RET_REPEAT;
    } else {
      p->playing = FALSE;
      fs_file_close(p->fp);
      p->fp = NULL;

      log_debug("play event done\n");
      return RET_REMOVE;
    }
  }
}

static ret_t event_recorder_player_record(event_recorder_player_t* p, event_t* e) {
  log_event_t event;
  memset(&event, 0x00, sizeof(event));

  if (p->fp == NULL) {
    return RET_REMOVE;
  }

  switch (e->type) {
    case EVT_POINTER_DOWN:
    case EVT_POINTER_MOVE:
    case EVT_POINTER_UP: {
      memcpy(&event, e, sizeof(pointer_event_t));
      break;
    }
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      memcpy(&event, e, sizeof(key_event_t));
      break;
    }
    case EVT_WHEEL: {
      memcpy(&event, e, sizeof(wheel_event_t));
      break;
    }
    default: { return RET_OK; }
  }

  ENSURE(fs_file_write(p->fp, &event, sizeof(log_event_t)) == sizeof(log_event_t));

  return RET_OK;
}

static ret_t event_recorder_player_on_input_event(void* ctx, event_t* e) {
  event_recorder_player_t* p = (event_recorder_player_t*)ctx;

  if (p->recording) {
    return event_recorder_player_record(p, e);
  }

  return RET_REMOVE;
}

ret_t event_recorder_player_start_record(const char* filename) {
  event_recorder_player_t* p = &s_event_recorder_player;
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);
  event_recorder_player_stop();

  p->fp = fs_open_file(os_fs(), filename, "wb+");
  return_value_if_fail(p->fp != NULL, RET_BAD_PARAMS);

  p->recording = TRUE;
  widget_on(window_manager(), EVT_POINTER_DOWN, event_recorder_player_on_input_event, p);
  widget_on(window_manager(), EVT_POINTER_MOVE, event_recorder_player_on_input_event, p);
  widget_on(window_manager(), EVT_POINTER_UP, event_recorder_player_on_input_event, p);
  widget_on(window_manager(), EVT_WHEEL, event_recorder_player_on_input_event, p);
  widget_on(window_manager(), EVT_KEY_DOWN, event_recorder_player_on_input_event, p);
  widget_on(window_manager(), EVT_KEY_UP, event_recorder_player_on_input_event, p);

  log_debug("start record events: %s\n", filename);

  return RET_OK;
}

ret_t event_recorder_player_start_play(const char* filename, uint32_t times) {
  event_recorder_player_t* p = &s_event_recorder_player;
  return_value_if_fail(filename != NULL && times > 0, RET_BAD_PARAMS);

  event_recorder_player_stop();
  p->fp = fs_open_file(os_fs(), filename, "rb");
  return_value_if_fail(p->fp != NULL, RET_BAD_PARAMS);

  p->times = times;
  p->playing = TRUE;
  p->timer_id = timer_add(dispatch_event_in_timer, p, 0);

  log_debug("start play events: %s\n", filename);

  return RET_OK;
}

static ret_t event_recorder_player_stop(void) {
  event_recorder_player_t* p = &s_event_recorder_player;

  if (p->recording || p->playing) {
    fs_file_close(p->fp);

    p->fp = NULL;
    p->playing = FALSE;
    p->recording = FALSE;

    if (p->timer_id != TK_INVALID_ID) {
      timer_remove(p->timer_id);
      p->timer_id = TK_INVALID_ID;
    }

    widget_off_by_ctx(window_manager(), p);
    memset(p, 0x00, sizeof(event_recorder_player_t));
  }

  return RET_OK;
}

ret_t event_recorder_player_stop_play(void) {
  event_recorder_player_t* p = &s_event_recorder_player;
  return_value_if_fail(p->playing, RET_BAD_PARAMS);

  log_debug("event play stop ok\n");

  return event_recorder_player_stop();
}

ret_t event_recorder_player_stop_record(void) {
  event_recorder_player_t* p = &s_event_recorder_player;
  return_value_if_fail(p->recording, RET_BAD_PARAMS);

  log_debug("event record stop ok\n");

  return event_recorder_player_stop();
}
#else
ret_t event_recorder_player_start_record(const char* filename) {
  return RET_NOT_IMPL;
}

ret_t event_recorder_player_start_play(const char* filename, uint32_t times) {
  return RET_NOT_IMPL;
}

ret_t event_recorder_player_stop_play(void) {
  return RET_NOT_IMPL;
}

ret_t event_recorder_player_stop_record(void) {
  return RET_NOT_IMPL;
}
#endif /*WITH_EVENT_RECORDER_PLAYER*/
