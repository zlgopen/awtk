## 32 位色转 16 位色生成工具

在嵌入式系统中，为了节省带宽屏幕往往使用 16 位色屏幕，但是而在 PC 上面的图片一般为 32 位色的图片，而把 32 位色的图片直接放到 16 位色的屏幕中显示会有可能会出现色块阶梯的情况，为了解决这个情况，使用该工具把 32 位色的图片转为 16 位色的图片，保证在 PC 上面看到的图片颜色和板子上面看到的颜色一致。

用法说明：

```
./bin/image_dither_gen in_filename out_filename (png|res|data) (bgra|rgba|bgr565|rgb565)
```

* in\_filename png/jpg 文件的文件名。
* out\_filename 保存文件的文件名。
* (png|res|data) 选择其中一个资源格式类型，其中 png 和 res 只能保存 RGBA 或者 BGRA 的图片格式。
* png 表示保存的文件格式为 png 格式
* res 表示保存的文件格式为 res 格式
* data 表示保存的文件格式为 data 格式
* (bgra|rgba|bgr565|rgb565) 选择其中一个转换后的图片格式。
* bgra 表示输出 BITMAP\_FMT\_BGRA8888 格式的图片。
* rgba 表示输出 BITMAP\_FMT\_RGBA8888 格式的图片。
* bgr565 表示把不透明的图片转成 BITMAP\_FMT\_BGR565 格式的图片。
* rgb565 表示把不透明的图片转成 BITMAP\_FMT\_RGB565 格式的图片。

> 注意：
>
> 1. 因为 jpg 格式是有损压缩会导致抖动算法的颜色改变，导致失去抖动算法的效果，所以这里没有保存 jpg 格式的选项。
> 2. 如果 in\_filename 的图片为 png 图片，并且存在半透明或者全透明的地方，同时输出个图片格式为 bgr565 或者 rgb565 的话，半透明或者全透明的地方颜色表达会有问题。

