## shortcut\_t
### 概述
快捷键。
----------------------------------
### 函数
<p id="shortcut_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#shortcut_t_shortcut_equal">shortcut\_equal</a> | 比较两个快捷键对象。 |
| <a href="#shortcut_t_shortcut_init">shortcut\_init</a> | 初始化快捷键对象。 |
| <a href="#shortcut_t_shortcut_init_with_str">shortcut\_init\_with\_str</a> | 初始化快捷键对象。 |
| <a href="#shortcut_t_shortcut_match">shortcut\_match</a> | 检查事件与快捷键是否匹配。 |
### 属性
<p id="shortcut_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#shortcut_t_alt">alt</a> | bool\_t | alt键是否按下。 |
| <a href="#shortcut_t_cmd">cmd</a> | bool\_t | cmd/win键是否按下。 |
| <a href="#shortcut_t_ctrl">ctrl</a> | bool\_t | ctrl键是否按下。 |
| <a href="#shortcut_t_is_valid">is\_valid</a> | bool\_t | 是否有效。 |
| <a href="#shortcut_t_key">key</a> | uint32\_t | 键值。 |
| <a href="#shortcut_t_lalt">lalt</a> | bool\_t | left alt键是否按下。 |
| <a href="#shortcut_t_lctrl">lctrl</a> | bool\_t | left ctrl键是否按下。 |
| <a href="#shortcut_t_lshift">lshift</a> | bool\_t | left shift键是否按下。 |
| <a href="#shortcut_t_menu">menu</a> | bool\_t | menu键是否按下。 |
| <a href="#shortcut_t_ralt">ralt</a> | bool\_t | right alt键是否按下。 |
| <a href="#shortcut_t_rctrl">rctrl</a> | bool\_t | right ctrl键是否按下。 |
| <a href="#shortcut_t_rshift">rshift</a> | bool\_t | right shift键是否按下。 |
| <a href="#shortcut_t_shift">shift</a> | bool\_t | shift键是否按下。 |
#### shortcut\_equal 函数
-----------------------

* 函数功能：

> <p id="shortcut_t_shortcut_equal">比较两个快捷键对象。

* 函数原型：

```
bool_t shortcut_equal (shortcut_t* shortcut1, shortcut_t* shortcut2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示相同，否则表示不同。 |
| shortcut1 | shortcut\_t* | 快捷键对象1。 |
| shortcut2 | shortcut\_t* | 快捷键对象2。 |
#### shortcut\_init 函数
-----------------------

* 函数功能：

> <p id="shortcut_t_shortcut_init">初始化快捷键对象。

* 函数原型：

```
shortcut_t* shortcut_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | shortcut\_t* | 返回快捷键对象。 |
#### shortcut\_init\_with\_str 函数
-----------------------

* 函数功能：

> <p id="shortcut_t_shortcut_init_with_str">初始化快捷键对象。

* 函数原型：

```
shortcut_t* shortcut_init_with_str (shortcut_t* shortcut);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | shortcut\_t* | 返回快捷键对象。 |
| shortcut | shortcut\_t* | 快捷键对象。 |
#### shortcut\_match 函数
-----------------------

* 函数功能：

> <p id="shortcut_t_shortcut_match">检查事件与快捷键是否匹配。

* 函数原型：

```
bool_t shortcut_match (shortcut_t* filter, shortcut_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示匹配，否则表示不匹配。 |
| filter | shortcut\_t* | 过滤器快捷键对象。 |
| event | shortcut\_t* | 事件快捷键对象。 |
#### alt 属性
-----------------------
> <p id="shortcut_t_alt">alt键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### cmd 属性
-----------------------
> <p id="shortcut_t_cmd">cmd/win键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### ctrl 属性
-----------------------
> <p id="shortcut_t_ctrl">ctrl键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### is\_valid 属性
-----------------------
> <p id="shortcut_t_is_valid">是否有效。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### key 属性
-----------------------
> <p id="shortcut_t_key">键值。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lalt 属性
-----------------------
> <p id="shortcut_t_lalt">left alt键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lctrl 属性
-----------------------
> <p id="shortcut_t_lctrl">left ctrl键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lshift 属性
-----------------------
> <p id="shortcut_t_lshift">left shift键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### menu 属性
-----------------------
> <p id="shortcut_t_menu">menu键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### ralt 属性
-----------------------
> <p id="shortcut_t_ralt">right alt键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### rctrl 属性
-----------------------
> <p id="shortcut_t_rctrl">right ctrl键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### rshift 属性
-----------------------
> <p id="shortcut_t_rshift">right shift键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### shift 属性
-----------------------
> <p id="shortcut_t_shift">shift键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
