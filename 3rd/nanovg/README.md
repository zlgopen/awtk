## nanovg支持下列backend:

* 1.gl 基于OpenGL/GLES实现(nanovg内置)，由宏WITH\_NANOVG\_GL决定。

* 2.agge 基于agge实现。纯软件实现，渲染效果一般，速度较agg快，适合没有GPU的嵌入平台。由宏WITH\_NANOVG\_AGGE决定。

* 3.bgfx 基于bgfx实现。支持多种渲染方式(OpenGL/metal/vulkan/DirectX)，推荐在Android、iOS、Linux、MacOS、Windows等平台上使用。由宏WITH\_NANOVG\_BGFX决定。


