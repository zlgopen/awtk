## window\_animator\_t
### 概述
 窗口动画。

### 函数
<p id="window_animator_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#window_animator_t_window_animator_create_for_close">window\_animator\_create\_for\_close</a> |  为关闭窗口创建动画。 |
| <a href="#window_animator_t_window_animator_create_for_open">window\_animator\_create\_for\_open</a> |  为打开窗口创建动画。 |
| <a href="#window_animator_t_window_animator_destroy">window\_animator\_destroy</a> |  销毁窗口动画对象。 |
| <a href="#window_animator_t_window_animator_update">window\_animator\_update</a> |  更新动画。 |
### 属性
<p id="window_animator_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="window_animator_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### window\_animator\_create\_for\_close 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_t* | 窗口动画对象。 |
| type | char* | 动画类型。 |
| c | canvas\_t* | canvas。 |
| prev\_win | widget\_t* | 前一窗口。 |
| curr\_win | widget\_t* | 当前窗口。 |
<p id="window_animator_t_window_animator_create_for_close"> 为关闭窗口创建动画。




#### window\_animator\_create\_for\_open 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_t* | 窗口动画对象。 |
| type | char* | 动画类型。 |
| c | canvas\_t* | canvas。 |
| prev\_win | widget\_t* | 前一窗口。 |
| curr\_win | widget\_t* | 当前窗口。 |
<p id="window_animator_t_window_animator_create_for_open"> 为打开窗口创建动画。




#### window\_animator\_destroy 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
<p id="window_animator_t_window_animator_destroy"> 销毁窗口动画对象。




#### window\_animator\_update 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
| canvas | canvas\_t* | 画布对象。 |
| time\_ms | uint32\_t | 当前时间(毫秒)。 |
<p id="window_animator_t_window_animator_update"> 更新动画。




