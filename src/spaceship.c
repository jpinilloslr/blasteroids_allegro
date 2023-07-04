#include "spaceship.h"

static void _draw(Spaceship *self);
static void _update(Spaceship *self);
static void _update_on_spawning_state(Spaceship *self);
static void _update_on_normal_state(Spaceship *self);
static void _update_on_collided_state(Spaceship *self);
static void _update_physics(Spaceship *self);
static void _check_spaceship_collisions(Spaceship *self,
                                        Space *space,
                                        bool* spaceship_impacted);

Spaceship *spaceship_create()
{
    Spaceship *s = (Spaceship *)malloc(sizeof(Spaceship));
    s->cannon = cannon_create();
    s->collider = collider_create(15.0f);
    spaceship_reset(s);
    return s;
}

void spaceship_reset(Spaceship *self)
{
    self->pos.x = SCREEN_WIDTH / 2.0f;
    self->pos.y = SCREEN_HEIGHT / 2.0f;
    self->rotation = 0.0f;
    self->speed.x = 0.0f;
    self->speed.y = 0.0f;
    self->state = SpaceshipSpawning;
    self->color = al_map_rgb(0, 255, 0);
    self->timer = 0;
    self->explosion = NULL;
    self->collider->pos.x = self->pos.x;
    self->collider->pos.y = self->pos.y;
}

void spaceship_update_and_draw(Spaceship *self)
{
    if (self->explosion != NULL && self->explosion->active)
        explosion_update_and_draw(self->explosion);
    
    _update(self);
    _draw(self);
    cannon_update_and_draw(self->cannon);
}

void spaceship_thrust(Spaceship *self)
{
    float radians = self->rotation * M_PI / 180.0f;
    self->speed.x += SPACESHIP_ACCELERATION * sin(radians);
    self->speed.y += SPACESHIP_ACCELERATION * cos(radians);
}

void spaceship_break(Spaceship *self)
{
    if (self->speed.x > SPACESHIP_ACCELERATION)
        self->speed.x -= SPACESHIP_ACCELERATION;
    else if (self->speed.x < -SPACESHIP_ACCELERATION)
        self->speed.x += SPACESHIP_ACCELERATION;
    else
        self->speed.x = 0.0f;

    if (self->speed.y > SPACESHIP_ACCELERATION)
        self->speed.y -= SPACESHIP_ACCELERATION;
    else if (self->speed.y < -SPACESHIP_ACCELERATION)
        self->speed.y += SPACESHIP_ACCELERATION;
    else
        self->speed.y = 0.0f;
}

void spaceship_turn(Spaceship *self, bool right)
{
    if (right)
        self->rotation += SPACESHIP_ROTATION_SPEED;
    else
        self->rotation -= SPACESHIP_ROTATION_SPEED;
}

void spaceship_check_collissions(Spaceship *self,
                                Space *space,
                                bool *spaceship_impacted,
                                int *impacted_asteroids)
{
    _check_spaceship_collisions(self, space, spaceship_impacted);
    cannon_check_collissions(self->cannon, space, impacted_asteroids);
}

void spaceship_shoot(Spaceship *self)
{
    if (self->state != SpaceshipCollided)
        cannon_shoot(self->cannon);
}

void spaceship_destroy(Spaceship *self)
{
    cannon_destroy(self->cannon);
    collider_destroy(self->collider);
    free(self);
}

static void _draw(Spaceship *self)
{
    if (self->state == SpaceshipCollided)
        return;

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, self->rotation * M_PI / 180.0f);
    al_translate_transform(&transform, self->pos.x, self->pos.y);
    al_use_transform(&transform);
    al_draw_line(-8, 9, 0, -11, self->color, 3.0f);
    al_draw_line(0, -11, 8, 9, self->color, 3.0f);
    al_draw_line(-6, 4, -1, 4, self->color, 3.0f);
    al_draw_line(6, 4, 1, 4, self->color, 3.0f);

#ifdef DRAW_COLLIDERS
    collider_draw(self->collider);
#endif
}

static void _update(Spaceship *self)
{
    switch (self->state)
    {
    case SpaceshipSpawning:
        _update_on_spawning_state(self);
        break;
    case SpaceshipCollided:
        _update_on_collided_state(self);
        break;
    default:
        _update_on_normal_state(self);
    }

    self->cannon->pos.x = self->pos.x;
    self->cannon->pos.y = self->pos.y;
    self->cannon->rotation = self->rotation;

    if (self->explosion != NULL && !self->explosion->active)
    {
        explosion_destroy(self->explosion);
        self->explosion = NULL;
    }
}

static void _update_on_spawning_state(Spaceship *self)
{
    static bool toggle = false;
    self->timer++;

    if (self->timer % 10 == 0)
    {
        toggle = !toggle;
        self->color = toggle
                            ? al_map_rgb(0, 100, 0)
                            : al_map_rgb(0, 255, 0);
    }

    if (self->timer >= 60 * 3)
    {
        self->color = al_map_rgb(0, 255, 0);
        self->state = SpaceshipNormal;
        self->timer = 0;
    }
    _update_physics(self);
}

static void _update_on_normal_state(Spaceship *self)
{
    _update_physics(self);
}

static void _update_on_collided_state(Spaceship *self)
{
    self->timer++;

    if (self->timer > 60 * 2)
    {
        spaceship_reset(self);
    }
}

void _update_physics(Spaceship *self)
{
    self->pos.x += (self->speed.x);
    self->pos.y -= (self->speed.y);
    if (self->pos.x < 0)
        self->pos.x = SCREEN_WIDTH;
    else if (self->pos.x > SCREEN_WIDTH)
        self->pos.x = 0;

    if (self->pos.y < 0)
        self->pos.y = SCREEN_HEIGHT;
    else if (self->pos.y > SCREEN_HEIGHT)
        self->pos.y = 0;

    if (self->speed.x > SPACESHIP_MAX_SPEED)
        self->speed.x = SPACESHIP_MAX_SPEED;
    else if (self->speed.x < -SPACESHIP_MAX_SPEED)
        self->speed.x = -SPACESHIP_MAX_SPEED;

    if (self->speed.y > SPACESHIP_MAX_SPEED)
        self->speed.y = SPACESHIP_MAX_SPEED;
    else if (self->speed.y < -SPACESHIP_MAX_SPEED)
        self->speed.y = -SPACESHIP_MAX_SPEED;

    if (self->rotation <= -360.0f)
        self->rotation = 0.0f;
    else if (self->rotation >= 360.0f)
        self->rotation = 0.0f;

    self->collider->pos.x = self->pos.x;
    self->collider->pos.y = self->pos.y;
}

static void _check_spaceship_collisions(Spaceship *self,
                                        Space *space,
                                        bool* spaceship_impacted)
{
    *spaceship_impacted = false;
    if (self->state != SpaceshipNormal)
        return;

    Collider *ssc = self->collider;
    Asteroid *asteroid = space->first_asteroid;

    while (!*spaceship_impacted && asteroid != NULL)
    {
        *spaceship_impacted = collider_check(ssc, asteroid->collider);
        asteroid = asteroid->next;
    }

    if (*spaceship_impacted)
    {
        self->state = SpaceshipCollided;
        if (self->explosion == NULL)
            self->explosion = explosion_create(self->pos);
        audio_play_once(SoundExplosionMedium);
    }
}

