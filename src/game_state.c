#include "game_state.h"

static int _param = 0;

void game_state_set_param(int param)
{
    _param = param;
}

int game_state_get_param()
{
    return _param;
}
