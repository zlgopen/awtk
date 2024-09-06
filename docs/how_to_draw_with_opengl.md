# 如何用 OpenGL 绘制图形

在有 GPU 的情况下，AWTK 使用 OpenGL 绘制图形。但是你会发现，如果自己在 paint 事件中使用 OpenGL 绘制图形，图形是无法显示的。原因是，AWTK 采用 nanovg 绘制图形，而 nanovg 并不是在绘制时立即执行的，而是在整个界面绘制完成 (EndFrame 中）集中提交给 GPU 执行的。所以，如果在 paint 事件中使用 OpenGL 绘制图形，这些图形会被 nanovg 绘制的图形覆盖掉。这里我们介绍一下，使用 OpenGL 绘制图形时的几个注意事项：

## 1. 需要提交前面的缓存图形

在使用 OpenGL 绘制之前，需要调用函数 vgcanvas_flush 刷新前面的缓存图形。

示例：

```c
static ret_t on_paint(void* ctx, event_t* e) {
  gl_info_t* info = &g_gl_info;
  paint_event_t* evt = paint_event_cast(e);
  vgcanvas_t* vg = canvas_get_vgcanvas(evt->c);
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);

  vgcanvas_flush(vg);

  glview_render_triangle(info, info->triangle1_vao);
  glview_render_triangle(info, info->triangle2_vao);

  return RET_OK;
}
```

## 2. 需要初始化 OpenGL

在 Windows 平台，需要调用 opengl_init 初始化 OpenGL。

示例：

```c
static ret_t glview_init(gl_info_t* info) {
#ifdef WIN32
  opengl_init();
#endif/*WIN32*/

  float triangle1_vertices[] = { 
      -0.5f, -0.5f, 0.0f, 
      0.5f,  -0.5f, 0.0f,
      0.0f,  0.5f,  0.0f
  };  

  float triangle2_vertices[] = { 
      -0.5f, -1.0f, 0.0f, 
      0.5f,  -1.0f, 0.0f,
      0.0f,  -0.5f, 0.0f
  };  

  info->program = glhelper_create_program(s_vertexShaderSource, s_fragmentShaderSource);
  info->triangle2_vao = glhelper_create_vao(triangle1_vertices, 3); 
  info->triangle1_vao = glhelper_create_vao(triangle2_vertices, 3); 

  return RET_OK;
}

```

## 3. 需要链接 glad 库

```
env.Program(os.path.join(BIN_DIR, 'demo_opengl'), ['opengl.c', 'assets.c'], LIBS=env['LIBS'] + ['glad'])
```

[完整示例请参考](https://gitee.com/zlgopen/awtk-c-demos/blob/master/demos/opengl.c)
