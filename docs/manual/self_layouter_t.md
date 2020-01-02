## self\_layouter\_t
### 概述
控件自身排版布局器的接口。

按特定算法对控件进行排版布局，子类需要实现vtable中的函数。
----------------------------------
### 函数
<p id="self_layouter_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#self_layouter_t_self_layouter_clone">self\_layouter\_clone</a> | 克隆layouter对象。 |
| <a href="#self_layouter_t_self_layouter_create">self\_layouter\_create</a> | 创建layouter对象。 |
| <a href="#self_layouter_t_self_layouter_destroy">self\_layouter\_destroy</a> | 销毁layouter对象。 |
| <a href="#self_layouter_t_self_layouter_get_param">self\_layouter\_get\_param</a> | 获取指定的参数。 |
| <a href="#self_layouter_t_self_layouter_get_param_float">self\_layouter\_get\_param\_float</a> | 获取指定的浮点格式的参数。 |
| <a href="#self_layouter_t_self_layouter_get_param_int">self\_layouter\_get\_param\_int</a> | 获取指定的整数格式的参数。 |
| <a href="#self_layouter_t_self_layouter_layout">self\_layouter\_layout</a> | 对控件自身进行布局。 |
| <a href="#self_layouter_t_self_layouter_set_param">self\_layouter\_set\_param</a> | 获取指定的参数。 |
| <a href="#self_layouter_t_self_layouter_set_param_str">self\_layouter\_set\_param\_str</a> | 设置字符串格式的参数。 |
| <a href="#self_layouter_t_self_layouter_to_string">self\_layouter\_to\_string</a> | 获取全部参数。 |
#### self\_layouter\_clone 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_clone">克隆layouter对象。

* 函数原型：

```
self_layouter_t* self_layouter_clone (const char* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | self\_layouter\_t* | 返回layouter对象。 |
| layouter | const char* | 被克隆的对象。 |
#### self\_layouter\_create 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_create">创建layouter对象。

* 函数原型：

```
self_layouter_t* self_layouter_create (const char* params);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | self\_layouter\_t* | 返回layouter对象。 |
| params | const char* | 参数。 |
#### self\_layouter\_destroy 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_destroy">销毁layouter对象。

* 函数原型：

```
ret_t self_layouter_destroy (self_layouter_t* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | self\_layouter\_t* | layouter对象。 |
#### self\_layouter\_get\_param 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_get_param">获取指定的参数。

* 函数原型：

```
ret_t self_layouter_get_param (self_layouter_t* layouter, const char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | self\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| v | value\_t* | 返回参数的值。 |
#### self\_layouter\_get\_param\_float 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_get_param_float">获取指定的浮点格式的参数。

* 函数原型：

```
ret_t self_layouter_get_param_float (self_layouter_t* layouter, const char* name, float_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 成功返回参数的值，失败返回缺省值。 |
| layouter | self\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| defval | float\_t | 缺省值。 |
#### self\_layouter\_get\_param\_int 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_get_param_int">获取指定的整数格式的参数。

* 函数原型：

```
ret_t self_layouter_get_param_int (self_layouter_t* layouter, const char* name, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 成功返回参数的值，失败返回缺省值。 |
| layouter | self\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| defval | int32\_t | 缺省值。 |
#### self\_layouter\_layout 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_layout">对控件自身进行布局。

* 函数原型：

```
ret_t self_layouter_layout (self_layouter_t* layouter, widget_t* widget, rect_t* area);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | self\_layouter\_t* | layouter对象。 |
| widget | widget\_t* | 控件。 |
| area | rect\_t* | 可以使用的区域。 |
#### self\_layouter\_set\_param 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_set_param">获取指定的参数。

* 函数原型：

```
ret_t self_layouter_set_param (self_layouter_t* layouter, const char* name, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | self\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| v | const value\_t* | 参数的值。 |
#### self\_layouter\_set\_param\_str 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_set_param_str">设置字符串格式的参数。

* 函数原型：

```
ret_t self_layouter_set_param_str (self_layouter_t* layouter, const char* name, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | self\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| value | const char* | 参数值。 |
#### self\_layouter\_to\_string 函数
-----------------------

* 函数功能：

> <p id="self_layouter_t_self_layouter_to_string">获取全部参数。

* 函数原型：

```
ret_t self_layouter_to_string (self_layouter_t* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回字符串格式的参数。 |
| layouter | self\_layouter\_t* | layouter对象。 |
