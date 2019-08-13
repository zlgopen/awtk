# 字体工具

## 点阵字体生成工具

fontgen从指定的tff文件，提取指定字符集(从文件中读取)的glyph，生成C常量文件。

```
./bin/fontgen ttf_filename str_filename output_filename font_size [mono]
```

* ttf\_filename tff文件
* str\_filename 字符集合(UTF-8)编码
* output\_filename 输出的文件
* font\_size 字体大小
* mono是否生成单色字体，目前只有启用freetype时才有效。可选。

## 从TTF字体文件中提取部分字体

可以使用fonttools中的pyftsubset工具提取：

安装：
```
pip install fonttools
```

示例：

```
pyftsubset ../../../fonts/msyh.ttf --text-file=../../demos/assets/raw/fonts/text.txt  --output-file=../../demos/assets/raw/fonts/default.mini.ttf
```

