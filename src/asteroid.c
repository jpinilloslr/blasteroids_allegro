#include "asteroid.h"

static void _draw(Asteroid *asteroid);
static void _update(Asteroid *asteroid);

Asteroid *asteroid_create(Vector2D pos, float scale)
{
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    asteroid->pos.x = pos.x;
    asteroid->pos.y = pos.y;
    asteroid->speed.x = random(-ASTEROID_MAX_SPEED, ASTEROID_MAX_SPEED);
    asteroid->speed.y = random(-ASTEROID_MAX_SPEED, ASTEROID_MAX_SPEED);
    asteroid->rotation = random(0.0f, 360.0f);
    asteroid->rotation_speed = random(-ASTEROID_MAX_ROT_SPEED,
                                        ASTEROID_MAX_ROT_SPEED);
    asteroid->scale = scale;
    asteroid->next = NULL;
    asteroid->previous = NULL;

    asteroid->collider = collider_create(50.0f * scale / 2.0f);
    asteroid->collider->pos.x = asteroid->pos.x;
    asteroid->collider->pos.y = asteroid->pos.y;

    return asteroid;
}

void asteroid_update_and_draw(Asteroid *asteroid)
{
    _update(asteroid);
    _draw(asteroid);
}

void asteroid_destroy(Asteroid *asteroid)
{
    collider_destroy(asteroid->collider);
    free(asteroid);
}

static void _draw(Asteroid *asteroid)
{
    ALLEGRO_TRANSFORM transform;
    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
    al_identity_transform(&transform);
    al_scale_transform(&transform, asteroid->scale, asteroid->scale);
    al_rotate_transform(&transform, asteroid->rotation * M_PI / 180.0f);
    al_translate_transform(&transform, asteroid->pos.x, asteroid->pos.y);
    al_use_transform(&transform);

    al_draw_line(-20, 20, -25, 5, color, 2.0f);
    al_draw_line(-25, 5, -25, -10, color, 2.0f);
    al_draw_line(-25, -10, -5, -10, color, 2.0f);
    al_draw_line(-5, -10, -10, -20, color, 2.0f);
    al_draw_line(-10, -20, 5, -20, color, 2.0f);
    al_draw_line(5, -20, 20, -10, color, 2.0f);
    al_draw_line(20, -10, 20, -5, color, 2.0f);
    al_draw_line(20, -5, 0, 0, color, 2.0f);
    al_draw_line(0, 0, 20, 10, color, 2.0f);
    al_draw_line(20, 10, 10, 20, color, 2.0f);
    al_draw_line(10, 20, 0, 15, color, 2.0f);
    al_draw_line(0, 15, -20, 20, color, 2.0f);

#ifdef DRAW_COLLIDERS
    collider_draw(asteroid->collider);
#endif
}

static void _update(Asteroid *asteroid)
{
    asteroid->pos.x += (asteroid->speed.x);
    asteroid->pos.y -= (asteroid->speed.y);
    asteroid->rotation += (asteroid->rotation_speed);

    if (asteroid->pos.x < 0)
        asteroid->pos.x = SCREEN_WIDTH;
    else if (asteroid->pos.x > SCREEN_WIDTH)
        asteroid->pos.x = 0;

    if (asteroid->pos.y < 0)
        asteroid->pos.y = SCREEN_HEIGHT;
    else if (asteroid->pos.y > SCREEN_HEIGHT)
        asteroid->pos.y = 0;

    if (asteroid->rotation <= -360.0f)
        asteroid->rotation = 0.0f;
    else if (asteroid->rotation >= 360.0f)
        asteroid->rotation = 0.0f;

    asteroid->collider->pos.x = asteroid->pos.x;
    asteroid->collider->pos.y = asteroid->pos.y;
}

