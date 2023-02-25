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

    // Text init.
    TTF_Font* font = sdl_err_handle_ptr(
        TTF_OpenFont("./assets/Roboto-Regular.ttf", 12),
        TTF_GetError()
    );

    // Start menu.
    Vec2i start_game_text_pos = vec2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    Vec2i start_game_text_dim = vec2i(500, 40);
    SDL_Rect start_game_rect = {
        .x = start_game_text_pos.x - start_game_text_dim.x / 2,
        .y = start_game_text_pos.y - start_game_text_dim.y / 2,
        .w = start_game_text_dim.x,
        .h = start_game_text_dim.y
    };

    TextContainer start_game_text = {
        .font = font,
        .rect = start_game_rect,
        .text = "Press 'p' to start the game.",
        .size = 24,
        .color = COLOR_WHITE
    };

    // Paddles.
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

    // Scores.
    int32_t left_score = 0;
    int32_t right_score = 0;

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

    TextContainer left_score_text = {
        .font = font,
        .rect = left_score_rect,
        .text = "",
        .size = 24,
        .color = COLOR_WHITE
    };
    TextContainer right_score_text = {
        .font = font,
        .rect = right_score_rect,
        .text = "",
        .size = 24,
        .color = COLOR_WHITE
    };

    // Ball init.
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

    GameState game_state = GameState_Start;

    while (running)
    {
        start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    game_state = GameState_Quit;
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

                        case SDLK_p:
                            game_state = (game_state != GameState_Play) ? GameState_Play : game_state; 

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
        render_screen(renderer, window);

        switch (game_state)
        {
            case GameState_Start:
                render_text(
                    renderer, 
                    window, 
                    &start_game_text
                );
                break;

            case GameState_Play:
                // Move objects.
                move_paddle(window, &left_paddle, left_paddle_velocity);
                move_paddle(window, &right_paddle, right_paddle_velocity);
                move_ball(&ball, ball_velocity);
                
                update_score(window, &ball, &left_score, &right_score);
                sprintf(left_score_text.text, "%d", left_score);
                sprintf(right_score_text.text, "%d", right_score);

                collide_ball_walls(window, &ball);
                collide_ball_paddle(window, &ball, &left_paddle);
                collide_ball_paddle(window, &ball, &right_paddle);
                
                // Game rendering.
                render_board(renderer, window);

                render_paddle(&left_paddle, renderer);
                render_paddle(&right_paddle, renderer);
                render_ball(renderer, window, &ball);

                render_text(renderer, window, &left_score_text);
                render_text(renderer, window, &right_score_text);
                break;

            default: 
                break;
        }

        // Update the window.
        SDL_RenderPresent(renderer);

        regulate_fps(start);
    }

    SDL_Quit();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
