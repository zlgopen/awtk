#ifndef AWTK_OPENGL_H
#define AWTK_OPENGL_H
#if defined(WITH_GPU_GL)

#ifdef WITHOUT_GLAD
#define GL_GLEXT_PROTOTYPES 1
#ifdef IOS
#include <SDL.h>
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(HARMONY_OS)
#include <GLES3/gl31.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#else
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/
#define opengl_loadGL()
#else
#include "glad/glad.h"
#define opengl_loadGL() gladLoadGL()
#endif /*WITHOUT_GLAD*/

static inline void opengl_init(void) {
  opengl_loadGL();

  glDisable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);

  return;
}
#endif /*WITH_GPU_GL*/
#endif /*AWTK_OPENGL_H*/
