# 如何启用文本双向排版 (Unicode Bidirectional Algorithm)

## 1. 启用文本双向排版算法

定义宏 WITH\_TEXT\_BIDI 即可，嵌入式版本请修改 awtk_config.h:

```c
/**
 * 如果希望支持文字双向排版算法（如阿拉伯语言），请定义本宏。
 * 
 * #define WITH_TEXT_BIDI 1
 * 
 */
 ```

>PC　版本默认已经定义。

 ### 2. 加入相关文件


