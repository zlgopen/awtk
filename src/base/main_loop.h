/**
 * File:   main_loop.h
 * Author: AWTK Develop Team
 * Brief:  main_loop interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MAIN_LOOP_H
#define TK_MAIN_LOOP_H

#include "base/widget.h"
#include "base/event_queue.h"
#include "tkc/event_source_manager.h"

BEGIN_C_DECLS

struct _main_loop_t;
typedef struct _main_loop_t main_loop_t;

typedef ret_t (*main_loop_run_t)(main_loop_t* l);
typedef ret_t (*main_loop_quit_t)(main_loop_t* l);
typedef ret_t (*main_loop_queue_event_t)(main_loop_t* l, const event_queue_req_t* e);
typedef ret_t (*main_loop_recv_event_t)(main_loop_t* loop, event_queue_req_t* r);
typedef event_source_manager_t* (*main_loop_get_event_source_manager_t)(main_loop_t* l);
typedef ret_t (*main_loop_wakeup_t)(main_loop_t* l);
typedef ret_t (*main_loop_step_t)(main_loop_t* l);
typedef ret_t (*main_loop_sleep_t)(main_loop_t* l);
typedef ret_t (*main_loop_destroy_t)(main_loop_t* l);

/**
 * @class main_loop_t
 * 主循环接口。
 *
 */
struct _main_loop_t {
  main_loop_run_t run;
  main_loop_quit_t quit;
  main_loop_step_t step;
  main_loop_sleep_t sleep;
  main_loop_wakeup_t wakeup;
  main_loop_recv_event_t recv_event;
  main_loop_queue_event_t queue_event;
  main_loop_get_event_source_manager_t get_event_source_manager;
  main_loop_destroy_t destroy;

  uint8_t running;
  uint8_t quit_num;
  bool_t app_quited;
  uint32_t last_loop_time;
  widget_t* wm;
};

main_loop_t* main_loop_init(int w, int h);

/**
 * @method main_loop
 * 获取当前main_loop对象
 * 
 * @annotation ["static"]
 *
 * @return {main_loop_t*} 返回main_loop对象。
 */
main_loop_t* main_loop(void);

ret_t main_loop_set(main_loop_t* loop);
ret_t main_loop_run(main_loop_t* l);
ret_t main_loop_wakeup(main_loop_t* l);
ret_t main_loop_quit(main_loop_t* l);
ret_t main_loop_queue_event(main_loop_t* l, const event_queue_req_t* e);
ret_t main_loop_recv_event(main_loop_t* l, event_queue_req_t* r);
ret_t main_loop_destroy(main_loop_t* l);

ret_t main_loop_step(main_loop_t* l);
ret_t main_loop_sleep(main_loop_t* l);

/*event_source*/
event_source_manager_t* main_loop_get_event_source_manager(main_loop_t* l);
/**
 * @method main_loop_add_event_source
 * 添加event_source。
 * @param {main_loop_t*} l main_loop对象。
 * @param {event_source_t*} source event_source对象。
 * 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_add_event_source(main_loop_t* l, event_source_t* source);

/**
 * @method main_loop_remove_event_source
 * 删除event_source。
 * @param {main_loop_t*} l main_loop对象。
 * @param {event_source_t*} source event_source对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_remove_event_source(main_loop_t* l, event_source_t* source);

END_C_DECLS

#endif /*TK_MAIN_LOOP_H*/
