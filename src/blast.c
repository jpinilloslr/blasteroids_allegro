#include "blast.h"

Blast *blast_create(Vector2D pos, float rotation)
{
    Blast *blast = (Blast *)malloc(sizeof(Blast));
    blast->collider = collider_create(3.0f);
    blast->pos.x = pos.x;
    blast->pos.y = pos.y;
    blast->rotation = rotation;
    blast->gone = false;
    blast->state = BlastNormal;
    blast->color = al_map_rgb(0, 255, 0);
    blast->collider->pos.x = blast->pos.x;
    blast->collider->pos.y = blast->pos.y;
    audio_play_once(SoundBlast);
    return blast;
}

void blast_draw(Blast *blast)
{
    if (blast->state == BlastCollided || blast->gone)
        return;

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, blast->rotation * M_PI / 180.0f);
    al_translate_transform(&transform, blast->pos.x, blast->pos.y);
    al_use_transform(&transform);
    al_draw_line(0, -6, 0, 6, blast->color, 3.0f);

#ifdef DRAW_COLLIDERS
    collider_draw(blast->collider);
#endif
}

static void _update_physics(Blast *blast)
{
    float radians = blast->rotation * M_PI / 180.0f;
    blast->pos.x += BLAST_SPEED * sin(radians);
    blast->pos.y -= BLAST_SPEED * cos(radians);

    blast->collider->pos.x = blast->pos.x;
    blast->collider->pos.y = blast->pos.y;

    if (blast->pos.x < 0
            || blast->pos.x > SCREEN_WIDTH
            || blast->pos.y < 0
            || blast->pos.y > SCREEN_HEIGHT)
        blast->gone = true;
}

static void _update_on_collided(Blast *blast)
{
    blast->gone = true;
}

static void _update_on_ready(Blast *blast)
{
    _update_physics(blast);
}

void blast_update(Blast *blast)
{
    switch (blast->state)
    {
        case BlastCollided:
            _update_on_collided(blast);
            break;
        default:
            _update_on_ready(blast);
    }
}

void blast_destroy(Blast *blast)
{
    collider_destroy(blast->collider);
    free(blast);
}
