# 键值映射

AWTK 控件支持键值映射，也就是将一个键值转换成另外一个键值。

有的硬件平台只有四个方向和其它少数几个键。比如在 listview 界面，希望按 up/down 键时执行翻页的功能，此时可以把 up 键映射 pageup，把 down 映射成 pagedown。

如：

```
<list_view x="0"  y="0" w="100%" h="-50" item_height="60" map_key:up="pageup" map_key:down="pagedown">
```

> 键值映射可以放到任何控件，请根据自己的需要调整。

> 键名的定义请参考 [enums.c](https://github.com/zlgopen/awtk/blob/master/src/base/enums.c#L99)。
