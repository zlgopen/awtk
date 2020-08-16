# 如何更新拼音输入法字典和联想字库

有的情况下，可能需要自己更新拼音输入法字典和联想词库。比如：

* 使用更好的字典数据。
* 去掉一些不需要的汉字。
* 使用更完善的联想字库。

## 1.更新拼音输入法字典

### 1.1 根据自己的需要编辑下列文件：

* 3rd/gpinyin/data/rawdict\_utf16\_65105\_freq.txt

* 3rd/gpinyin/data/valid\_utf16.txt

### 1.2 重新生成数据

```
./bin/gpinyingen

```

### 1.3 拷贝3rd/gpinyin/data/gpinyin.dat到资源目录的data目录。

```
cp 3rd/gpinyin/data/gpinyin.dat design/default/data/gpinyin.dat
```

## 2.更新联想词库

联想词库是抓取网页，通过分词工具自动生成的。请参考：tools/word_gen/README.md



