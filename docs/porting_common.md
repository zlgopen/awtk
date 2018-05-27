## 嵌入式平台移植注意事项

### 一、几个常用的宏

* 1.WITH\_BITMAP\_FONT 如果需要使用位图字体请定义该宏。

* 2.WITH\_STB\_FONT 如果需要使用truetype字体请定义该宏。

* 3.WITH\_STB\_IMAGE 如果需要使用png/jpg图片请定义该宏。由于解码图片需要较大内存，所以在内存较
少的情况下，我们直接把png/jpg转换成位图数据，当作常量编译到代码中。 

* 4.WITH\_FS\_RES 如果支持从文件系统中加载资源请定义该宏。需要有兼容POSIX标准的FS接口，如果没有可以自己实现src/base/fs.c中的函数。


### 二、include路径

* 1.基本路径

```
awtk
awtk\src
awtk\3rd
```

* 2.picasso的路径(如果启用)

```
awtk\3rd\picasso\src
awtk\3rd\picasso\src\gfx
awtk\3rd\picasso\include
awtk\3rd\picasso\src\include
awtk\3rd\picasso\build
```

* 3.agg的路径(如果启用)

```
```

### 三、加入哪些文件到工程

### 四、其它

> 使用keil编译时，请在C/C++ -> Misc Controls中加上--gnu标志。



