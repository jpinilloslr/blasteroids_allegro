#ifndef COLLIDER_H
#define COLLIDER_H

#include <stdbool.h>
#include "allegro.h"
#include "math_utils.h"

typedef struct
{
    Vector2D pos;
    float radius;
} Collider;

Collider *collider_create(float radius);
bool collider_check(Collider *collider1, Collider *collider2);
void collider_draw(Collider *collider);
void collider_destroy(Collider *collider);

#endif
