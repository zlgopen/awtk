# 矢量图绘图接口的实现

目前有3类实现：

* vgcanvas\_nanovg\_soft 软件实现(**适合普通嵌入式平台**)。软件实现又有agg和agge两种实现。

* vgcanvas\_nanovg\_gl OpenGL实现。OpenGL实现又有OpenGL3、OpenGLES2和OpenGLES3三种实现。

* vgcanvas\_null 空实现。极低端平台不支持矢量图绘图函数，使用本实现。
