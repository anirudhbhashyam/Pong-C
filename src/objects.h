#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL2/SDL.h>
#include "utils.h"

typedef struct 
{
    Vec2i pos;
    Vec2i dir;
    int32_t r; 
} Ball;

void render_ball(SDL_Renderer*, SDL_Window*, const Ball*);
void render_paddle(const SDL_Rect*, SDL_Renderer*);
void move_paddle(SDL_Window*, SDL_Rect*, int32_t);
void move_ball(Ball*, Vec2i);
void collide_ball_walls(SDL_Window*, Ball*);
void collide_ball_paddle(SDL_Window*, Ball*, SDL_Rect*);

static inline void reset_ball(Ball* ball, Vec2i pos)
{
    ball->pos = pos;
    ball->dir = vec2i(1, 1);
}

#endif // OBJECTS_H
