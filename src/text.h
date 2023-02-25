#ifndef TEXT_H
#define TEXT_H

#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font* font;
    SDL_Rect rect;
    char text[256];
    int32_t size;
    SDL_Color color;
} TextContainer;

void render_text(SDL_Renderer*, SDL_Window*, const TextContainer*);

#endif // TEXT_H
