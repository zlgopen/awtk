## candidates\_t
### 概述
![image](images/candidates_t_0.png)

输入法候选字词控件。

如果希望启用用数字选择对应的候选字，请设置属性grab_keys="true"。如：

```xml
<candidates x="0" y="0" w="100%" h="30" grab_keys="true"/>
```

>相关文件： assets/default/raw/ui/kb_default.xml

如果希望通过左右键切换不同的候选字，除了设置属性grab_keys="true"，还需要设置按钮的focused状态的style。

```xml
<style name="candidates" text_color="black">
<normal  />
<pressed    bg_color="#c0c0c0" border_color="#a0a0a0"/>
<over       bg_color="#e0e0e0" border_color="#a0a0a0"/>
<focused    border_color="#a0a0a0"/>
</style>
```

>相关文件：assets/default/raw/styles/keyboard.xml
----------------------------------
### 函数
<p id="candidates_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#candidates_t_candidates_cast">candidates\_cast</a> | 转换为candidates对象(供脚本语言使用)。 |
| <a href="#candidates_t_candidates_create">candidates\_create</a> | 创建candidates对象 |
| <a href="#candidates_t_candidates_set_auto_hide">candidates\_set\_auto\_hide</a> | 设置是否自动隐藏。 |
| <a href="#candidates_t_candidates_set_button_style">candidates\_set\_button\_style</a> | 设置按钮的style名称。 |
| <a href="#candidates_t_candidates_set_pre">candidates\_set\_pre</a> | 设置是否为预候选字列表。 |
| <a href="#candidates_t_candidates_set_select_by_num">candidates\_set\_select\_by\_num</a> | 设置是否启用用数字选择候选字。 |
### 属性
<p id="candidates_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#candidates_t_auto_hide">auto\_hide</a> | bool\_t | 没有候选字时，是否自动隐藏控件。 |
| <a href="#candidates_t_button_style">button\_style</a> | char* | 按钮的style名称。 |
| <a href="#candidates_t_pre">pre</a> | bool\_t | 是否为预候选字。 |
| <a href="#candidates_t_select_by_num">select\_by\_num</a> | bool\_t | 是否启用用数字选择候选字。比如按下1选择第1个候选字，按下2选择第2个候选字。 |
#### candidates\_cast 函数
-----------------------

* 函数功能：

> <p id="candidates_t_candidates_cast">转换为candidates对象(供脚本语言使用)。

* 函数原型：

```
widget_t* candidates_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | candidates对象。 |
| widget | widget\_t* | candidates对象。 |
#### candidates\_create 函数
-----------------------

* 函数功能：

> <p id="candidates_t_candidates_create">创建candidates对象

* 函数原型：

```
widget_t* candidates_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
#### candidates\_set\_auto\_hide 函数
-----------------------

* 函数功能：

> <p id="candidates_t_candidates_set_auto_hide">设置是否自动隐藏。

* 函数原型：

```
ret_t candidates_set_auto_hide (widget_t* widget, bool_t auto_hide);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| auto\_hide | bool\_t | 是否自动隐藏。 |
#### candidates\_set\_button\_style 函数
-----------------------

* 函数功能：

> <p id="candidates_t_candidates_set_button_style">设置按钮的style名称。

* 函数原型：

```
ret_t candidates_set_button_style (widget_t* widget, const char* button_style);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| button\_style | const char* | 按钮的style名称。 |
#### candidates\_set\_pre 函数
-----------------------

* 函数功能：

> <p id="candidates_t_candidates_set_pre">设置是否为预候选字列表。

> 为预候选字列表则注册EVT\_IM\_SHOW\_PRE\_CANDIDATES，否则注册EVT\_IM\_SHOW\_CANDIDATES事件。

* 函数原型：

```
ret_t candidates_set_pre (widget_t* widget, bool_t pre);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| pre | bool\_t | 是否为预候选字列表。 |
#### candidates\_set\_select\_by\_num 函数
-----------------------

* 函数功能：

> <p id="candidates_t_candidates_set_select_by_num">设置是否启用用数字选择候选字。

* 函数原型：

```
ret_t candidates_set_select_by_num (widget_t* widget, bool_t select_by_num);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| select\_by\_num | bool\_t | 是否启用用数字选择候选字。 |
#### auto\_hide 属性
-----------------------
> <p id="candidates_t_auto_hide">没有候选字时，是否自动隐藏控件。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### button\_style 属性
-----------------------
> <p id="candidates_t_button_style">按钮的style名称。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### pre 属性
-----------------------
> <p id="candidates_t_pre">是否为预候选字。

> 预候选字: 在有的输入法中，比如T9硬键盘输入时，按下12两个键时，预候选字会显示可用的拼音列表。
> 从预候选字列表中选择拼音，再查询拼音对应的候选字列表。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### select\_by\_num 属性
-----------------------
> <p id="candidates_t_select_by_num">是否启用用数字选择候选字。比如按下1选择第1个候选字，按下2选择第2个候选字。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
