#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "vec2i.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 240

#define UNIMPLEMENTED(x) printf("%s\n", (x))

#define BACKGROUND_COLOR 0x202020

// #define ARRAY_LEN(x) sizeof((x)) / sizeof((x)[0])

// #define KEY_SPACE_LENGTH 322
// bool KEYS[KEY_SPACE_LENGTH] = { 0 };

typedef struct 
{
    Vec2i pos;
    Vec2i dir;
    int32_t r; 
} Ball;

int32_t sdl_err_handle(int32_t code, const char* reason) 
{
    if (code < 0)
    {
        fprintf(stderr, "SDL failed with %d: %s", code, reason);
        exit(1);
    }
    return code;
}

void* sdl_err_handle_ptr(void* ptr, const char* reason) 
{
    if (ptr == NULL)
    {
        fprintf(stderr, "SDL failed: %s", reason);
        exit(1);
    }

    return ptr;
}

static inline void regulate_fps(const uint32_t old_time)
{
    const uint32_t now = SDL_GetTicks();
    const int32_t delay = 1000 / FPS - (now - old_time);
    if (delay > 0) SDL_Delay(delay);
}

static inline Vec2i get_window_size(SDL_Window* window)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);
    return vec2i(w, h);
}

void render_paddle(const SDL_Rect* paddle, SDL_Renderer* renderer)
{
    sdl_err_handle(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255), SDL_GetError());
    sdl_err_handle(SDL_RenderFillRect(renderer, paddle), SDL_GetError());
}

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

void render_text(
    SDL_Renderer* renderer, 
    SDL_Window* window,
    SDL_Rect* font_rect,
    const char* font_file,
    const char* text,
    int32_t font_size
)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    TTF_Font* font = sdl_err_handle_ptr(
        TTF_OpenFont(font_file, font_size),
        TTF_GetError()
    ); 

    SDL_Color white = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 255
    };
    
    SDL_Surface* font_surface = sdl_err_handle_ptr(
        TTF_RenderText_Solid(font, text, white),
        TTF_GetError()
    );

    SDL_Texture* font_texture = sdl_err_handle_ptr(
        SDL_CreateTextureFromSurface(renderer, font_surface),
        TTF_GetError()
    );

    sdl_err_handle(SDL_RenderCopy(renderer, font_texture, NULL, font_rect), SDL_GetError());
}

void move_paddle(SDL_Window* window, SDL_Rect* paddle, int32_t velocity)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    int32_t new_y_pos = paddle->y + velocity;
    if (new_y_pos <= 0 || new_y_pos + paddle->h >= h) return;
    paddle->y = new_y_pos;
}

static inline void reset_ball(Ball* ball, Vec2i pos)
{
    ball->pos = pos;
    ball->dir = vec2i(1, 1);
}

void move_ball(Ball* ball, Vec2i velocity)
{
    ball->pos = vec2i_add(vec2i_mul(velocity, ball->dir), ball->pos);
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

void update_score(SDL_Window* window, Ball* ball, int32_t* left_score, int32_t* right_score)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    if (ball->pos.x <= 0)
    {
        *right_score += 1;
        return;
    }

    if (ball->pos.x >= w)
    {
        *left_score += 1;
        return;
    }
}

void render_window(SDL_Renderer* renderer, SDL_Window* window)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    sdl_err_handle(SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255), SDL_GetError());
    sdl_err_handle(SDL_RenderClear(renderer), SDL_GetError());
    sdl_err_handle(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255), SDL_GetError());
    sdl_err_handle(SDL_RenderDrawLine(renderer, w / 2, 0, w / 2, h), SDL_GetError());
}

