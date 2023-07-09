#include "gameplay.h"

static Gameplay *gameplay;

static void _process_input();
static void _check_collisions();

void gameplay_init()
{
    hud_init();
    
    gameplay = (Gameplay *)malloc(sizeof(Gameplay));
    gameplay->lifes = 3;
    gameplay->score = 0;
    gameplay->delay_for_next_state = 0;
    
    gameplay->space = space_create();
    gameplay->spaceship = spaceship_create();
    spaceship_reset(gameplay->spaceship);
}

NextGameState gameplay_process_frame()
{
    _process_input();
    _check_collisions();
    space_update_and_draw(gameplay->space);
    spaceship_update_and_draw(gameplay->spaceship);
    hud_draw(gameplay->score, gameplay->lifes);
   
    if (gameplay->delay_for_next_state > 0)
        gameplay->delay_for_next_state--;

    return (gameplay->lifes == 0 && gameplay->delay_for_next_state == 0)
        ? NextGameStateOver
        : NextGameStateNone;
}

void gameplay_destroy()
{
    spaceship_destroy(gameplay->spaceship);
    space_destroy(gameplay->space);
    hud_destroy();
    free(gameplay);
}

static void _check_collisions()
{
    int impacted_asteroids = 0;
    bool spaceship_impacted = false;

    spaceship_check_collissions(gameplay->spaceship,
                            gameplay->space,
                            &spaceship_impacted,
                            &impacted_asteroids);

    gameplay->score += impacted_asteroids * 100;
    
    if (spaceship_impacted)
    {
        gameplay->lifes--;
        game_state_set_param(gameplay->score);

        if (gameplay->lifes == 0)
            gameplay->delay_for_next_state = 60 * 2;
    }
}

static void _process_input()
{
    unsigned char keys = input_get_key_down();
    if (keys & KEY_LEFT)
        spaceship_turn(gameplay->spaceship, false);
    if (keys & KEY_RIGHT)
        spaceship_turn(gameplay->spaceship, true);
    if (keys & KEY_THRUST)
        spaceship_thrust(gameplay->spaceship);
    if (keys & KEY_BREAK)
        spaceship_break(gameplay->spaceship);
    if (keys & KEY_SHOOT)
        spaceship_shoot(gameplay->spaceship);
}

