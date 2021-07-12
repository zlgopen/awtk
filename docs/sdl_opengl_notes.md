#SDL + OpenGL 使用笔记

[AWTK](https://github.com/zlgopen/awtk) 是一个嵌入式 GUI，为了开发方便，需要提供 PC 运行环境。我选择了 [SDL2](https://www.libsdl.org)+OpenGL+[nanovg](https://github.com/memononen/nanovg) 来实现底层的渲染，让 [AWTK](https://github.com/zlgopen/awtk) 可以运行在各个平台上。[GLFW](http://www.glfw.org/)+OpenGL 也是一个不错的选择，但是 GLFW 没有 Android 和 iOS 的移植，而且没有提供原生输入法的支持。[AWTK](https://github.com/zlgopen/awtk) 虽然最初是为嵌入式系统而生，但也有一个小目标：可以用于开发嵌入式系统，也可以开发 PC 软件和移动 APP，所以最后选择了 SDL2+OpenGL+nanovg。在使用 SDL2+OpenGL+nanovg 的过程中，踩了一些坑，这里做个笔记，给需要的朋友参考：

## 一、在 MacPro 上显示模糊的问题。

在网上查了一下，有人提供的方案是设置 SCALE_QUALITY，貌似也有些道理，但是效果不佳。

```
SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
```

花了一些时间去看 SDL 的源码后，发现其实 SDL 在创建窗口时提供了 SDL\_WINDOW\_ALLOW\_HIGHDPI 标志，可以用来解决模糊的问题：

```
SDL_CreateWindow("AWTK Simulator", x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
```

如果设置了 SDL\_WINDOW\_ALLOW\_HIGHDPI 标志，窗口大小和 Drawable 的大小可能不一致，在绘图时需要做相应缩放：

```
SDL_GetWindowSize(sdl_window, &ww, &wh);
SDL_GL_GetDrawableSize(sdl_window, &fw, &fh);
ratio = (float)fw / (float)ww;
```

## 二、nanovg 裁剪算法无效。

使用低级的 OpenGL 去绘图是一个比较麻烦的事情。在 [AWTK](https://github.com/zlgopen/awtk) 中，采用了 [nanovg](https://github.com/memononen/nanovg) 矢量图绘图函数库，nanovg 缺省使用的 [GLFW](http://www.glfw.org/)，要移植到 [SDL2](https://www.libsdl.org) 上也不难。但是我发现 nanovg 的示例在 SDL 上和在 GLFW 上的效果有些差异，仔细观察后，初步判断与用 stencil 进行裁剪有关，以为是没有启用 stencil 测试引起的，于是加了下面的代码：

```
glEnable(GL_STENCIL_TEST);
```

但是没有效果，在 nanovg 的代码中，也发现它自己会启用 stencil 测试。对比基于 nanovg 基于 GLFW 的例子，也没发现有价值的线索。然后对比 SDL\_CreateWindow/glfwCreateWindow 和 SDL\_Init/glfwInit 的实现，发现 SDL 中，stencil\_size 的缺省值是 0，尝试把设置它为 8：

```
SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
```

显示正常了。

## 三、Windows 下的 OpenGL

在 MacPro 和 Ubuntu 下测试没有问题，在 Window 测试时发现一些 OpenGL 函数找不到，明明已经链接了 opengl32.dll 啊。网上的方案多是基于 GLUT 和 GLFW 在 Windows 下做 OpenGL 开发的，SDL 则没有找到相关资料，只好再去研读 GLFW，看能不能从中借用部分代码。很快发现 deps/glad 是干这个的，而 glad 是 [http://glad.dav1d.de/](http://glad.dav1d.de/) 上在线生成的。把 glad.c 加入项目中，并调用初始化函数：

```
gladLoadGL();
```

编译没有问题了，不过运行起来就崩掉了。调试时发现 glGetString(GL\_VERSION) 返回 NULL，这个简单的函数居然会失败！后来在 google 搜索
到 glGetString(GL\_VERSION) 失败的原因：gladLoadGL 需要放在 SDL\_GL\_CreateContext 之后调用。而我把它放在了 SDL\_Init 和 SDL\_GL\_CreateContext 之间了。
