## window\_base\_t
### 概述
![image](images/window_base_t_0.png)

 窗口。
### 函数
<p id="window_base_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
### 属性
<p id="window_base_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#window_base_t_closable">closable</a> | window_closable_t | 收到EVT_REQUEST_CLOSE_WINDOW是否自动关闭窗口。 |
| <a href="#window_base_t_close_anim_hint">close\_anim\_hint</a> | char* | 关闭时的动画名称。 |
| <a href="#window_base_t_open_anim_hint">open\_anim\_hint</a> | char* | 打开时的动画名称。 |
| <a href="#window_base_t_script">script</a> | char* | 脚本文件名称(暂时没用)。 |
| <a href="#window_base_t_stage">stage</a> | char* | 窗口当前处于的状态。 |
| <a href="#window_base_t_theme">theme</a> | char* | 主题资源的名称。 |
### 事件
<p id="window_base_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### closable 属性
-----------------------
> <p id="window_base_t_closable"> 收到EVT_REQUEST_CLOSE_WINDOW是否自动关闭窗口。
 如果关闭窗口时，需要用户确认:
 * 1.将closable设置为WINDOW_CLOSABLE_CONFIRM
 * 2.处理窗口的EVT_REQUEST_CLOSE_WINDOW事件


* 类型：window\_closable\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### close\_anim\_hint 属性
-----------------------
> <p id="window_base_t_close_anim_hint"> 关闭时的动画名称。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### open\_anim\_hint 属性
-----------------------
> <p id="window_base_t_open_anim_hint"> 打开时的动画名称。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### script 属性
-----------------------
> <p id="window_base_t_script"> 脚本文件名称(暂时没用)。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### stage 属性
-----------------------
> <p id="window_base_t_stage"> 窗口当前处于的状态。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 否 |
#### theme 属性
-----------------------
> <p id="window_base_t_theme"> 主题资源的名称。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
