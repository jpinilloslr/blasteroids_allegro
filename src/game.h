#ifndef GAME_H
#define GAME_H

#include "allegro.h"
#include "audio.h"
#include "input.h"
#include "game_state.h"
#include "title_screen.h"
#include "gameplay.h"
#include "gameover.h"

void game_init();
void game_process_frame();
void game_destroy();

#endif
