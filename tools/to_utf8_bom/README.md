# utf8 to utf8 with bom

在UTF8文件前加上UTF-8 BOM的标志。

用法：

```
find src -name \*.h -exec node tools/to_utf8_bom/index.js {} \;
```
