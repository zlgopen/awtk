# 字符串工具

## 国际化字符串转换工具

将 XML 格式的国际化字符串转换成二进制格式。

## 用法

```
Usage: bin/strgen input outputdir [bin] [theme]
```

* input 输入文件。一般为资源目录下的 strings.xml。
* ouputdir 输出的目录。二进制数据放到 raw 目录下，常量数据放到 inc 目录下。
* bin 表示生成 bin 文件，否则生成编译到代码中的常量数据。
* theme 一般不用指定，使用缺省主题即可。

示例：

### 生成 bin 文件

> 一般 PC 或有文件系统的情况使用生成的 bin 文件，在没有文件系统的情况下使用生成常量数据，编译到代码中。一般同时生成两种数据，供 PC 模拟和嵌入式运行使用。

* 生成二进制数据。

```
bin/strgen demos/assets/default/raw/strings/strings.xml demos/assets/default/raw/strings bin
```

* 生成 C 代码常量。

```
bin/strgen demos/assets/default/raw/strings/strings.xml demos/assets/default/inc/strings           
```

> 本文以Linux/MacOS为例，Windows可能会微妙差异，请酌情处理。