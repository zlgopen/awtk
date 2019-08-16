# 如何启用鼠标指针

## 一、demo启用鼠标指针

在demo\_main.c中有下列代码，要启用鼠标指针，只需要定义宏ENABLE\_CURSOR即可。

```
 #ifdef ENABLE_CURSOR
    window_manager_set_cursor(window_manager(), "cursor");
 #endif /*ENABLE_CURSOR*/
```

## 二、自己的应用程序启用鼠标指针

* 如果自己的应用程序没有走demo_main的流程，可以自己调用window\_manager\_set\_cursor或widget\_set\_cursor。

* 在任何时候，可以根据需要切换鼠标指针。

## 三、指针图片的要求

指针的形状有多种，比如普通指针、十字、文本选择和等待等等。为了简化处理，在绘制指针时，我们把图片的中心定位到鼠标的当前位置，在制作指针图片时，请考虑这一点。
