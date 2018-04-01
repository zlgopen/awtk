## 位图生成工具

在嵌入式系统中，RAM比较少，虽然PNG/JPG等文件可以大幅度降低存储空间，但是需要解码到内存中，不太适合直接使用。LFTK提供了imagegen工具，将图片转换成位图数据，直接编译到代码中，放在flash中，不占用内存空间。

```
./bin/imagegen in_filename out_filename
```

* in_filename png/jpg 文件
* out_filename 位图数据文件