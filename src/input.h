#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "allegro.h"

#define KEY_LEFT    0x01
#define KEY_RIGHT   0x02
#define KEY_THRUST  0x04
#define KEY_BREAK   0x08
#define KEY_SHOOT   0x10
#define KEY_START   0x20

void input_init();
unsigned char input_get_key_down();
bool input_joystick_available();
void input_destroy();

#endif
