#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include <time.h>
#include <stdbool.h>
#include "screen.h"
#include "allegro.h"
#include "input.h"
#include "game_state.h"

typedef struct
{
    clock_t timer;
    bool subtitle_toggle;
    NextGameState next_state;
} TitleScreen;

void title_screen_init();
NextGameState title_screen_process_frame();
void title_screen_destroy();

#endif
