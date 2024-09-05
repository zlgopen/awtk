/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic demo
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"

#if defined(WITH_GPU_GL)

#ifdef WITHOUT_GLAD
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#ifdef IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/
#else
#include "glad/glad.h"
#endif /*WITHOUT_GLAD*/

/*本示例中，OpenGL代码源于：https://learnopengl.com */

static const char* s_vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char* s_fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

typedef struct _gl_info_t {
  uint32_t program;
  uint32_t fg_vao;
  uint32_t bg_vao;
} gl_info_t;

static uint32_t glhelper_create_program(const char* vertexShaderSource,
                                        const char* fragmentShaderSource) {
  int success;
  char infoLog[512];
  uint32_t shaderProgram = 0;
  uint32_t vertexShader = 0;
  uint32_t fragmentShader = 0;
  return_value_if_fail(vertexShaderSource != NULL && fragmentShaderSource != NULL, 0);

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    log_warn("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    log_warn("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    log_warn("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: %s\n", infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

static uint32_t glhelper_create_vao(void* vertices, uint32_t size) {
  uint32_t VBO = 0, VAO = 0;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return VAO;
}

static ret_t glview_init(gl_info_t* info) {
  float fg_vertices[] = {
      -0.5f, -0.5f, 0.0f, 
      0.5f,  -0.5f, 0.0f,
      0.0f,  0.5f,  0.0f
  };

  float bg_vertices[] = {
      -0.5f, -1.0f, 0.0f, 
      0.5f,  -1.0f, 0.0f,
      0.0f,  -0.5f, 0.0f
  };

  info->program = glhelper_create_program(s_vertexShaderSource, s_fragmentShaderSource);
  info->fg_vao = glhelper_create_vao(fg_vertices, 3);
  info->bg_vao = glhelper_create_vao(bg_vertices, 3);

  return RET_OK;
}

static ret_t glview_render(gl_info_t* info, uint32_t VAO) {
  uint32_t shaderProgram = info->program;

  if (shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(0);
    glBindVertexArray(0);
  }

  return RET_OK;
}

static ret_t glview_render_bg(void* ctx) {
  gl_info_t* info = (gl_info_t*)ctx;

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  return glview_render(info, info->bg_vao);
}

static ret_t glview_render_fg(void* ctx) {
  gl_info_t* info = (gl_info_t*)ctx;

  return glview_render(info, info->fg_vao);
}

static ret_t glview_deinit(gl_info_t* info) {
  glDeleteProgram(info->program);

  memset(info, 0x00, sizeof(*info));

  return RET_OK;
}

static gl_info_t g_gl_info;

static ret_t on_close(void* ctx, event_t* e) {
  tk_quit();
  glview_deinit(&g_gl_info);

  return RET_OK;
}

static vgcanvas_opengl_draw_t s_opengl_draw_bg = {
    .ctx = &g_gl_info, .draw = glview_render_bg, .call_before_ui = TRUE, .next = NULL};
static vgcanvas_opengl_draw_t s_opengl_draw_fg = {
    .ctx = &g_gl_info, .draw = glview_render_fg, .call_before_ui = FALSE, .next = NULL};

static ret_t on_paint(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  vgcanvas_t* vg = canvas_get_vgcanvas(evt->c);
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);

  vgcanvas_append_opengl_draw(vg, &s_opengl_draw_bg);
  vgcanvas_append_opengl_draw(vg, &s_opengl_draw_fg);

  return RET_OK;
}
#endif/*WITH_GPU_GL*/

ret_t application_init() {
  widget_t* system_bar = window_open("system_bar");
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* close = button_create(win, 0, 0, 0, 0);

  widget_set_text_utf8(close, "Close");
  widget_set_self_layout_params(close, "c", "b:10", "60", "30");

  /*将窗口的背景设置为透明，glview_render_bg绘制的东西才不会被覆盖*/
  widget_set_style_color(win, "normal:bg_color", 0);

#if defined(WITH_GPU_GL)
  glview_init(&g_gl_info);
  widget_on(win, EVT_PAINT, on_paint, win);
  widget_on(close, EVT_CLICK, on_close, win);
#endif

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
