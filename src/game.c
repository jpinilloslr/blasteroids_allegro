#include "game.h"

static GameState *_game_state;
static void _set_next_game_state();

void game_init()
{
    audio_init();
    input_init();
    
    _game_state = (GameState *)malloc(sizeof(GameState));
    _game_state->init = NULL;
    _game_state->process_frame = NULL;
    _game_state->destroy = NULL;
    _game_state->next_state = NextGameStateTitle;
}

void game_process_frame()
{
    if (_game_state->process_frame != NULL)
        _game_state->next_state = _game_state->process_frame();
    _set_next_game_state();
}

void game_destroy()
{
    input_destroy();
    audio_destroy();
    
    if (_game_state->destroy != NULL)
        _game_state->destroy();
    
    free(_game_state);
}

static void _set_next_game_state()
{
    if (_game_state->next_state == NextGameStateNone)
        return;

    if (_game_state->destroy != NULL)
        _game_state->destroy();

    switch (_game_state->next_state)
    {
       case NextGameStatePlay:
            _game_state->init = gameplay_init;
            _game_state->process_frame = gameplay_process_frame;
            _game_state->destroy = gameplay_destroy;
            break;
        case NextGameStateOver:
            _game_state->init = gameover_init;
            _game_state->process_frame = gameover_process_frame;
            _game_state->destroy = gameover_destroy;
            break;
        default:
            _game_state->init = title_screen_init;
            _game_state->process_frame = title_screen_process_frame;
            _game_state->destroy = title_screen_destroy;
    }
    _game_state->next_state = NextGameStateNone;
    _game_state->init();
}
