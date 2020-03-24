# 去掉UTF-8前的BOM

有的编译器不支持UTF-8 BOM，此时需要取到去掉UTF-8 BOM文件的BOM.

用法(bash)：

```
find src -name \*.h -exec node tools/remove_utf8_bom/index.js {} \;
```

> 请安装[nodejs](https://nodejs.org)。