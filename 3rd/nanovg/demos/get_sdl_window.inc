#include "SDL.h"

static SDL_Window* CreateSDLWindow(int w, int h){
	SDL_Init(0 | SDL_INIT_GAMECONTROLLER);
	static SDL_Window* sdl_window = NULL;
	sdl_window = SDL_CreateWindow("bgfx"
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, w
		, h
		, SDL_WINDOW_SHOWN
		| SDL_WINDOW_RESIZABLE
	);
	return sdl_window;
}
