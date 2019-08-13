## 位图生成工具

在嵌入式系统中，RAM比较少，虽然PNG/JPG等文件可以大幅度降低存储空间，但是需要解码到内存中，不太适合直接使用。AWTK提供了imagegen工具，将图片转换成位图数据，直接编译到代码中，放在flash中，不占用内存空间。

用法说明：

```
./bin/imagegen in_filename out_filename bgra+bgr565|mono
```

* in\_filename png/jpg 文件的文件名。
* out\_filename 位图数据文件的文件名。
* bgra 表示输出BITMAP\_FMT\_BGRA8888格式的图片。
* bgr565 表示把不透明的图片转成BITMAP\_FMT\_BGR565格式的图片。
* mono 是否生成单色图片。
