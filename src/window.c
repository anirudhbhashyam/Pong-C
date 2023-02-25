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
