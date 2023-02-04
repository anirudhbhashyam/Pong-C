#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"

void render_window(SDL_Renderer*, SDL_Window*);
void render_text(
    SDL_Renderer*, 
    SDL_Window*,
    SDL_Rect*,
    const char*,
    const char*,
    int32_t
);
#endif // WINDOW_H