#ifndef VEC2I_H
#define VEC2I_H

#include <stdint.h>

typedef struct {
    int32_t x;
    int32_t y;
} Vec2i;

static inline Vec2i vec2i(const int32_t x, const int32_t y)
{
    return (Vec2i) {
        .x = x,
        .y = y
    };
}

static inline Vec2i vec2i_add(const Vec2i v1, const Vec2i v2)
{
    return vec2i(v1.x + v2.x, v1.y + v2.y);
}

static inline Vec2i vec2i_sub(const Vec2i v1, const Vec2i v2)
{
    return vec2i(v1.x - v2.x, v1.y - v2.y);
}

static inline Vec2i vec2i_mul(const Vec2i v1, const Vec2i v2)
{
    return vec2i(v1.x * v2.x, v1.y * v2.y);
}

#endif // VEC2I_H
