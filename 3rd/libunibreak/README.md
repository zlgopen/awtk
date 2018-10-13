
> 在原始项目中emojidata.c/graphemebreakdata.c/wordbreakdata.c 是直接include进来的，很容被多次引用造成符号重复，所以把它们改名为对应的.inc文件。

