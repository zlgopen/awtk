# 类似函数调用的参数格式

在AWTK的XML UI中，不少地方使用了类似函数调用的参数格式，常见用途有：

* 控件动画。如：

```
  <progress_bar name="value" x="center" y="middle" w="90%" h="40" 
    animation="value(from=50, to=100, yoyo_times=1000, duration=1000, delay=4000)"/>
```

* 窗口动画。如：

```
<dialog anim_hint="popup" highlight="default(start_alpha=0, end_alpha=80)"
```
* 布局参数。如：

```
<row  x="10" y="bottom:30" w="100%" h="90" children_layout="default(row=1,col=3)">
```

**基本格式为：**

**函数名**后跟一对括号，括号中有一个或多个**参数**，**参数**之间用英文','分隔，**参数名**和**参数值**之间用'='分隔。如果没有**参数**可以只写**函数名**。


用不太正规的语法描述为：

```
func_call => func_name  '('  params_list  ')' | func_name
params_list => params_list  ','  param | param
param => name  '='  value
```


