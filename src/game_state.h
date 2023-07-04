#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum 
{ 
    NextGameStateNone,
    NextGameStateTitle,
    NextGameStatePlay,
    NextGameStateOver
} NextGameState;

typedef void (*game_state_init_fn)(void);
typedef NextGameState (*game_state_process_frame_fn)(void);
typedef void (*game_state_destroy_fn)(void);

typedef struct
{
    game_state_init_fn init;
    game_state_process_frame_fn process_frame;
    game_state_destroy_fn destroy;
    NextGameState next_state;
} GameState;

#endif
