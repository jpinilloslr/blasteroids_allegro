#include "gameover.h"

static ALLEGRO_FONT *font;
static NextGameState next_state;

static void _process_input();
static void _draw();
static void _draw_title();
static void _draw_subtitle();
static void _draw_score();

void gameover_init()
{
    next_state = NextGameStateNone;
    font = al_create_builtin_font();
}

NextGameState gameover_process_frame()
{
    _process_input();
    _draw();
    return next_state;
}

void gameover_destroy()
{
    al_destroy_font(font);
}

static void _process_input()
{
    if(input_get_key_down() & KEY_START)
        next_state = NextGameStatePlay;
}

static void _draw()
{
    _draw_title();
    _draw_subtitle();
    _draw_score();
}

static void _draw_title()
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, 12.0f, 12.0f);
    al_translate_transform(&transform,
                        (SCREEN_WIDTH / 2.0f) - 435.0f,
                        (SCREEN_HEIGHT / 2.0f) - 100.0f);
    al_use_transform(&transform);

    al_draw_text(font,
                al_map_rgb(0, 255, 0),
                0,
                0,
                0,
                "Game Over");
}

static void _draw_subtitle()
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, 3.0f, 3.0f);
    al_translate_transform(&transform,
                        (SCREEN_WIDTH / 2.0f) - 150.0f,
                        (SCREEN_HEIGHT / 2.0f) + 80.0f);
    al_use_transform(&transform);

    char *msg = input_joystick_available()
                    ? "Press START"
                    : "Press ENTER";

    al_draw_text(font,
                al_map_rgb(0, 255, 0),
                0,
                0,
                0,
                msg);
}

static void _draw_score()
{
    int score = game_state_get_param();

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, 3.0f, 3.0f);
    al_translate_transform(&transform, 30.0f, 30.0f);
    al_use_transform(&transform);

    char msg[100];
    sprintf(msg, "High score: %d", score);

    al_draw_text(font,
                al_map_rgb(0, 255, 0),
                0,
                0,
                0,
                msg);
}

