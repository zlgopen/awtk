# 字体工具

## 点阵字体生成工具

fontgen 从指定的 tff 文件，提取指定字符集（从文件中读取）的 glyph，生成 C 常量文件。

用法：

```
./bin/fontgen ttf_filename str_filename output_filename font_size [mono]
```

* ttf\_filename tff 文件名。
* str\_filename 字符集合 （需要哪些字符，就把那些字符放在本文件中，UTF-8 编码，参考 demos/assets/default/raw/fonts/text.txt)。
* output\_filename 输出文件名。如果文件扩展名为“.bin“，生成二进制格式，否则生成 C 语言常量数据。
* font\_size 字体大小。
* mono 是否生成单色字体，目前只有启用 freetype 时才有效。可选。

示例：

```
 ./bin/fontgen.exe demos/assets/default/raw/fonts/default_full.ttf  demos/assets/default/raw/fonts/text.txt demos/assets/default/inc/fonts/default_18.data  18
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