int main(void) 
{
    srand(time(NULL) * getpid());

    sdl_err_handle(SDL_Init(SDL_INIT_VIDEO), SDL_GetError());

    sdl_err_handle(TTF_Init(), TTF_GetError());

    SDL_Window* window = sdl_err_handle_ptr(
        SDL_CreateWindow(
            "Brick Breaker",
            0,
            0, 
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_RESIZABLE
        ),
        SDL_GetError()
    );

    int32_t window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    SDL_Renderer* renderer = sdl_err_handle_ptr(
        SDL_CreateRenderer(
            window,
            0,
            SDL_RENDERER_ACCELERATED
        ),
        SDL_GetError()
    );

    sdl_err_handle(
        SDL_RenderSetLogicalSize(
            renderer,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
        ),
        SDL_GetError()
    );
    
    SDL_Rect left_paddle = {
        .x = 0,
        .y = 10,
        .w = 10,
        .h = 50
    };
   
    SDL_Rect right_paddle = {
        .x = SCREEN_WIDTH - 10,
        .y = 10,
        .w = 10,
        .h = 50
    };

    int32_t left_score = 0;
    int32_t right_score = 0;
    char left_score_str[50] = { 0 };
    char right_score_str[50] = { 0 };

    Vec2i left_score_pos = vec2i(SCREEN_WIDTH / 2 - 100, 50);
    Vec2i right_score_pos = vec2i(SCREEN_WIDTH / 2 + 100, 50);

    SDL_Rect left_score_rect = {
        .x = left_score_pos.x - 50 / 2,
        .y = left_score_pos.y - 50 / 2,
        .w = 50,
        .h = 50
    };

    SDL_Rect right_score_rect = {
        .x = right_score_pos.x - 50 / 2,
        .y = right_score_pos.y - 50 / 2,
        .w = 50,
        .h = 50
    };

    Vec2i ball_velocity = vec2i(1, 1);

    Ball ball = {
        vec2i(400, 300),
        vec2i(-1, -1),
        50,
    };

    SDL_Event event;
    uint8_t running = 1;
    int32_t left_paddle_velocity = 0;
    int32_t right_paddle_velocity = 0;

    uint32_t start;

    while (running)
    {
        start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break; 

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) 
                    {
                        case SDLK_w:
                            left_paddle_velocity = -2;
                            break;

                        case SDLK_s:
                            left_paddle_velocity = 2;
                            break;

                        case SDLK_UP:
                            right_paddle_velocity = -2;
                            break;

                        case SDLK_DOWN:
                            right_paddle_velocity = 2;
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            left_paddle_velocity = (left_paddle_velocity < 0) ? 0 : left_paddle_velocity;
                            break;

                        case SDLK_s:
                            left_paddle_velocity = (left_paddle_velocity > 0) ? 0 : left_paddle_velocity;
                            break;

                        case SDLK_UP:
                            right_paddle_velocity = (right_paddle_velocity < 0) ? 0 : right_paddle_velocity;
                            break;

                        case SDLK_DOWN:
                            right_paddle_velocity = (right_paddle_velocity > 0) ? 0 : right_paddle_velocity;
                            break;

                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
            break;
        }

        if (!running) break;

        // Clear the background.
        render_window(renderer, window);

        // Move objects.
        move_paddle(window, &left_paddle, left_paddle_velocity);
        move_paddle(window, &right_paddle, right_paddle_velocity);
        move_ball(&ball, ball_velocity);
        
        update_score(window, &ball, &left_score, &right_score);
        sprintf(left_score_str, "%d", left_score);
        sprintf(right_score_str, "%d", right_score);

        collide_ball_walls(window, &ball);
        collide_ball_paddle(window, &ball, &left_paddle);
        collide_ball_paddle(window, &ball, &right_paddle);
        
        // Game rendering.
        render_paddle(&left_paddle, renderer);
        render_paddle(&right_paddle, renderer);
        render_ball(renderer, window, &ball);

        render_text(renderer, window, &left_score_rect, "./assets/Roboto-Regular.ttf", left_score_str, 24);
        render_text(renderer, window, &right_score_rect, "./assets/Roboto-Regular.ttf", right_score_str, 24);

        // Update the window.
        SDL_RenderPresent(renderer);

        regulate_fps(start);
    }

    SDL_Quit();
    return 0;
}