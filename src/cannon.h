#ifndef CANNON_H
#define CANNON_H

#include "math_utils.h"
#include "blast.h"
#include "space.h"
#include "collider.h"

#define MAX_BLASTS 100

typedef struct
{
    Vector2D pos;
    float rotation;
    Blast *blasts[MAX_BLASTS];
    int rate_of_fire;
    clock_t last_blast;
} Cannon;

Cannon *cannon_create();
void cannon_shoot(Cannon *cannon);
void cannon_update_and_draw(Cannon *cannon);
void cannon_check_collissions(Cannon *cannon,
                            Space *space,
                            int *impacted_asteroids);
void cannon_destroy(Cannon *cannon);

#endif
