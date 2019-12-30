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
