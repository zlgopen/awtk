# 输入事件记录与重放

## 1. 用途

输入事件记录与重放的常见用途有：

* 自动演示功能。
* 长时间运行进行压力测试。
* 辅助手工测试。有时出现崩溃的 BUG 时，往往忘记之前是如何操作的了，输入事件记录与重放可以精确重现问题。同时也可以减轻手工测试的工作量。

## 2.API

```c
/**
 * @method event_recorder_player_start_record
 * 开始事件记录。
 * @annotation ["static"]
 * @param {const char*} filename 用于保存事件的文件名。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t event_recorder_player_start_record(const char* filename);

/**
 * @method event_recorder_player_start_play
 * 开始事件重放。
 * @annotation ["static"]
 * @param {const char*} filename 存放事件的文件名。
 * @param {uint32_t} times 循环播放的次数。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t event_recorder_player_start_play(const char* filename, uint32_t times);

/**
 * @method event_recorder_player_stop_record
 * 停止事件记录。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t event_recorder_player_stop_record(void);

/**
 * @method event_recorder_player_stop_play
 * 停止事件重放。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t event_recorder_player_stop_play(void);
```

## 3. 用法

一般有两种方式启用输入事件记录与重放：

* 通过命令行参数启动记录或重放功能(目前没有提供示例)。

* 通过快捷键启动记录和重放功能。这种方法更为灵活，可以随时启用和停止，可以随时记录和播放。

> 要定义宏 WITH|_EVENT\_RECORDER\_PLAYER 才能启用事件记录与重放功能，PC 版本缺省是定义了该宏的。

demoui 中演示了通过快捷键启动记录和重放功能：

```c 
#include "base/event_recorder_player.h"
...

static ret_t on_key_record_play_events(void* ctx, event_t* e) {
  key_event_t* evt = (key_event_t*)e;

#ifdef WITH_EVENT_RECORDER_PLAYER
  if (evt->key == TK_KEY_F5) {
    event_recorder_player_start_record("event_log.bin");
    return RET_STOP;
  } else if (evt->key == TK_KEY_F6) {
    event_recorder_player_stop_record();
    return RET_STOP;
  } else if (evt->key == TK_KEY_F7) {
    event_recorder_player_start_play("event_log.bin", 0xffff);
    return RET_STOP;
  } else if (evt->key == TK_KEY_F8) {
    event_recorder_player_stop_play();
    return RET_STOP;
  }
#endif /*WITH_EVENT_RECORDER_PLAYER*/

  return RET_OK;
}

...

widget_on(wm, EVT_KEY_UP, on_key_record_play_events, wm);

```

在以上代码中：

* F5 键开始记录。
* F6 键停止记录。
* F7 键开始播放。
* F8 键停止播放。

## 4.已知问题

* 如果想重复播放记录的事件，确保记录事件时，回到初始界面后才停止记录。
* 目前平台原生输入法的输入事件没有记录。
