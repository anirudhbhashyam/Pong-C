#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Do not remove this include. 
// Utils is used by many other headers.
#include "vec2i.h"

static inline int32_t sdl_err_handle(int32_t code, const char* reason) 
{
    if (code < 0)
    {
        fprintf(stderr, "SDL failed with %d: %s", code, reason);
        exit(1);
    }
    return code;
}

static inline void* sdl_err_handle_ptr(void* ptr, const char* reason)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "SDL failed: %s", reason);
        exit(1);
    }

    return ptr;
}

#endif // UTILS_H
