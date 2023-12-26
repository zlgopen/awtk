## remote\_ui\_service\_args\_t
### 概述
remote ui服务端启动参数。
----------------------------------
### 属性
<p id="remote_ui_service_args_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#remote_ui_service_args_t_auth">auth</a> | tk\_service\_auth\_t | 登陆认证函数。 |
| <a href="#remote_ui_service_args_t_find_target">find\_target</a> | remote\_ui\_service\_find\_target\_t | 查找target的函数。 |
| <a href="#remote_ui_service_args_t_logout">logout</a> | tk\_service\_logout\_t | 登出函数。 |
#### auth 属性
-----------------------
> <p id="remote_ui_service_args_t_auth">登陆认证函数。

* 类型：tk\_service\_auth\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### find\_target 属性
-----------------------
> <p id="remote_ui_service_args_t_find_target">查找target的函数。

* 类型：remote\_ui\_service\_find\_target\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### logout 属性
-----------------------
> <p id="remote_ui_service_args_t_logout">登出函数。

* 类型：tk\_service\_logout\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
