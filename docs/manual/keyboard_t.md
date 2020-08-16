## keyboard\_t
### 概述
![image](images/keyboard_t_0.png)

软键盘。

软键盘是一个特殊的窗口，由编辑器通过输入法自动打开和关闭。

这里介绍一下定制软键盘的方法：

编辑器输入类型和软键盘UI资源文件的对应关系:

| 输入类型       | 软键盘UI资源文件|
|----------------|:---------------:|
| INPUT\_PHONE    | kb\_phone.xml    |
| INPUT\_INT      | kb\_int.xml      |
| INPUT\_FLOAT    | kb\_float.xml    |
| INPUT\_UINT     | kb\_uint.xml     |
| INPUT\_UFLOAT   | kb\_ufloat.xml   |
| INPUT\_HEX      | kb\_hex.xml      |
| INPUT\_EMAIL    | kb\_ascii.xml    |
| INPUT\_PASSWORD | kb\_ascii.xml    |
| INPUT\_CUSTOM   | 使用自定义的键盘 |
| 其它            | kb\_default.xml  |

keyboard中按钮子控件的名称有些特殊要求：

|  名称          | 功能            |
|----------------|:---------------:|
| return         | 回车键          |
| action         | 定制按钮        |
| backspace      | 删除键          |
| tab            | tab键           |
| space          | 空格键          |
| close          | 关闭软键盘       |
| back           | 关闭当前窗口     |
| back_to_home   | 返回home窗口    |
| 前缀key:        | 键值           |
| 前缀hard_key:   | 模拟物理键盘    |
| 前缀page:       | 切换到页面      |
| 前缀opt:        | 多个字符选择一个，点击切换到下一个，超时提交字符(用于实现九宫格输入) |

示例：

* 按键"a"，提交输入法处理。

```xml
<button repeat="300" name="key:a" text="a"/>
```

* 字符"a"，直接提交到编辑器。

```xml
<button repeat="300" name="a" text="a"/>
```

* 模拟物理键盘数字"1"，触发key down/up事件（可以用来选择候选字）。

```xml
<button repeat="300" name="hard_key:1" text="1"/>
```

* 九宫格输入

```xml
<button repeat="300" name="opt:._@/#" text="._@/#"/>
<button repeat="300" name="opt:abc" text="abc"/>
<button repeat="300" name="opt:def" text="def"/>
```


* 输入语言切换

有的输入法，同时支持输入多种语言。
比如T9，可以同时支持中文和英文输入，配合软键盘随时切换输入的语言。

可以在pages的页面里指定lang属性，切换到该页面时会设置输入法的语言。如：

```xml
<pages x="0" y="bottom" w="100%" h="-28" active="2">
<view name="lower" lang="en_us"
x="0" y="0" w="100%" h="100%" children_layout="default(r=4,c=4,s=2,m=2)">
...
</view>
<view name="chinese" lang="zh_cn"
x="0" y="0" w="100%" h="100%" children_layout="default(r=4,c=4,s=2,m=2)">
...
</view>
</pages>
```

> 更多用法请参考：
[kb_default](https://github.com/zlgopen/awtk/blob/master/design/default/ui/kb_default.xml)
----------------------------------
### 函数
<p id="keyboard_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#keyboard_t_keyboard_cast">keyboard\_cast</a> | 转换为keyboard对象(供脚本语言使用)。 |
| <a href="#keyboard_t_keyboard_close">keyboard\_close</a> | 关闭keyboard窗口。 |
| <a href="#keyboard_t_keyboard_create">keyboard\_create</a> | 创建keyboard对象 |
#### keyboard\_cast 函数
-----------------------

* 函数功能：

> <p id="keyboard_t_keyboard_cast">转换为keyboard对象(供脚本语言使用)。

* 函数原型：

```
widget_t* keyboard_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | keyboard对象。 |
| widget | widget\_t* | keyboard对象。 |
#### keyboard\_close 函数
-----------------------

* 函数功能：

> <p id="keyboard_t_keyboard_close">关闭keyboard窗口。

* 函数原型：

```
ret_t keyboard_close (widget_t* parent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| parent | widget\_t* | keyboard对象。 |
#### keyboard\_create 函数
-----------------------

* 函数功能：

> <p id="keyboard_t_keyboard_create">创建keyboard对象

* 函数原型：

```
widget_t* keyboard_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
