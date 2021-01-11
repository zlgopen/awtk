## url\_t
### 概述
URL
----------------------------------
### 函数
<p id="url_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#url_t_url_create">url\_create</a> | 创建URL对象。 |
| <a href="#url_t_url_destroy">url\_destroy</a> | 销毁url对象。 |
| <a href="#url_t_url_get_param">url\_get\_param</a> | 获取参数。 |
| <a href="#url_t_url_set_host">url\_set\_host</a> | 设置主机名。 |
| <a href="#url_t_url_set_param">url\_set\_param</a> | 设置参数。 |
| <a href="#url_t_url_set_password">url\_set\_password</a> | 设置密码。 |
| <a href="#url_t_url_set_path">url\_set\_path</a> | 设置路径。 |
| <a href="#url_t_url_set_port">url\_set\_port</a> | 设置端口。 |
| <a href="#url_t_url_set_schema">url\_set\_schema</a> | 设置schema。 |
| <a href="#url_t_url_set_user_name">url\_set\_user\_name</a> | 设置用户名。 |
### 属性
<p id="url_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#url_t_host">host</a> | char* | 主机名。 |
| <a href="#url_t_params">params</a> | object\_t* | 参数集合。 |
| <a href="#url_t_password">password</a> | char* | 密码。 |
| <a href="#url_t_path">path</a> | char* | 路径。 |
| <a href="#url_t_port">port</a> | int32\_t | 端口。 |
| <a href="#url_t_schema">schema</a> | char* | schema。 |
| <a href="#url_t_user_name">user\_name</a> | char* | 用户名。 |
#### url\_create 函数
-----------------------

* 函数功能：

> <p id="url_t_url_create">创建URL对象。

* 函数原型：

```
url_t* url_create (const char* surl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | url\_t* | 返回URL对象。 |
| surl | const char* | 字符串格式的URL。 |
#### url\_destroy 函数
-----------------------

* 函数功能：

> <p id="url_t_url_destroy">销毁url对象。

* 函数原型：

```
ret_t url_destroy (url_t* url);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
#### url\_get\_param 函数
-----------------------

* 函数功能：

> <p id="url_t_url_get_param">获取参数。

* 函数原型：

```
const char* url_get_param (url_t* url, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回指定参数名的参数值。 |
| url | url\_t* | url对象。 |
| name | const char* | 参数名。 |
#### url\_set\_host 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_host">设置主机名。

* 函数原型：

```
ret_t url_set_host (url_t* url, const char* host);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| host | const char* | 主机名。 |
#### url\_set\_param 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_param">设置参数。

* 函数原型：

```
ret_t url_set_param (url_t* url, const char* name, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| name | const char* | 参数名。 |
| value | const char* | 参数值。 |
#### url\_set\_password 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_password">设置密码。

* 函数原型：

```
ret_t url_set_password (url_t* url, const char* password);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| password | const char* | 密码。 |
#### url\_set\_path 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_path">设置路径。

* 函数原型：

```
ret_t url_set_path (url_t* url, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| path | const char* | 路径。 |
#### url\_set\_port 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_port">设置端口。

* 函数原型：

```
ret_t url_set_port (url_t* url, int32_t port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| port | int32\_t | 端口。 |
#### url\_set\_schema 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_schema">设置schema。

* 函数原型：

```
ret_t url_set_schema (url_t* url, const char* schema);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| schema | const char* | schema。 |
#### url\_set\_user\_name 函数
-----------------------

* 函数功能：

> <p id="url_t_url_set_user_name">设置用户名。

* 函数原型：

```
ret_t url_set_user_name (url_t* url, const char* user_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | url\_t* | url对象。 |
| user\_name | const char* | 用户名。 |
#### host 属性
-----------------------
> <p id="url_t_host">主机名。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### params 属性
-----------------------
> <p id="url_t_params">参数集合。

* 类型：object\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### password 属性
-----------------------
> <p id="url_t_password">密码。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### path 属性
-----------------------
> <p id="url_t_path">路径。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### port 属性
-----------------------
> <p id="url_t_port">端口。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### schema 属性
-----------------------
> <p id="url_t_schema">schema。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### user\_name 属性
-----------------------
> <p id="url_t_user_name">用户名。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
