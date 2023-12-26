## remote\_ui\_t
### 概述
remote ui客户端。
----------------------------------
### 函数
<p id="remote_ui_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#remote_ui_t_remote_ui_back_to_home">remote\_ui\_back\_to\_home</a> | 返回主窗口。 |
| <a href="#remote_ui_t_remote_ui_back_to_prev">remote\_ui\_back\_to\_prev</a> | 返回上一个窗口。 |
| <a href="#remote_ui_t_remote_ui_click">remote\_ui\_click</a> | 点击指定的控件。 |
| <a href="#remote_ui_t_remote_ui_close_window">remote\_ui\_close\_window</a> | 关闭窗口。 |
| <a href="#remote_ui_t_remote_ui_create">remote\_ui\_create</a> | 创建remote ui客户端。 |
| <a href="#remote_ui_t_remote_ui_create_dir">remote\_ui\_create\_dir</a> | 创建目录。 |
| <a href="#remote_ui_t_remote_ui_create_widget">remote\_ui\_create\_widget</a> | 创建控件。 |
| <a href="#remote_ui_t_remote_ui_destroy">remote\_ui\_destroy</a> | 销毁remote ui客户端。 |
| <a href="#remote_ui_t_remote_ui_destroy_widget">remote\_ui\_destroy\_widget</a> | 销毁控件。 |
| <a href="#remote_ui_t_remote_ui_dispatch">remote\_ui\_dispatch</a> | 分发事件。 |
| <a href="#remote_ui_t_remote_ui_download_file">remote\_ui\_download\_file</a> | 下载文件。 |
| <a href="#remote_ui_t_remote_ui_exec_fscript">remote\_ui\_exec\_fscript</a> | 执行fscript脚本。 |
| <a href="#remote_ui_t_remote_ui_get_dev_info">remote\_ui\_get\_dev\_info</a> | 获取设备信息。 |
| <a href="#remote_ui_t_remote_ui_get_loaded_assets_info">remote\_ui\_get\_loaded\_assets\_info</a> | 获取已经加载的资源。 |
| <a href="#remote_ui_t_remote_ui_get_loaded_images_info">remote\_ui\_get\_loaded\_images\_info</a> | 获取已经加载的图片。 |
| <a href="#remote_ui_t_remote_ui_get_manifest">remote\_ui\_get\_manifest</a> | 获取manifest。 |
| <a href="#remote_ui_t_remote_ui_get_prop">remote\_ui\_get\_prop</a> | 获取属性。 |
| <a href="#remote_ui_t_remote_ui_get_xml_source">remote\_ui\_get\_xml\_source</a> | 获取当前窗口的XML源码。 |
| <a href="#remote_ui_t_remote_ui_hook_log">remote\_ui\_hook\_log</a> | 设置log钩子函数。 |
| <a href="#remote_ui_t_remote_ui_key">remote\_ui\_key</a> | 发送按键给控件。 |
| <a href="#remote_ui_t_remote_ui_login">remote\_ui\_login</a> | 登录。 |
| <a href="#remote_ui_t_remote_ui_logout">remote\_ui\_logout</a> | 登出。 |
| <a href="#remote_ui_t_remote_ui_move_widget">remote\_ui\_move\_widget</a> | 移动控件。 |
| <a href="#remote_ui_t_remote_ui_off_event">remote\_ui\_off\_event</a> | 注销事件。 |
| <a href="#remote_ui_t_remote_ui_on_event">remote\_ui\_on\_event</a> | 注册事件。 |
| <a href="#remote_ui_t_remote_ui_open_window">remote\_ui\_open\_window</a> | 打开窗口。 |
| <a href="#remote_ui_t_remote_ui_reboot">remote\_ui\_reboot</a> | 重新启动。 |
| <a href="#remote_ui_t_remote_ui_remove_dir">remote\_ui\_remove\_dir</a> | 删除目录。 |
| <a href="#remote_ui_t_remote_ui_remove_file">remote\_ui\_remove\_file</a> | 删除文件。 |
| <a href="#remote_ui_t_remote_ui_resize_widget">remote\_ui\_resize\_widget</a> | 调整控件大小。 |
| <a href="#remote_ui_t_remote_ui_send_event">remote\_ui\_send\_event</a> | 发送事件。 |
| <a href="#remote_ui_t_remote_ui_set_language">remote\_ui\_set\_language</a> | 设置语言。 |
| <a href="#remote_ui_t_remote_ui_set_prop">remote\_ui\_set\_prop</a> | 设置属性。 |
| <a href="#remote_ui_t_remote_ui_set_theme">remote\_ui\_set\_theme</a> | 设置主题。 |
| <a href="#remote_ui_t_remote_ui_show_confirm">remote\_ui\_show\_confirm</a> | 显示确认对话框。 |
| <a href="#remote_ui_t_remote_ui_show_info">remote\_ui\_show\_info</a> | 显示信息对话框。 |
| <a href="#remote_ui_t_remote_ui_show_toast">remote\_ui\_show\_toast</a> | 显示信息对话框。 |
| <a href="#remote_ui_t_remote_ui_show_warn">remote\_ui\_show\_warn</a> | 显示警告对话框。 |
| <a href="#remote_ui_t_remote_ui_take_snapshot">remote\_ui\_take\_snapshot</a> | 截屏。 |
| <a href="#remote_ui_t_remote_ui_unhook_log">remote\_ui\_unhook\_log</a> | 取消log钩子函数。 |
| <a href="#remote_ui_t_remote_ui_upload_file">remote\_ui\_upload\_file</a> | 上传文件。 |
#### remote\_ui\_back\_to\_home 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_back_to_home">返回主窗口。

