# 为不同语言加载不同的字体文件

在支持多语言的时候，把所有语言的字体放在一个文件中，可能会导致资源文件过大，加载时间变长，对内存要求也很高。另外，有时也可能找不到支持多语言的字体文件。此时我们需要为不同的语言加载不同的字体文件。

比如，我们需要为中文加载中文字体，为英文加载英文字体。假设加载名为 default 的字体，AWTK 按下面的顺序查找字体资源：

* 1. 加载 default_{语言_国家/地区}。比如，简体中文加载 default\_zh\_CN，美国英文加载 default\_en\_US。
* 2. 加载 default_{语言}。比如，中文加载 default\_zh，英文加载 default\_en。
* 3. 加载 default

这样，在 UI 文件和样式文件中，我们可以使用 default 字体，而不用关心当前的语言是什么。AWTK 会自动加载合适的字体文件。
