#include "text.h"

void render_text(
    SDL_Renderer* renderer, 
    SDL_Window* window,
    const TextContainer* text_container
)
{
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Font object may not be the same size as 
    // the font size. Resize it.

    if SDL_VERSION_ATLEAST(2, 0, 18)
    {
        sdl_err_handle(
            TTF_SetFontSize(text_container->font, text_container->size),
            TTF_GetError()
        );
    }

    SDL_Surface* font_surface = sdl_err_handle_ptr(
        TTF_RenderText_Solid(text_container->font, text_container->text, text_container->color),
        TTF_GetError()
    );

    SDL_Texture* font_texture = sdl_err_handle_ptr(
        SDL_CreateTextureFromSurface(renderer, font_surface),
        TTF_GetError()
    );

    sdl_err_handle(SDL_RenderCopy(renderer, font_texture, NULL, &text_container->rect), SDL_GetError());

    SDL_FreeSurface(font_surface);
    SDL_DestroyTexture(font_texture);
}