* 函数原型：

```
ret_t remote_ui_back_to_home (remote_ui_t* ui);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
#### remote\_ui\_back\_to\_prev 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_back_to_prev">返回上一个窗口。

* 函数原型：

```
ret_t remote_ui_back_to_prev (remote_ui_t* ui);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
#### remote\_ui\_click 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_click">点击指定的控件。

* 函数原型：

```
ret_t remote_ui_click (remote_ui_t* ui, const char* target);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
#### remote\_ui\_close\_window 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_close_window">关闭窗口。

* 函数原型：

```
ret_t remote_ui_close_window (remote_ui_t* ui, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| name | const char* | 窗口名称。 |
#### remote\_ui\_create 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_create">创建remote ui客户端。

* 函数原型：

```
remote_ui_t* remote_ui_create (tk_iostream_t* io);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | remote\_ui\_t* | 返回remote ui客户端对象。 |
| io | tk\_iostream\_t* | IO流。 |
#### remote\_ui\_create\_dir 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_create_dir">创建目录。

* 函数原型：

```
ret_t remote_ui_create_dir (remote_ui_t* ui, const char* remote_dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| remote\_dir | const char* | 远程目录。 |
#### remote\_ui\_create\_widget 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_create_widget">创建控件。

* 函数原型：

```
ret_t remote_ui_create_widget (remote_ui_t* ui, const char* target, const char* xml);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| xml | const char* | XML。 |
#### remote\_ui\_destroy 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_destroy">销毁remote ui客户端。

* 函数原型：

```
ret_t remote_ui_destroy (remote_ui_t* ui);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
#### remote\_ui\_destroy\_widget 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_destroy_widget">销毁控件。

* 函数原型：

```
ret_t remote_ui_destroy_widget (remote_ui_t* ui, const char* target);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
#### remote\_ui\_dispatch 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_dispatch">分发事件。

* 函数原型：

```
ret_t remote_ui_dispatch (remote_ui_t* ui);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
#### remote\_ui\_download\_file 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_download_file">下载文件。

* 函数原型：

```
ret_t remote_ui_download_file (remote_ui_t* ui, const char* remote_file, const char* local_file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| remote\_file | const char* | 远程文件。 |
| local\_file | const char* | 本地文件。 |
#### remote\_ui\_exec\_fscript 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_exec_fscript">执行fscript脚本。

* 函数原型：

```
ret_t remote_ui_exec_fscript (remote_ui_t* ui, const char* script, str_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| script | const char* | 脚本。 |
| result | str\_t* | 执行结果。 |
#### remote\_ui\_get\_dev\_info 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_get_dev_info">获取设备信息。

* 函数原型：

```
ret_t remote_ui_get_dev_info (remote_ui_t* ui, remote_ui_dev_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| info | remote\_ui\_dev\_info\_t* | 设备信息。 |
#### remote\_ui\_get\_loaded\_assets\_info 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_get_loaded_assets_info">获取已经加载的资源。

* 函数原型：

```
ret_t remote_ui_get_loaded_assets_info (remote_ui_t* ui, const char* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| file | const char* | 保存数据的文件名。 |
#### remote\_ui\_get\_loaded\_images\_info 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_get_loaded_images_info">获取已经加载的图片。

* 函数原型：

```
ret_t remote_ui_get_loaded_images_info (remote_ui_t* ui, const char* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| file | const char* | 保存数据的文件名。 |
#### remote\_ui\_get\_manifest 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_get_manifest">获取manifest。

* 函数原型：

```
ret_t remote_ui_get_manifest (remote_ui_t* ui, const char* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| file | const char* | 保存数据的文件名。 |
#### remote\_ui\_get\_prop 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_get_prop">获取属性。

* 函数原型：

```
ret_t remote_ui_get_prop (remote_ui_t* ui, const char* target, const char* name, value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| name | const char* | 属性名称。 |
| value | value\_t* | 属性值。 |
#### remote\_ui\_get\_xml\_source 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_get_xml_source">获取当前窗口的XML源码。

* 函数原型：

```
ret_t remote_ui_get_xml_source (remote_ui_t* ui, const char* target, const char* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标控件或窗口。 |
| file | const char* | 保存数据的文件名。 |
#### remote\_ui\_hook\_log 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_hook_log">设置log钩子函数。

* 函数原型：

```
ret_t remote_ui_hook_log (remote_ui_t* ui, tk_log_hook_t log, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| log | tk\_log\_hook\_t | log hook。 |
| ctx | void* | 上下文。 |
#### remote\_ui\_key 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_key">发送按键给控件。

* 函数原型：

```
ret_t remote_ui_key (remote_ui_t* ui, const char* target, int32_t key_code);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| key\_code | int32\_t | 按键代码。 |
#### remote\_ui\_login 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_login">登录。

* 函数原型：

```
ret_t remote_ui_login (remote_ui_t* ui, const char* username, const char* password);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| username | const char* | 用户名。 |
| password | const char* | 密码。 |
#### remote\_ui\_logout 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_logout">登出。

* 函数原型：

```
ret_t remote_ui_logout (remote_ui_t* ui);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
#### remote\_ui\_move\_widget 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_move_widget">移动控件。

* 函数原型：

```
ret_t remote_ui_move_widget (remote_ui_t* ui, const char* target, int32_t x, int32_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| x | int32\_t | x坐标。 |
| y | int32\_t | y坐标。 |
#### remote\_ui\_off\_event 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_off_event">注销事件。

* 函数原型：

```
ret_t remote_ui_off_event (remote_ui_t* ui, const char* target, uint32_t event, event_func_t func, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| event | uint32\_t | 事件。 |
| func | event\_func\_t | 事件处理函数。 |
| ctx | void* | 上下文。 |
#### remote\_ui\_on\_event 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_on_event">注册事件。

* 函数原型：

```
ret_t remote_ui_on_event (remote_ui_t* ui, const char* target, uint32_t event, event_func_t func, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| event | uint32\_t | 事件。 |
| func | event\_func\_t | 事件处理函数。 |
| ctx | void* | 上下文。 |
#### remote\_ui\_open\_window 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_open_window">打开窗口。

* 函数原型：

```
ret_t remote_ui_open_window (remote_ui_t* ui, const char* name, const char* xml, const char* init_json);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| name | const char* | 窗口名称。 |
| xml | const char* | 窗口XML。 |
| init\_json | const char* | 初始化JSON。 |
#### remote\_ui\_reboot 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_reboot">重新启动。

* 函数原型：

```
ret_t remote_ui_reboot (remote_ui_t* ui, remote_ui_reboot_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| type | remote\_ui\_reboot\_type\_t | 重启类型。 |
#### remote\_ui\_remove\_dir 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_remove_dir">删除目录。

* 函数原型：

```
ret_t remote_ui_remove_dir (remote_ui_t* ui, const char* remote_dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| remote\_dir | const char* | 远程目录。 |
#### remote\_ui\_remove\_file 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_remove_file">删除文件。

* 函数原型：

```
ret_t remote_ui_remove_file (remote_ui_t* ui, const char* remote_file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| remote\_file | const char* | 远程文件。 |
#### remote\_ui\_resize\_widget 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_resize_widget">调整控件大小。

* 函数原型：

```
ret_t remote_ui_resize_widget (remote_ui_t* ui, const char* target, uint32_t w, uint32_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 高度。 |
#### remote\_ui\_send\_event 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_send_event">发送事件。

* 函数原型：

```
ret_t remote_ui_send_event (remote_ui_t* ui, const char* target, event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| event | event\_t* | 事件。 |
#### remote\_ui\_set\_language 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_set_language">设置语言。

* 函数原型：

```
ret_t remote_ui_set_language (remote_ui_t* ui, const char* language);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| language | const char* | 语言名称。 |
#### remote\_ui\_set\_prop 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_set_prop">设置属性。

* 函数原型：

```
ret_t remote_ui_set_prop (remote_ui_t* ui, const char* target, const char* name, const value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标。 |
| name | const char* | 属性名称。 |
| value | const value\_t* | 属性值。 |
#### remote\_ui\_set\_theme 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_set_theme">设置主题。

* 函数原型：

```
ret_t remote_ui_set_theme (remote_ui_t* ui, const char* theme);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| theme | const char* | 主题名称。 |
#### remote\_ui\_show\_confirm 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_show_confirm">显示确认对话框。

* 函数原型：

```
ret_t remote_ui_show_confirm (remote_ui_t* ui, const char* title, const char* content);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| title | const char* | 标题。 |
| content | const char* | 内容。 |
#### remote\_ui\_show\_info 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_show_info">显示信息对话框。

* 函数原型：

```
ret_t remote_ui_show_info (remote_ui_t* ui, const char* title, const char* content);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| title | const char* | 标题。 |
| content | const char* | 内容。 |
#### remote\_ui\_show\_toast 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_show_toast">显示信息对话框。

* 函数原型：

```
ret_t remote_ui_show_toast (remote_ui_t* ui, uint32_t duration, const char* content);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| duration | uint32\_t | 时长。 |
| content | const char* | 内容。 |
#### remote\_ui\_show\_warn 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_show_warn">显示警告对话框。

* 函数原型：

```
ret_t remote_ui_show_warn (remote_ui_t* ui, const char* title, const char* content);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| title | const char* | 标题。 |
| content | const char* | 内容。 |
#### remote\_ui\_take\_snapshot 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_take_snapshot">截屏。

* 函数原型：

```
ret_t remote_ui_take_snapshot (remote_ui_t* ui, const char* target, const char* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| target | const char* | 目标控件或窗口。 |
| file | const char* | 保存截图文件名。 |
#### remote\_ui\_unhook\_log 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_unhook_log">取消log钩子函数。

* 函数原型：

```
ret_t remote_ui_unhook_log (remote_ui_t* ui);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
#### remote\_ui\_upload\_file 函数
-----------------------

* 函数功能：

> <p id="remote_ui_t_remote_ui_upload_file">上传文件。

* 函数原型：

```
ret_t remote_ui_upload_file (remote_ui_t* ui, const char* remote_file, const char* local_file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_t* | remote ui客户端对象。 |
| remote\_file | const char* | 远程文件。 |
| local\_file | const char* | 本地文件。 |
