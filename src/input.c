#include "input.h"

unsigned char _get_joystick_key();
unsigned char _get_keyboard_key();

void input_init()
{
    al_install_keyboard();
    al_install_joystick();
}

unsigned char input_get_key_down()
{
    return (input_joystick_available())
            ? _get_joystick_key()
            : _get_keyboard_key();
}

bool input_joystick_available()
{
    return (al_get_num_joysticks() > 0);
}

void input_destroy()
{
}

unsigned char _get_joystick_key()
{
    unsigned char keys = 0;
    ALLEGRO_JOYSTICK_STATE joystickState;
    al_get_joystick_state(al_get_joystick(0), &joystickState);
    
    if (joystickState.stick[3].axis[0] > 0.0f)
        keys |= KEY_BREAK;
    if (joystickState.stick[2].axis[0] > 0.0f)
        keys |= KEY_THRUST;
    if (joystickState.stick[0].axis[0] < -0.3f)
        keys |= KEY_LEFT;
    if (joystickState.stick[0].axis[0] > 0.3f)
        keys |= KEY_RIGHT;
    if (joystickState.button[0] > 0)
        keys |= KEY_SHOOT;
    if (joystickState.button[11] > 0)
        keys |= KEY_START;

    return keys;
}

unsigned char _get_keyboard_key()
{
    unsigned char keys = 0;
    ALLEGRO_KEYBOARD_STATE keyboardState;

    al_get_keyboard_state(&keyboardState);
    if (al_key_down(&keyboardState, ALLEGRO_KEY_LEFT))
        keys |= KEY_LEFT;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_RIGHT))
        keys |= KEY_RIGHT;   
    if (al_key_down(&keyboardState, ALLEGRO_KEY_UP))
        keys |= KEY_THRUST;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_DOWN))
        keys |= KEY_BREAK;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_SPACE))
        keys |= KEY_SHOOT;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_ENTER))
        keys |= KEY_START;

    return keys;
}
