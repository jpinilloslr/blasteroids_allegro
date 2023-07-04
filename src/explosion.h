#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdbool.h>
#include "math_utils.h"
#include "allegro.h"

#define EXPLOSION_TIME 60
#define MAX_EXPLOSION_PARTICLES 60

typedef struct
{
    Vector2D pos;
    float direction;
    float speed;
} ExplosionParticle;

typedef struct
{
    ExplosionParticle particles[MAX_EXPLOSION_PARTICLES];
    unsigned int timer;
    bool active;
} Explosion;

Explosion *explosion_create(Vector2D pos);
void explosion_update_and_draw(Explosion *explosion);
void explosion_destroy(Explosion *explosion);

#endif
