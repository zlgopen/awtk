# 如何加入中文输入法

在示例项目中，没有加入输入法，主要是开发板的 flash 不够。如果 flash 够大（不小于 4M 时），可以自行加入：

* 加入 3rd/gpinyin/src 中的代码。
* 加入 src/input\_engines/input\_engine\_pinyin.cpp。
* 去掉 src/input\_engines/input\_engine\_null.cpp。
* include 路径加入 3rd/gpinyin/include。
* 把 default\_full.ttf 拷贝到 default.tff，重新生成资源并编译 (defualt.ttf 中没有汉字，default\_full.ttf 里才有，所以需要切换字体，否则输入的字符无法显示）。
