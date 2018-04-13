#define GL_GLEXT_PROTOTYPES
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "demo.h"
#include "perf.h"

#define USE_SDL

static int quitting = 0;
static SDL_GLContext gl_context;
static SDL_Window* sdl_window = NULL;

int SDLCALL watch(void* userdata, SDL_Event* event) {
  if (event->type == SDL_APP_WILLENTERBACKGROUND) {
    quitting = 1;
  }

  return 1;
}

int main(int argc, char* argv[]) {
  int t = 0;
  PerfGraph fps;
  DemoData data;
  NVGcontext* vg = NULL;
  int winWidth = 1000;
  int winHeight = 600;
  double prevt = (double)SDL_GetTicks() / 1000;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  sdl_window = SDL_CreateWindow(
      "title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight,
      SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
  
  gl_context = SDL_GL_CreateContext(sdl_window);
  SDL_GL_MakeCurrent(sdl_window, gl_context);
  SDL_GL_SetSwapInterval(0);

  glEnable(GL_ALPHA_TEST);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);
  SDL_AddEventWatch(watch, NULL);

  initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
  vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
  // vg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
  if (loadDemoData(vg, &data) == -1) return -1;

  while (!quitting) {
    double mx, my, t, dt = 0;
    int winWidth, winHeight;
    int fbWidth, fbHeight;
    float pxRatio;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quitting = 1;
      }
    }

    t = (double)SDL_GetTicks() / 1000;
    dt = t - prevt;
    prevt = t;
    updateGraph(&fps, dt);

    printf("t=%lf dt=%lf\n", t, dt);
    SDL_GetWindowSize(sdl_window, &winWidth, &winHeight);
    SDL_GL_GetDrawableSize(sdl_window, &fbWidth, &fbHeight);

    pxRatio = (float)fbWidth / (float)winWidth;

    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
    renderDemo(vg, 0, 0, winWidth, winHeight, t, 0, &data);
    renderGraph(vg, 5, 5, &fps);
    nvgEndFrame(vg);
    SDL_GL_SwapWindow(sdl_window);

    SDL_Delay(16);
  }

  SDL_DelEventWatch(watch, NULL);
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  exit(0);

}  // main
