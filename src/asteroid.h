#ifndef ASTEROID_H
#define ASTEROID_H

#include "allegro.h"
#include "screen.h"
#include "math_utils.h"
#include "collider.h"

#define ASTEROID_MAX_SPEED 1.0f
#define ASTEROID_MAX_ROT_SPEED 2.0f
#define ASTEROID_BIG 2.0f
#define ASTEROID_MEDIUM 1.0f
#define ASTEROID_SMALL 0.5f

typedef struct
{
    Vector2D pos;
    Vector2D speed;
    float rotation;
    float rotation_speed;
    float scale;
    void *next;
    void *previous;
    Collider *collider;
} Asteroid;

Asteroid *asteroid_create(Vector2D pos, float scale);
void asteroid_update_and_draw(Asteroid *asteroid);
void asteroid_destroy(Asteroid *asteroid);

#endif

