# 字体工具

## 点阵字体生成工具

### 1. 功能
------------------
fontgen 从指定的 tff 文件，提取指定字符集（从文件中读取）的 glyph，生成点阵字体文件（二进制或者 C 语言常量数组）。

### 2. 用法

```
Usage: ./bin/fontgen ttf_filename str_filename out_filename font_size [mono|4bits]
```

* ttf\_filename tff 文件名。
* str\_filename 字符集合 （需要哪些字符，就把那些字符放在本文件中，UTF-8 编码，参考 demos/assets/default/raw/fonts/text.txt)。
* output\_filename 输出文件名。如果文件扩展名为“.bin“，生成二进制格式，否则生成 C 语言常量数据。
* font\_size 字体大小（像素）。
* mono 是否生成单色字体，目前只有启用 freetype 时才有效。可选。
* 4bits 每个点用4位表示(可选，默认8位)。

示例：

```
./bin/fontgen design/default/fonts/default_full.ttf test.txt test.bin 18
```

### 3. 文本文件格式
---------------
为了使用方便，在文本文件中，除了可以直接列出需要的字符外，也可以指定一个范围，或者混合两种用法。

* 范围用 [[和]] 括起来，中间用-分隔。

```
[[0-9]]
```

* 指定 ASCII 字符范围。下面的文本表示所有数字，小写字母和大写字母。

```
[[0-9]][[a-z]][[A-Z]]
```

* 用十进制指定 uncode 的范围。下面的文本表示"中丮丯"三个字符。

```
[[20013-20015]]
```

* 用十六进制指定 uncode 的范围。下面的文本表示"中丮丯"三个字符。

```
[[0x4e2d-0x4e2f]]
```
或

```
[[0X4e2d-0X4e2f]]
```

或

```
[[0X4E2D-0X4E2F]]
```

## 文本展开工具 

为了方便 fonttools 工具也支持指定范围，提供了一个小工具expand\_text。它将输入文件中的范围展开后写入输出文件。

```
Usage:./bin/expand_text input_file output_file
```

## 从 TTF 字体文件中提取部分字体

可以使用 fonttools 中的 pyftsubset 工具提取：

安装：
```
pip install fonttools
```

示例：

```
pyftsubset ../../../fonts/msyh.ttf --text-file=../../demos/assets/raw/fonts/text.txt  --output-file=../../demos/assets/raw/fonts/default.mini.ttf
```
