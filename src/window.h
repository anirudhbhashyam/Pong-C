#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"

void render_screen(SDL_Renderer*, SDL_Window*);
void render_board(SDL_Renderer*, SDL_Window*);

#endif // WINDOW_H
