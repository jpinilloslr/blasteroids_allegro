#include "cannon.h"

static int _get_blast_available_slot(Cannon *cannon);
static int _check_blast_collision(Blast *blast, Space *space);
static void _blast_hit_asteroid(Blast *blast,
                            Space *space,
                            Asteroid *impacted_asteroid);

Cannon *cannon_create()
{
    Cannon* cannon = (Cannon *)malloc(sizeof(Cannon));
    cannon->pos.x = 0;
    cannon->pos.y = 0;
    cannon->rotation = 0;
    memset(cannon->blasts, 0, sizeof(Blast *) * MAX_BLASTS);
    cannon->rate_of_fire = 20000;
    cannon->last_blast = 0;
    return cannon;
}

void cannon_shoot(Cannon *cannon)
{
    if (clock() - cannon->last_blast < cannon->rate_of_fire)
        return;

    int slot = _get_blast_available_slot(cannon);

    if (slot > -1)
        cannon->blasts[slot] = blast_create(cannon->pos, cannon->rotation);

    cannon->last_blast = clock();
}

void cannon_update_and_draw(Cannon *cannon)
{
    for (int i = 0; i < MAX_BLASTS; i++)
    {
        if (cannon->blasts[i] != NULL && !cannon->blasts[i]->gone)
        {
            blast_update(cannon->blasts[i]);
            blast_draw(cannon->blasts[i]);
        }
    }
}

void cannon_check_collissions(Cannon *cannon,
                            Space *space,
                            int *impacted_asteroids)
{
    Blast **blasts = cannon->blasts;
    *impacted_asteroids = 0;

    for (int i = 0; i < MAX_BLASTS; i++)
    {
        if (blasts[i] != NULL && !blasts[i]->gone)
            *impacted_asteroids += _check_blast_collision(blasts[i], space);
    }
}

void cannon_destroy(Cannon *cannon)
{
    for (int i = 0; i < MAX_BLASTS; i++)
    {
        if (cannon->blasts[i] != NULL)
            blast_destroy(cannon->blasts[i]);
    }
    free(cannon);
}

static int _get_blast_available_slot(Cannon *cannon)
{
    int availableSlot = -1;
    for (int i = 0; i < MAX_BLASTS && availableSlot == -1; i++)
    {
        if (cannon->blasts[i] == NULL)
        {
            availableSlot = i;
        }
        else if (cannon->blasts[i]->gone)
        {
            blast_destroy(cannon->blasts[i]);
            cannon->blasts[i] = NULL;
            availableSlot = i;
        }
    }
    return availableSlot;
}

static int _check_blast_collision(Blast *blast, Space *space)
{
    Asteroid *next = NULL;
    Asteroid *asteroid = space->first_asteroid;
    int impacted_asteroids = 0;

    while (asteroid != NULL)
    {
        next = asteroid->next;
        if (collider_check(asteroid->collider, blast->collider))
        {
            impacted_asteroids++;
            _blast_hit_asteroid(blast, space, asteroid);
        }

        asteroid = next;
    }
    return impacted_asteroids;
}

static void _blast_hit_asteroid(Blast *blast,
                            Space *space,
                            Asteroid *impacted_asteroid)
{
    blast->gone = true;
    blast->state = BlastCollided;
    space_hit_asteroid(space, impacted_asteroid);
}

