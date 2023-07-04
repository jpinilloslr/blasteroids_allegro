#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <stddef.h>
#include "allegro.h"
#include "screen.h"
#include "math_utils.h"
#include "audio.h"
#include "space.h"
#include "asteroid.h"
#include "cannon.h"
#include "explosion.h"

#define SPACESHIP_MAX_SPEED 3.0f
#define SPACESHIP_ACCELERATION 0.06f
#define SPACESHIP_ROTATION_SPEED 5.0f

typedef enum 
{
    SpaceshipSpawning,
    SpaceshipNormal,
    SpaceshipCollided
} SpaceshipState;

typedef struct
{
    int timer;
    Vector2D pos;
    Vector2D speed;
    float rotation;
    Cannon *cannon;
    Collider *collider;
    ALLEGRO_COLOR color;
    SpaceshipState state;
    Explosion *explosion;
} Spaceship;

Spaceship *spaceship_create();
void spaceship_reset(Spaceship *self);
void spaceship_update_and_draw(Spaceship *self);
void spaceship_thrust(Spaceship *self);
void spaceship_break(Spaceship *self);
void spaceship_turn(Spaceship *self, bool right);
void spaceship_check_collissions(Spaceship *self,
                                Space *space,
                                bool *spaceship_impacted,
                                int *impacted_asteroids);
void spaceship_shoot(Spaceship *self);
void spaceship_destroy(Spaceship *self);

#endif
