#include "window.h"

void render_screen(SDL_Renderer* renderer, SDL_Window* window)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);
    sdl_err_handle(SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255), SDL_GetError());
    sdl_err_handle(SDL_RenderClear(renderer), SDL_GetError());
}

void render_board(SDL_Renderer* renderer, SDL_Window* window)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);
    sdl_err_handle(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255), SDL_GetError());
    sdl_err_handle(SDL_RenderDrawLine(renderer, w / 2, 0, w / 2, h), SDL_GetError());
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
