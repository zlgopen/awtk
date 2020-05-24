## event\_recorder\_player\_t
### 概述
事件录制与重放，用于辅助自动测试和压力测试。
----------------------------------
### 函数
<p id="event_recorder_player_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#event_recorder_player_t_event_recorder_player_start_play">event\_recorder\_player\_start\_play</a> | 开始事件重放。 |
| <a href="#event_recorder_player_t_event_recorder_player_start_record">event\_recorder\_player\_start\_record</a> | 开始事件记录。 |
| <a href="#event_recorder_player_t_event_recorder_player_stop_play">event\_recorder\_player\_stop\_play</a> | 停止事件重放。 |
| <a href="#event_recorder_player_t_event_recorder_player_stop_record">event\_recorder\_player\_stop\_record</a> | 停止事件记录。 |
#### event\_recorder\_player\_start\_play 函数
-----------------------

* 函数功能：

> <p id="event_recorder_player_t_event_recorder_player_start_play">开始事件重放。

* 函数原型：

```
ret_t event_recorder_player_start_play (const char* filename, uint32_t times);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| filename | const char* | 存放事件的文件名。 |
| times | uint32\_t | 循环播放的次数。 |
#### event\_recorder\_player\_start\_record 函数
-----------------------

* 函数功能：

> <p id="event_recorder_player_t_event_recorder_player_start_record">开始事件记录。

* 函数原型：

```
ret_t event_recorder_player_start_record (const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| filename | const char* | 用于保存事件的文件名。 |
#### event\_recorder\_player\_stop\_play 函数
-----------------------

* 函数功能：

> <p id="event_recorder_player_t_event_recorder_player_stop_play">停止事件重放。

* 函数原型：

```
ret_t event_recorder_player_stop_play ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### event\_recorder\_player\_stop\_record 函数
-----------------------

* 函数功能：

> <p id="event_recorder_player_t_event_recorder_player_stop_record">停止事件记录。

* 函数原型：

```
ret_t event_recorder_player_stop_record ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
