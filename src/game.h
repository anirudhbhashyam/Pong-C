#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "objects.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 240
#define BACKGROUND_COLOR 0x202020

void regulate_fps(const uint32_t);
void update_score(SDL_Window*, Ball*, int32_t*, int32_t*);
void run(void);

#endif // GAME_H