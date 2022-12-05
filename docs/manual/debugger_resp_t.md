## debugger\_resp\_t
### 概述
调试器响应头。
----------------------------------
### 属性
<p id="debugger_resp_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#debugger_resp_t_code">code</a> | uint32\_t | 响应码。 |
| <a href="#debugger_resp_t_error">error</a> | uint32\_t | 错误码(定义同ret_t)。 |
| <a href="#debugger_resp_t_size">size</a> | uint32\_t | 数据长度(除请求头外)。 |
| <a href="#debugger_resp_t_version">version</a> | uint32\_t | 版本号。 |
#### code 属性
-----------------------
> <p id="debugger_resp_t_code">响应码。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### error 属性
-----------------------
> <p id="debugger_resp_t_error">错误码(定义同ret_t)。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="debugger_resp_t_size">数据长度(除请求头外)。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### version 属性
-----------------------
> <p id="debugger_resp_t_version">版本号。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
