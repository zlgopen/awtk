## nanovg支持下列backend:

* 1.gl 基于OpenGL/GLES实现(nanovg内置)，由宏WITH\_NANOVG\_GL决定。

* 2.agge 基于agge实现。纯软件实现，渲染效果一般，速度较agg快，适合没有GPU的嵌入平台。由宏WITH\_NANOVG\_AGGE决定。


