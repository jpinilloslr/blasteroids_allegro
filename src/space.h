#ifndef SPACE_H
#define SPACE_H

#include "math_utils.h"
#include "audio.h"
#include "asteroid.h"
#include "explosion.h"

#define MAX_EXPLOSIONS 5

enum SpaceState
{
    SpaceStateSpawning,
    SpaceStateReady
};

typedef struct
{
    clock_t timer;
    enum SpaceState state;
    Asteroid *first_asteroid;
    Explosion* explosions[MAX_EXPLOSIONS];
    unsigned char initial_asteroids_count;
} Space;

Space *space_create();
void space_update_and_draw(Space *asteroid);
void space_hit_asteroid(Space *space, Asteroid *asteroid);
void space_destroy(Space *asteroid);

#endif
