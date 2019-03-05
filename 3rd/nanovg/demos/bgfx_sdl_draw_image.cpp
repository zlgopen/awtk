#include "bgfx/nanovg_bgfx.h"
#include "bgfx/nanovg_bgfx_ex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include "draw_image.inc"
#include "get_sdl_window.inc"


static void run_test(int w, int h)
{
	bool exit = false;
	SDL_Event event;
	SDL_WindowEvent wev;
	NVGcontext* vg = nvgCreateBGFX(1, 0, w, h, CreateSDLWindow(w, h));
	while (!exit)
	{
		setBGFXViewRect(0, 0, 0, w, h); 
		touchBGFX(0);
		do_draw(vg, w, h);
		frameBGFX(false);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exit = true;
				break;
			case SDL_WINDOWEVENT:
			{
				wev = event.window;
				switch (wev.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					w = wev.data1;
					h = wev.data2;
					resetBGFX(w, h, 0);
				}
				break;
				default:
					break;
				}
			}
			default:
				break;
			}
		}
	}
	nvgDeleteBGFX(vg);
}

#if defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
#else
int main(void) {
#endif
	run_test(400, 400);
	return 0;
}
