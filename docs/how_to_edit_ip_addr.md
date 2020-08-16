
# 如何用 edit 控件编辑 IP 地址

用 edit 控件编辑 IP 地址，将 edit 的 input\_type 指定为"ipv4"即可。

* 基本用法

示例：

```xml
 <edit w="70%" text="192.168.1.1" tips="ipv4" input_type="ipv4"/>
```

* 启用自动修正功能

示例：

```xml
<edit w="70%" text="192.168.1.1" tips="ipv4 autofix" input_type="ipv4" auto_fix="true"/>
```

参考：[input type](manual/input_type_t.md)
