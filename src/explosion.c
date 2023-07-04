#include "explosion.h"

static void _update_particle(unsigned int timer, ExplosionParticle *particle);
static void _draw_particle(ExplosionParticle *particle);
static float _random(float min, float max);

Explosion *explosion_create(Vector2D pos)
{
    Explosion *explosion = (Explosion *)malloc(sizeof(Explosion));
    explosion->timer = EXPLOSION_TIME;
    explosion->active = true;

    for (int i = 0; i < MAX_EXPLOSION_PARTICLES; i++)
    {
        explosion->particles[i].pos.x = pos.x;
        explosion->particles[i].pos.y = pos.y;
        explosion->particles[i].speed = _random(0.5f, 1.0f);
        explosion->particles[i].direction =
            (360.0f / MAX_EXPLOSION_PARTICLES) * i;
    }
    return explosion;
}

void explosion_update_and_draw(Explosion *explosion)
{
    if (explosion->timer <= 0)
    {
        explosion->active = false;
        return;
    }

    explosion->timer--;
    for (int i = 0; i < MAX_EXPLOSION_PARTICLES; i++)
    {
        ExplosionParticle *particle = &explosion->particles[i];
        _update_particle(explosion->timer, particle);
        _draw_particle(particle);
    }
}

void explosion_destroy(Explosion *explosion)
{
    free(explosion);
}

void _update_particle(unsigned int timer, ExplosionParticle *particle)
{
    float speed = timer / (float)EXPLOSION_TIME * 2.0f ;
    float radians = particle->direction * M_PI / 180.0f;
    particle->pos.x += particle->speed * speed * sin(radians);
    particle->pos.y += particle->speed * speed * cos(radians);
}

void _draw_particle(ExplosionParticle *particle)
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_translate_transform(&transform, particle->pos.x, particle->pos.y);
    al_use_transform(&transform);
    al_draw_line(-1, 0, 1, 0, al_map_rgb(0, 255, 0), 1.0f);
}

static float _random(float min, float max)
{
    return (((float)rand() / RAND_MAX) * (max - min + 1.0f) + min);
}

