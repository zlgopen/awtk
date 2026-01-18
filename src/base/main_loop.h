/**
 * File:   main_loop.h
 * Author: AWTK Develop Team
 * Brief:  main_loop interface
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  uint64_t last_loop_time;
  uint32_t curr_expected_sleep_time;
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

/**
 * @method main_loop_set
 * 设置当前main_loop对象
 * @param {main_loop_t*} loop main_loop对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_set(main_loop_t* loop);

/**
 * @method main_loop_run
 * 运行主循环。
 * @param {main_loop_t*} l main_loop对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_run(main_loop_t* l);

/**
 * @method main_loop_wakeup
 * 唤醒主循环。
 * @param {main_loop_t*} l main_loop对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_wakeup(main_loop_t* l);

/**
 * @method main_loop_quit
 * 退出主循环。
 * @param {main_loop_t*} l main_loop对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_quit(main_loop_t* l);

/**
 * @method main_loop_queue_event
 * 将事件加入到事件队列。
 * @param {main_loop_t*} l main_loop对象。
 * @param {const event_queue_req_t*} e 事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_queue_event(main_loop_t* l, const event_queue_req_t* e);

/**
 * @method main_loop_recv_event
 * 从事件队列中获取事件。
 * @param {main_loop_t*} l main_loop对象。
 * @param {event_queue_req_t*} r 事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_recv_event(main_loop_t* l, event_queue_req_t* r);

/**
 * @method main_loop_destroy
 * 销毁main_loop对象。
 * @param {main_loop_t*} l main_loop对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_destroy(main_loop_t* l);

/**
 * @method main_loop_step
 * 执行一次迭代。
 * @param {main_loop_t*} l main_loop对象。
 * 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_step(main_loop_t* l);

ret_t main_loop_sleep(main_loop_t* l);
/*event_source*/

/**
 * @method main_loop_get_event_source_manager
 * 获取event source manager
 * @param {main_loop_t*} l main_loop对象。
 *
 * @return {event_source_manager_t*} 返回event source manager对象。
 */
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

/**
 * @method main_loop_remove_event_source_by_tag
 * 删除event_source。
 * @param {main_loop_t*} l main_loop对象。
 * @param {void*} tag tag。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t main_loop_remove_event_source_by_tag(main_loop_t* l, void* tag);

/* private */
ret_t main_loop_set_curr_expected_sleep_time(main_loop_t* l, uint32_t sleep_time);

END_C_DECLS

#endif /*TK_MAIN_LOOP_H*/
