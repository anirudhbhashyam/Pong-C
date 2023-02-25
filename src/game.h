#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <SDL2/SDL.h>
#include "text.h"
#include "window.h"
#include "objects.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 240
#define BACKGROUND_COLOR 0x202020

static SDL_Color COLOR_WHITE = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255,
};

typedef enum 
{
    GameState_Start = 0,
    GameState_Play,
    GameState_Pause,
    GameState_Quit
} GameState;

void regulate_fps(const uint32_t);
void update_score(SDL_Window*, Ball*, int32_t*, int32_t*);
void run(void);

#endif // GAME_H 
