#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "vec2i.h"

#include "game.h"


int main(void) 
{
    srand(time(NULL) * getpid());
    run();

    return 0;
}