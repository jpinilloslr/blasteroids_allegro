#include "space.h"

static void _create_asteroids(Space *space);
static void _update(Space *space);
static void _update_and_draw_asteroids(Space *space);
static void _break_asteroid(Space *space, Asteroid *asteroid);
static void _remove_asteroid(Space *space, Asteroid *asteroid);
static void _inspect_asteroids_chain(Space *space);
static void _update_and_draw_explosions(Space *space);
static void _create_explosion(Space *space, Vector2D pos);
static int _get_explosion_available_slot(Space *space);
static float _random(float min, float max);

Space *space_create()
{
    Space *space = (Space *)malloc(sizeof(Space));
    space->timer = clock();
    space->first_asteroid = NULL;
    space->initial_asteroids_count = 4;
    space->state = SpaceStateSpawning;
    memset(space->explosions, 0, sizeof(Explosion *) * MAX_EXPLOSIONS);
    return space;
}

void space_update_and_draw(Space *space)
{
    _update(space);
    _update_and_draw_asteroids(space);
    _update_and_draw_explosions(space);
}

void space_hit_asteroid(Space *space, Asteroid *asteroid)
{
    printf("Hit %p (%.1f)\n", asteroid, asteroid->scale);
    _inspect_asteroids_chain(space);

    _create_explosion(space, asteroid->pos);

    (asteroid->scale == ASTEROID_SMALL)
            ? _remove_asteroid(space, asteroid)
            : _break_asteroid(space, asteroid);

    _inspect_asteroids_chain(space);
}

void space_destroy(Space *space)
{
    Asteroid *current = space->first_asteroid;
    while (current != NULL)
    {
        Asteroid *next = current->next;
        asteroid_destroy(current);
        current = next;
    }
}

static void _create_asteroids(Space *space)
{
    Asteroid *first = NULL;
    Asteroid *last = NULL;
    srand(time(NULL));

    for (int i = 0; i < space->initial_asteroids_count; i++)
    {
        Vector2D random_pos = {
            _random((float)SCREEN_WIDTH * -1.0f, (float)SCREEN_WIDTH * 2.0f),
            _random((float)SCREEN_HEIGHT * -1.0f, (float)SCREEN_HEIGHT * 2.0f)
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

    space->first_asteroid = first;
}

void _update(Space *space)
{
    if (space->first_asteroid == NULL && space->state == SpaceStateReady)
    {
        space->initial_asteroids_count++;
        space->timer = clock();
        space->state = SpaceStateSpawning;
    }
    else if (space->state == SpaceStateSpawning)
    {
        if (clock() - space->timer >= 100000)
        {
            _create_asteroids(space);
            space->state = SpaceStateReady;
        }
    }
}

static void _update_and_draw_asteroids(Space *space)
{
    Asteroid *current = space->first_asteroid;
    while (current != NULL)
    {
        asteroid_update_and_draw(current);
        current = current->next;
    }
}

static void _break_asteroid(Space *space, Asteroid *asteroid)
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
        space->first_asteroid = a1;
}

static void _remove_asteroid(Space *space, Asteroid *asteroid)
{
    audio_play_once(SoundExplosionSmall);
    Asteroid *prev = asteroid->previous;
    Asteroid *next = asteroid->next;
    asteroid_destroy(asteroid);

    if (!prev && !next)
        space->first_asteroid = NULL;
    else if (!prev)
        space->first_asteroid = next;

    if (prev)
        prev->next = next;

    if (next)
        next->previous = prev;
}

static void _inspect_asteroids_chain(Space *space)
{
    int c = 0;
    Asteroid *asteroid = space->first_asteroid;
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

static void _create_explosion(Space *space, Vector2D pos)
{
    int slot = _get_explosion_available_slot(space);
    if (slot > -1)
        space->explosions[slot] = explosion_create(pos);
}

static int _get_explosion_available_slot(Space *space)
{
    int availableSlot = -1;
    for (int i = 0; i < MAX_EXPLOSIONS && availableSlot == -1; i++)
    {
        if (space->explosions[i] == NULL)
            availableSlot = i;
    }
    return availableSlot;
}

static void _update_and_draw_explosions(Space *space)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (space->explosions[i] != NULL)
        {
            if (space->explosions[i]->active)
            {
                explosion_update_and_draw(space->explosions[i]);
            }
            else
            {
                explosion_destroy(space->explosions[i]);
                space->explosions[i] = NULL;
            }
        }
    }
}

static float _random(float min, float max)
{
    return (((float)rand() / RAND_MAX) * (max - min + 1.0f) + min);
}

