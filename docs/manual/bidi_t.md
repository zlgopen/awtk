## bidi\_t
### 概述
Unicode Bidirectional Algorithm.
----------------------------------
### 函数
<p id="bidi_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#bidi_t_bidi_deinit">bidi\_deinit</a> | 释放bidi对象相关资源。 |
| <a href="#bidi_t_bidi_init">bidi\_init</a> | 初始化bidi对象。 |
| <a href="#bidi_t_bidi_log2vis">bidi\_log2vis</a> | 将字符串转成用于显示的字符串，输出结果放在bidi->vis_str中。 |
| <a href="#bidi_t_bidi_type_from_name">bidi\_type\_from\_name</a> | 将bidi类型的名称转换成类型。 |
### 属性
<p id="bidi_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#bidi_t_positions_L_to_V">positions\_L\_to\_V</a> | int32\_t* | logical位置与visual位置的映射。 |
| <a href="#bidi_t_positions_V_to_L">positions\_V\_to\_L</a> | int32\_t* | visual位置与logical位置的映射。 |
| <a href="#bidi_t_request_type">request\_type</a> | bidi\_type\_t | 请求的类型。 |
| <a href="#bidi_t_resolved_type">resolved\_type</a> | bidi\_type\_t | 实际的类型。 |
| <a href="#bidi_t_vis_str_size">vis\_str\_size</a> | uint32\_t | 用于显示的字符串长度(存放log2vis的结果)。 |
| <a href="#bidi_t_vis_strx">vis\_strx</a> | wchar\_t* | 用于显示的字符串(存放log2vis的结果)。 |
#### bidi\_deinit 函数
-----------------------

* 函数功能：

> <p id="bidi_t_bidi_deinit">释放bidi对象相关资源。

* 函数原型：

```
ret_t bidi_deinit (bidi_t* bidi);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bidi | bidi\_t* | bidi对象。 |
#### bidi\_init 函数
-----------------------

* 函数功能：

> <p id="bidi_t_bidi_init">初始化bidi对象。

* 函数原型：

```
bidi_t* bidi_init (bidi_t* bidi, bool_t alloc_l2v, bool_t alloc_v2l, bidi_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bidi\_t* | 返回bidi对象。 |
| bidi | bidi\_t* | bidi对象。 |
| alloc\_l2v | bool\_t | 是否为positions\_L\_to\_V分配空间。 |
| alloc\_v2l | bool\_t | 是否为positions\_V\_to\_L分配空间。 |
| type | bidi\_type\_t | 类型。 |
#### bidi\_log2vis 函数
-----------------------

* 函数功能：

> <p id="bidi_t_bidi_log2vis">将字符串转成用于显示的字符串，输出结果放在bidi->vis_str中。

* 函数原型：

```
ret_t bidi_log2vis (bidi_t* bidi, const wchar_t* str, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bidi | bidi\_t* | bidi对象。 |
| str | const wchar\_t* | 字符串。 |
| size | uint32\_t | 字符串的长度。 |
#### bidi\_type\_from\_name 函数
-----------------------

* 函数功能：

> <p id="bidi_t_bidi_type_from_name">将bidi类型的名称转换成类型。

* 函数原型：

```
bidi_t* bidi_type_from_name (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bidi\_t* | 返回bidi对象。 |
| name | const char* | 类型名称(取值：rtl,ltr,auto,wrtl,wltr,lro,rlo)。 |
#### positions\_L\_to\_V 属性
-----------------------
> <p id="bidi_t_positions_L_to_V">logical位置与visual位置的映射。

* 类型：int32\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### positions\_V\_to\_L 属性
-----------------------
> <p id="bidi_t_positions_V_to_L">visual位置与logical位置的映射。

* 类型：int32\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### request\_type 属性
-----------------------
> <p id="bidi_t_request_type">请求的类型。

* 类型：bidi\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### resolved\_type 属性
-----------------------
> <p id="bidi_t_resolved_type">实际的类型。

* 类型：bidi\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### vis\_str\_size 属性
-----------------------
> <p id="bidi_t_vis_str_size">用于显示的字符串长度(存放log2vis的结果)。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### vis\_strx 属性
-----------------------
> <p id="bidi_t_vis_strx">用于显示的字符串(存放log2vis的结果)。

* 类型：wchar\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
