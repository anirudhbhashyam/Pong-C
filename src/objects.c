#include "objects.h"

void render_ball(SDL_Renderer* renderer, SDL_Window* window, const Ball* ball)
{
    int32_t cx = ball->pos.x;
    int32_t cy = ball->pos.y;
    int32_t r = ball->r;

    int32_t x_min = cx - r;
    int32_t x_max = cx + r;
    int32_t y_min = cy - r;
    int32_t y_max = cy + r;

    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    for (int32_t x = x_min; x <= x_max; x++)
    {
        if (x >= w || x < 0) continue;
        for (int32_t y = y_min; y <= y_max; y++)
        {
            if (y >= h || y < 0) continue;
            int32_t dx = cx - x;
            int32_t dy = cy - y;

            if (dx * dx + dy * dy <= r) sdl_err_handle(
                SDL_RenderDrawPoint(renderer, x, y), SDL_GetError()
            );
        }
    }
}

void collide_ball_walls(SDL_Window* window, Ball* ball)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    if (ball->pos.x <= 0 || ball->pos.x >= w)
    {        
        reset_ball(ball, vec2i(w / 2, h / 2));
    }

    if (ball->pos.y < 0 || ball->pos.y > h)
    {
        ball->dir.y *= -1;
    }
}

void move_ball(Ball* ball, Vec2i velocity)
{
    ball->pos = vec2i_add(vec2i_mul(velocity, ball->dir), ball->pos);
}

void render_paddle(const SDL_Rect* paddle, SDL_Renderer* renderer)
{
    sdl_err_handle(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255), SDL_GetError());
    sdl_err_handle(SDL_RenderFillRect(renderer, paddle), SDL_GetError());
}

void move_paddle(SDL_Window* window, SDL_Rect* paddle, int32_t velocity)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    int32_t new_y_pos = paddle->y + velocity;
    if (new_y_pos <= 0 || new_y_pos + paddle->h >= h) return;
    paddle->y = new_y_pos;
}

void collide_ball_paddle(SDL_Window* window, Ball* ball, SDL_Rect* paddle)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);
    if (
        abs(paddle->x + paddle->w - ball->pos.x) == 0 
        && (ball->pos.y >= paddle->y && ball->pos.y <= paddle->y + paddle->h)
    ) 
    { 
        ball->dir.x *= -1;
        return; 
    }

    if (
        abs(paddle->x - ball->pos.x) == 0 
        && (ball->pos.y >= paddle->y && ball->pos.y <= paddle->y + paddle->h)
    ) 
    { 
        ball->dir.x *= -1;
        return; 
    }
}
