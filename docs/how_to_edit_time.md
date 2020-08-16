
# 如何用 edit 控件编辑时间

用 edit 控件编辑时间，将 edit 的 input\_type 指定为"time"即可。

* 基本用法

示例：

```xml
 <edit w="70%" text="12:16"  tips="time"  input_type="time"/>
```

* 启用自动修正功能

示例：

```xml
<edit w="70%" text="" tips="time autofix"   input_type="time" auto_fix="true"/>
```

> time 只能输入时分，要输入时分秒请用 time\_full。

参考：[input type](manual/input_type_t.md)
