# 为 512K Flash 优化空间

## 背景

* 缺省情况下，demo1 在 STMF103 上的空间占用情况如下。

```
Program Size: Code=222780 RO-data=232212 RW-data=720 ZI-data=53440  
```

对于只用 512K Flash 的平台来说，给应用程序留下的空间就比较少了，这里介绍一下进一步优化空间方法：

## AWTK 优化

* 1. 将编译器的优化级别提高一级 (O1):

可以优化掉 30K 左右的空间：

```
Program Size: Code=193658 RO-data=232214 RW-data=720 ZI-data=53440  
```

* 2. 去掉 Unicode 换行算法的支持。

修改 awtk\_config.h 文件，去掉 WITH\_UNICODE\_BREAK 的定义。

```c
//#define WITH_UNICODE_BREAK 1
```

可以优化掉 50K 左右的空间：

```
Program Size: Code=190338 RO-data=186674 RW-data=400 ZI-data=53440  
```

* 3. 去掉不必要的 bitmap 格式

对于 BGR565 格式的 LCD，修改 awtk\_config.h 文件，增加下面的宏定义

```c
/**
 * 对于只有 512K flash 的平台，而且 LCD 格式是 BGR565。如果希望进一步优化空间，去掉多余的 bitmap 格式支持代码。请定义本宏。
 * 其它 LCD 格式，可以自行修改：src/blend/soft_g2d.c 保留需要的格式即可。
 *
 */
#define LCD_BGR565_LITE 1
```

可以优化掉 30K 左右的空间：

```
Program Size: Code=163602 RO-data=186498 RW-data=400 ZI-data=53440  
```

* 4. 将编译器的优化级别提高一级 (O2):

可以优化掉 10K 左右的空间：

```
Program Size: Code=149510 RO-data=186494 RW-data=400 ZI-data=53440  
```

* 5. 去掉不需要的控件：

最多可以优化掉 20K 左右的空间

## APP 优化

* 设计界面时不要使用 inline 样式。使用共享样式，可以减少 RAM 和 Flash 的空间。
