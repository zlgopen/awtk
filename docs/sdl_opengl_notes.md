#SDL + OpenGL使用笔记

[AWTK](https://github.com/zlgopen/awtk) 是一个嵌入式GUI，为了开发方便，需要提供PC运行环境。我选择了[SDL2](https://www.libsdl.org)+OpenGL+[nanovg](https://github.com/memononen/nanovg)来实现底层的渲染，让[AWTK](https://github.com/zlgopen/awtk)可以运行在各个平台上。[GLFW](http://www.glfw.org/)+OpenGL也是一个不错的选择，但是GLFW没有Android和iOS的移植，而且没有提供原生输入法的支持。[AWTK](https://github.com/zlgopen/awtk)虽然最初是为嵌入式系统而生，但也有一个小目标：可以用于开发嵌入式系统，也可以开发PC软件和移动APP，所以最后选择了SDL2+OpenGL+nanovg。在使用SDL2+OpenGL+nanovg的过程中，踩了一些坑，这里做个笔记，给需要的朋友参考：

## 一、在MacPro上显示模糊的问题。

在网上查了一下，有人提供的方案是设置SCALE_QUALITY，貌似也有些道理，但是效果不佳。

```
SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
```

花了一些时间去看SDL的源码后，发现其实SDL在创建窗口时提供了SDL\_WINDOW\_ALLOW\_HIGHDPI标志，可以用来解决模糊的问题：

```
SDL_CreateWindow("AWTK Simulator", x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
```

如果设置了SDL\_WINDOW\_ALLOW\_HIGHDPI标志，窗口大小和Drawable的大小可能不一致，在绘图时需要做相应缩放：

```
SDL_GetWindowSize(sdl_window, &ww, &wh);
SDL_GL_GetDrawableSize(sdl_window, &fw, &fh);
ratio = (float)fw / (float)ww;
```

## 二、nanovg裁剪算法无效。

使用低级的OpenGL去绘图是一个比较麻烦的事情。在[AWTK](https://github.com/zlgopen/awtk)中，采用了[nanovg](https://github.com/memononen/nanovg)矢量图绘图函数库，nanovg缺省使用的[GLFW](http://www.glfw.org/)，要移植到[SDL2](https://www.libsdl.org)上也不难。但是我发现nanovg的示例在SDL上和在GLFW上的效果有些差异，仔细观察后，初步判断与用stencil进行裁剪有关，以为是没有启用stencil测试引起的，于是加了下面的代码：

```
glEnable(GL_STENCIL_TEST);
```

但是没有效果，在nanovg的代码中，也发现它自己会启用stencil测试。对比基于nanovg基于GLFW的例子，也没发现有价值的线索。然后对比SDL\_CreateWindow/glfwCreateWindow和SDL\_Init/glfwInit的实现，发现SDL中，stencil\_size的缺省值是0，尝试把设置它为8：

```
SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
```

显示正常了。

## 三、Windows下的OpenGL

在MacPro和Ubuntu下测试没有问题，在Window测试时发现一些OpenGL函数找不到，明明已经链接了opengl32.dll啊。网上的方案多是基于GLUT和GLFW在Windows下做OpenGL开发的，SDL则没有找到相关资料，只好再去研读GLFW，看能不能从中借用部分代码。很快发现deps/glad是干这个的，而glad是[http://glad.dav1d.de/](http://glad.dav1d.de/)上在线生成的。把glad.c加入项目中，并调用初始化函数：

```
gladLoadGL();
```

编译没有问题了，不过运行起来就崩掉了。调试时发现glGetString(GL\_VERSION)返回NULL，这个简单的函数居然会失败！后来在google搜索
到glGetString(GL\_VERSION)失败的原因: gladLoadGL需要放在SDL\_GL\_CreateContext之后调用。而我把它放在了SDL\_Init和SDL\_GL\_CreateContext之间了。


