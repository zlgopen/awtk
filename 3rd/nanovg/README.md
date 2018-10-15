# nanovg的backend

nanovg支持下列backend:

* 1.gl 基于OpenGL/GLES实现(nanovg内置)。

* 2.agg 基于agg实现。纯软件实现，渲染效果好，速度较慢，适合没有GPU的嵌入平台。(注意：目前不支持565格式的图片，请勿定义WITH\_BITMAP\_BGR565)。

* 3.agge 基于agg实现。纯软件实现，渲染效果一般，速度较agg快，适合没有GPU的嵌入平台。

* 4.bgfx 基于bgfx实现。支持多种渲染方式(OpenGL/metal/vulkan/DirectX)，推荐在Android、iOS、Linux、MacOS、Windows等平台上使用。


