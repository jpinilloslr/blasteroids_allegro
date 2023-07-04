#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "input.h"
#include "game_state.h"
#include "hud.h"
#include "space.h"
#include "spaceship.h"

typedef struct
{
    int score;
    int lifes;
    Space *space;
    Spaceship *spaceship;
} Gameplay;

void gameplay_init();
NextGameState gameplay_process_frame();
void gameplay_destroy();

#endif
