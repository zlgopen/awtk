## children\_layouter\_t
### 概述
子控件排版布局器的接口。

按特定算法对子控件进行排版布局，子类需要实现vtable中的函数。
----------------------------------
### 函数
<p id="children_layouter_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#children_layouter_t_children_layouter_clone">children\_layouter\_clone</a> | 克隆layouter对象。 |
| <a href="#children_layouter_t_children_layouter_create">children\_layouter\_create</a> | 创建layouter对象。 |
| <a href="#children_layouter_t_children_layouter_destroy">children\_layouter\_destroy</a> | 销毁layouter对象。 |
| <a href="#children_layouter_t_children_layouter_get_param">children\_layouter\_get\_param</a> | 获取指定的参数。 |
| <a href="#children_layouter_t_children_layouter_get_param_float">children\_layouter\_get\_param\_float</a> | 获取指定的浮点格式的参数。 |
| <a href="#children_layouter_t_children_layouter_get_param_int">children\_layouter\_get\_param\_int</a> | 获取指定的整数格式的参数。 |
| <a href="#children_layouter_t_children_layouter_is_valid">children\_layouter\_is\_valid</a> | 判断layouter对象是否有效。 |
| <a href="#children_layouter_t_children_layouter_layout">children\_layouter\_layout</a> | 对控件的子控件进行布局。 |
| <a href="#children_layouter_t_children_layouter_set_param">children\_layouter\_set\_param</a> | 获取指定的参数。 |
| <a href="#children_layouter_t_children_layouter_set_param_str">children\_layouter\_set\_param\_str</a> | 设置字符串格式的参数。 |
| <a href="#children_layouter_t_children_layouter_to_string">children\_layouter\_to\_string</a> | 获取全部参数。 |
#### children\_layouter\_clone 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_clone">克隆layouter对象。

* 函数原型：

```
children_layouter_t* children_layouter_clone (children_layouter_t* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | children\_layouter\_t* | 返回layouter对象。 |
| layouter | children\_layouter\_t* | 被克隆的对象。 |
#### children\_layouter\_create 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_create">创建layouter对象。

* 函数原型：

```
children_layouter_t* children_layouter_create (const char* params);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | children\_layouter\_t* | 返回layouter对象。 |
| params | const char* | 参数。 |
#### children\_layouter\_destroy 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_destroy">销毁layouter对象。

* 函数原型：

```
ret_t children_layouter_destroy (children_layouter_t* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | children\_layouter\_t* | layouter对象。 |
#### children\_layouter\_get\_param 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_get_param">获取指定的参数。

* 函数原型：

```
ret_t children_layouter_get_param (children_layouter_t* layouter, const char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | children\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| v | value\_t* | 返回参数的值。 |
#### children\_layouter\_get\_param\_float 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_get_param_float">获取指定的浮点格式的参数。

* 函数原型：

```
ret_t children_layouter_get_param_float (children_layouter_t* layouter, const char* name, float_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 成功返回参数的值，失败返回缺省值。 |
| layouter | children\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| defval | float\_t | 缺省值。 |
#### children\_layouter\_get\_param\_int 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_get_param_int">获取指定的整数格式的参数。

* 函数原型：

```
ret_t children_layouter_get_param_int (children_layouter_t* layouter, const char* name, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 成功返回参数的值，失败返回缺省值。 |
| layouter | children\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| defval | int32\_t | 缺省值。 |
#### children\_layouter\_is\_valid 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_is_valid">判断layouter对象是否有效。

* 函数原型：

```
bool_t children_layouter_is_valid (children_layouter_t* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示有效，否则表示无效。 |
| layouter | children\_layouter\_t* | layouter对象。 |
#### children\_layouter\_layout 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_layout">对控件的子控件进行布局。

* 函数原型：

```
ret_t children_layouter_layout (children_layouter_t* layouter, widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | children\_layouter\_t* | layouter对象。 |
| widget | widget\_t* | 控件。 |
#### children\_layouter\_set\_param 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_set_param">获取指定的参数。

* 函数原型：

```
ret_t children_layouter_set_param (children_layouter_t* layouter, const char* name, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | children\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| v | const value\_t* | 参数的值。 |
#### children\_layouter\_set\_param\_str 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_set_param_str">设置字符串格式的参数。

* 函数原型：

```
ret_t children_layouter_set_param_str (children_layouter_t* layouter, const char* name, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| layouter | children\_layouter\_t* | layouter对象。 |
| name | const char* | 参数名。 |
| value | const char* | 参数值。 |
#### children\_layouter\_to\_string 函数
-----------------------

* 函数功能：

> <p id="children_layouter_t_children_layouter_to_string">获取全部参数。

* 函数原型：

```
ret_t children_layouter_to_string (children_layouter_t* layouter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回字符串格式的参数。 |
| layouter | children\_layouter\_t* | layouter对象。 |
