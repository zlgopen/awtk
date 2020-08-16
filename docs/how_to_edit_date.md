
# 如何用 edit 控件编辑日期

用 edit 控件编辑日期，将 edit 的 input\_type 指定为"date"即可。

* 基本用法

示例：

```xml
 <edit w="70%" text="2020/10/10"   tips="date" input_type="date"/
```

* 启用自动修正功能

示例：

```xml
<edit w="70%" text="" tips="date autofix"  input_type="date" auto_fix="true"/>
```

参考：[input type](manual/input_type_t.md)
