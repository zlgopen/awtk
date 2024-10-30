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

static ret_t opengl_init(void) {
  opengl_loadGL();

  glDisable(GL_STENCIL_TEST);
#ifndef HARMONY_OS  
  glDisable(GL_ALPHA_TEST);
#endif/*HARMONY_OS*/  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);

  return RET_OK;
}
#endif/*WITH_GPU_GL*/

