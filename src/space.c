#include "space.h"

static void _create_asteroids(Space *self);
static void _update(Space *self);
static void _update_and_draw_asteroids(Space *self);
static void _break_asteroid(Space *self, Asteroid *asteroid);
static void _remove_asteroid(Space *self, Asteroid *asteroid);
static void _inspect_asteroids_chain(Space *self);
static void _update_and_draw_explosions(Space *self);
static void _create_explosion(Space *self, Vector2D pos);
static int _get_explosion_available_slot(Space *self);

Space *space_create()
{
    Space *self = (Space *)malloc(sizeof(Space));
    self->timer = clock();
    self->first_asteroid = NULL;
    self->initial_asteroids_count = 4;
    self->state = SpaceStateSpawning;
    memset(self->explosions, 0, sizeof(Explosion *) * MAX_EXPLOSIONS);
    return self;
}

void space_update_and_draw(Space *self)
{
    _update(self);
    _update_and_draw_asteroids(self);
    _update_and_draw_explosions(self);
}

void space_hit_asteroid(Space *self, Asteroid *asteroid)
{
    printf("Hit %p (%.1f)\n", asteroid, asteroid->scale);
    _inspect_asteroids_chain(self);

    _create_explosion(self, asteroid->pos);

    (asteroid->scale == ASTEROID_SMALL)
            ? _remove_asteroid(self, asteroid)
            : _break_asteroid(self, asteroid);

    _inspect_asteroids_chain(self);
}

void space_destroy(Space *self)
{
    Asteroid *current = self->first_asteroid;
    while (current != NULL)
    {
        Asteroid *next = current->next;
        asteroid_destroy(current);
        current = next;
    }
}

static void _create_asteroids(Space *self)
{
    Asteroid *first = NULL;
    Asteroid *last = NULL;
    srand(time(NULL));

    for (int i = 0; i < self->initial_asteroids_count; i++)
    {
        Vector2D random_pos = {
            random((float)SCREEN_WIDTH * -1.0f, (float)SCREEN_WIDTH * 2.0f),
            random((float)SCREEN_HEIGHT * -1.0f, (float)SCREEN_HEIGHT * 2.0f)
        };
        Asteroid *n = asteroid_create(random_pos, ASTEROID_BIG);
        if (first == NULL)
        {
            first = last = n;
        }
        else
        {
            last->next = n;
            Asteroid *previous = last;
            last = last->next;
            last->previous = previous;
        }
    }

    self->first_asteroid = first;
}

void _update(Space *self)
{
    if (self->first_asteroid == NULL && self->state == SpaceStateReady)
    {
        self->initial_asteroids_count++;
        self->timer = clock();
        self->state = SpaceStateSpawning;
    }
    else if (self->state == SpaceStateSpawning)
    {
        if (clock() - self->timer >= 100000)
        {
            _create_asteroids(self);
            self->state = SpaceStateReady;
        }
    }
}

static void _update_and_draw_asteroids(Space *self)
{
    Asteroid *current = self->first_asteroid;
    while (current != NULL)
    {
        asteroid_update_and_draw(current);
        current = current->next;
    }
}

static void _break_asteroid(Space *self, Asteroid *asteroid)
{
    if (asteroid->scale == ASTEROID_BIG)
        audio_play_once(SoundExplosionLarge);
    else
        audio_play_once(SoundExplosionMedium);

    float scale = asteroid->scale / 2.0f;
    Asteroid *prev = asteroid->previous;
    Asteroid *next = asteroid->next;
    Asteroid *a1 = asteroid_create(asteroid->pos, scale);
    Asteroid *a2 = asteroid_create(asteroid->pos, scale);
    asteroid_destroy(asteroid);

    a1->next = a2;
    a2->previous = a1;

    if (prev)
    {
        prev->next = a1;
        a1->previous = prev;
    }

    if (next)
    {
        a2->next = next;
        next->previous = a2;
    }

    if (!prev)
        self->first_asteroid = a1;
}

static void _remove_asteroid(Space *self, Asteroid *asteroid)
{
    audio_play_once(SoundExplosionSmall);
    Asteroid *prev = asteroid->previous;
    Asteroid *next = asteroid->next;
    asteroid_destroy(asteroid);

    if (!prev && !next)
        self->first_asteroid = NULL;
    else if (!prev)
        self->first_asteroid = next;

    if (prev)
        prev->next = next;

    if (next)
        next->previous = prev;
}

static void _inspect_asteroids_chain(Space *self)
{
    int c = 0;
    Asteroid *asteroid = self->first_asteroid;
    while (asteroid != NULL)
    {
        c++;
        printf("%p (%.1f)", asteroid, asteroid->scale);
        if (asteroid->next)
            printf(" => ");
        else
            printf("\n");
        asteroid = asteroid->next;
    }
}

static void _create_explosion(Space *self, Vector2D pos)
{
    int slot = _get_explosion_available_slot(self);
    if (slot > -1)
        self->explosions[slot] = explosion_create(pos);
}

static int _get_explosion_available_slot(Space *self)
{
    int availableSlot = -1;
    for (int i = 0; i < MAX_EXPLOSIONS && availableSlot == -1; i++)
    {
        if (self->explosions[i] == NULL)
            availableSlot = i;
    }
    return availableSlot;
}

static void _update_and_draw_explosions(Space *self)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (self->explosions[i] != NULL)
        {
            if (self->explosions[i]->active)
            {
                explosion_update_and_draw(self->explosions[i]);
            }
            else
            {
                explosion_destroy(self->explosions[i]);
                self->explosions[i] = NULL;
            }
        }
    }
}

