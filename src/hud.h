#ifndef HUD_H
#define HUD_H

#include <stdio.h>
#include "allegro.h"
#include "math_utils.h"

void hud_init();
void hud_draw(int score, int lifes);
void hud_destroy();

#endif
