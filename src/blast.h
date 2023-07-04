#ifndef BLAST_H
#define BLAST_H

#include <stdbool.h>
#include "math_utils.h"
#include "screen.h"
#include "audio.h"
#include "collider.h"

#define BLAST_SPEED 16.0f

enum BlastState
{
    BlastNormal,
    BlastCollided
};

typedef struct
{
    Vector2D pos;
    float rotation;
    bool gone;
    enum BlastState state;
    Collider *collider;
    ALLEGRO_COLOR color;
} Blast;

Blast *blast_create(Vector2D pos, float rotation);
void blast_update(Blast *blast);
void blast_draw(Blast *blast);
void blast_destroy(Blast *blast);

#endif
