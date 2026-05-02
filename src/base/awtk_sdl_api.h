/**
 * @file awtk_sdl_api.h
 * @brief Central SDL include for AWTK (SDL2 bundled or SDL3 via find_package).
 */
#ifndef AWTK_SDL_API_H
#define AWTK_SDL_API_H

#ifdef AWTK_SDL3
#include <SDL3/SDL.h>
#else
#include <SDL.h>
#endif

#endif /*AWTK_SDL_API_H*/
