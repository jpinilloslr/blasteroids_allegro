#ifndef GAMEOVER_H
#define GAMEOVER_H 

#include <stdio.h>
#include "input.h"
#include "allegro.h"
#include "screen.h"
#include "game_state.h"

void gameover_init();
NextGameState gameover_process_frame();
void gameover_destroy();

#endif
