#include "collider.h"

Collider *collider_create(float radius)
{
    Collider *c = (Collider *)malloc(sizeof(Collider));
    c->radius = radius;
    return c;
}

bool collider_check(Collider *collider1, Collider *collider2)
{
    float distance = sqrtf(powf(collider2->pos.y - collider1->pos.y, 2) + powf(collider1->pos.x - collider2->pos.x, 2));
    return distance < collider1->radius + collider2->radius;
}

void collider_draw(Collider *collider)
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_translate_transform(&transform, collider->pos.x, collider->pos.y);
    al_use_transform(&transform);
    al_draw_circle(0, 0, collider->radius, al_map_rgb(255, 0, 0), 1.0f);
}

void collider_destroy(Collider *collider)
{
    free(collider);
}
