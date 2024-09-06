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
#define  opengl_init()
#else
#include "glad/glad.h"
#define opengl_init gladLoadGL
#endif /*WITHOUT_GLAD*/

#endif/*WITH_GPU_GL*/

