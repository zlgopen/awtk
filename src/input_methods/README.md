# 注意事项

目前 designer 生成项目没有加入输入法数据和键盘数据，需要手动修改。

* 1. 将下面的文件从 awtk 目录拷贝到自己项目对应的目录。

```
design/default/ui/kb_*.xml
design/default/styles/keyboard.xml
design/default/data/suggest_words_zh_cn.dat
```

* 2. 如果启用 google 拼音输入法，还要将下面的文件从 awtk 目录拷贝到自己项目对应的目录。

```
design/default/data/gpinyin.dat
``

* 3. 然后重新生成资源文件。