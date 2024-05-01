# bgfx

使用bgfx需要手工去下列软件包：

* bx：https://github.com/bkaradzic/bx.git 
* bimg：https://github.com/bkaradzic/bimg.git 
* bgfx: https://github.com/bkaradzic/bgfx.git

```
git clone https://github.com/bkaradzic/bx.git 
git clone https://github.com/bkaradzic/bimg.git 
git clone https://github.com/bkaradzic/bgfx.git

```

已知问题：


* linux

src/config.h

```patch
 #      ifndef BGFX_CONFIG_RENDERER_DIRECT3D11
 #              define BGFX_CONFIG_RENDERER_DIRECT3D11 (0 \
-                                       || BX_PLATFORM_LINUX          \
                                        || BX_PLATFORM_WINDOWS        \
                                        || BX_PLATFORM_WINRT          \
                                        || BX_PLATFORM_XBOXONE        \
@@ -46,7 +45,6 @@

 #      ifndef BGFX_CONFIG_RENDERER_DIRECT3D12
 #              define BGFX_CONFIG_RENDERER_DIRECT3D12 (0 \
-                                       || BX_PLATFORM_LINUX          \
                                        || BX_PLATFORM_WINDOWS        \
                                        || BX_PLATFORM_WINRT          \
                                        || BX_PLATFORM_XBOXONE        \
```
