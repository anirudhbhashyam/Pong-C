#include "game.h"

void regulate_fps(const uint32_t old_time)
{
    const uint32_t now = SDL_GetTicks();
    const int32_t delay = 1000 / FPS - (now - old_time);
    if (delay > 0) SDL_Delay(delay);
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


void run(void)
{
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

    Vec2i ball_velocity = vec2i(2, 2);

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
}