## input\_engine\_t
### 概述
输入法引擎接口。

常见的实现方式有以下几种：

* 空实现。用于不需要输入法的嵌入式平台。

* 拼音输入法实现。用于需要输入法的嵌入式平台。

![image](images/input_engine_t_0.png)

----------------------------------
### 函数
<p id="input_engine_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#input_engine_t_input_engine_add_candidate">input\_engine\_add\_candidate</a> | 给输入法引擎对象加入一个候选字。 |
| <a href="#input_engine_t_input_engine_add_candidates_from_char">input\_engine\_add\_candidates\_from\_char</a> | 根据字符类型给输入法引擎对象加入所有符合的候选字。 |
| <a href="#input_engine_t_input_engine_add_candidates_from_string">input\_engine\_add\_candidates\_from\_string</a> | 根据字符串给输入法引擎对象加入所有符合的候选字。 |
| <a href="#input_engine_t_input_engine_create">input\_engine\_create</a> | 创建输入法引擎对象。 |
| <a href="#input_engine_t_input_engine_deinit">input\_engine\_deinit</a> | 释放输入法引擎对象。 |
| <a href="#input_engine_t_input_engine_destroy">input\_engine\_destroy</a> | 销毁输入法引擎对象。 |
| <a href="#input_engine_t_input_engine_dispatch_candidates">input\_engine\_dispatch\_candidates</a> | 请求显示候选字。 |
| <a href="#input_engine_t_input_engine_init">input\_engine\_init</a> | 初始化输入法引擎对象。 |
| <a href="#input_engine_t_input_engine_input">input\_engine\_input</a> | 输入新的按键，并更新候选字。 |
| <a href="#input_engine_t_input_engine_reset_candidates">input\_engine\_reset\_candidates</a> | 重置输入法引擎对象。 |
| <a href="#input_engine_t_input_engine_reset_input">input\_engine\_reset\_input</a> | 清除所有输入的按键组合和候选字。 |
| <a href="#input_engine_t_input_engine_search">input\_engine\_search</a> | 根据按键组合，更新候选字，并通过输入法对象提交候选字和pre候选字。 |
### 属性
<p id="input_engine_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
#### input\_engine\_add\_candidate 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_add_candidate">给输入法引擎对象加入一个候选字。

* 函数原型：

```
ret_t input_engine_add_candidate (input_engine_t* engine);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
#### input\_engine\_add\_candidates\_from\_char 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_add_candidates_from_char">根据字符类型给输入法引擎对象加入所有符合的候选字。
备注：搜索表是二维数组，需要通过参数 c 来确定使用具体行数。

* 函数原型：

```
ret_t input_engine_add_candidates_from_char (input_engine_t* engine, const wchar_t** table, char c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
| table | const wchar\_t** | 输入法搜索表。 |
| c | char | 需要显示查询的字符。 |
#### input\_engine\_add\_candidates\_from\_string 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_add_candidates_from_string">根据字符串给输入法引擎对象加入所有符合的候选字。

* 函数原型：

```
ret_t input_engine_add_candidates_from_string (input_engine_t* engine, const table_entry_t* items, uint32_t items_nr, const char* key, bool_t exact);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
| items | const table\_entry\_t* | 输入法搜索表。 |
| items\_nr | uint32\_t | 输入法搜索表长度。 |
| key | const char* | 需要显示查询的字符。 |
| exact | bool\_t | 是否只查询完全匹配的字符串。 |
#### input\_engine\_create 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_create">创建输入法引擎对象。

* 函数原型：

```
input_engine_t* input_engine_create (input_method_t* im);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | input\_engine\_t* | 返回输入法引擎对象。 |
| im | input\_method\_t* | 输入法对象。 |
#### input\_engine\_deinit 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_deinit">释放输入法引擎对象。

* 函数原型：

```
ret_t input_engine_deinit (input_engine_t* engine);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
#### input\_engine\_destroy 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_destroy">销毁输入法引擎对象。

* 函数原型：

```
ret_t input_engine_destroy (input_engine_t* engine);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
#### input\_engine\_dispatch\_candidates 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_dispatch_candidates">请求显示候选字。

* 函数原型：

```
ret_t input_engine_dispatch_candidates (input_engine_t* engine, int32_t selected);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
| selected | int32\_t | 缺省选中候选字的序数。 |
#### input\_engine\_init 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_init">初始化输入法引擎对象。

* 函数原型：

```
ret_t input_engine_init (input_engine_t* engine);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
#### input\_engine\_input 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_input">输入新的按键，并更新候选字。

* 函数原型：

```
ret_t input_engine_input (input_engine_t* engine, int key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
| key | int | 键值。 |
#### input\_engine\_reset\_candidates 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_reset_candidates">重置输入法引擎对象。

* 函数原型：

```
ret_t input_engine_reset_candidates (input_engine_t* engine);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
#### input\_engine\_reset\_input 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_reset_input">清除所有输入的按键组合和候选字。

* 函数原型：

```
ret_t input_engine_reset_input (input_engine_t* engine);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
#### input\_engine\_search 函数
-----------------------

* 函数功能：

> <p id="input_engine_t_input_engine_search">根据按键组合，更新候选字，并通过输入法对象提交候选字和pre候选字。

* 函数原型：

```
ret_t input_engine_search (input_engine_t* engine, const char* keys);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| engine | input\_engine\_t* | 输入法引擎对象。 |
| keys | const char* | 按键组合。 |
