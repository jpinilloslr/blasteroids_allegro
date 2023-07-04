#include "title_screen.h"

static ALLEGRO_FONT *font;
static TitleScreen* title_screen;

static void _process_input();
static void _update();
static void _draw();
static void _draw_title();
static void _draw_subtitle();

void title_screen_init()
{
    font = al_create_builtin_font();

    title_screen = (TitleScreen *)malloc(sizeof(TitleScreen));
    title_screen->timer = 0;
    title_screen->subtitle_toggle = false;
    title_screen->next_state = NextGameStateNone;
}

NextGameState title_screen_process_frame()
{
    _update();
    _draw();
    _process_input();
    return title_screen->next_state;
}

void title_screen_destroy()
{
    al_destroy_font(font);
    free(title_screen);
}

void _update()
{
    if (clock() - title_screen->timer >= 10000)
    {
        title_screen->timer = clock();
        title_screen->subtitle_toggle = !title_screen->subtitle_toggle;
    }
}

static void _process_input()
{
    if(input_get_key_down() & KEY_START)
        title_screen->next_state = NextGameStatePlay;
}

static void _draw()
{
    _draw_title();
    if (title_screen->subtitle_toggle)
        _draw_subtitle();
}

static void _draw_title()
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, 10.0f, 10.0f);
    al_translate_transform(&transform,
                        (SCREEN_WIDTH / 2.0f) - 450.0f,
                        (SCREEN_HEIGHT / 2.0f) - 100.0f);
    al_use_transform(&transform);

    al_draw_text(font,
                al_map_rgb(0, 255, 0),
                0,
                0,
                0,
                "BLASTEROIDS");
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

    const char *msg = input_joystick_available()
                    ? "Press START"
                    : "Press ENTER";

    al_draw_text(font,
                al_map_rgb(0, 255, 0),
                0,
                0,
                0,
                msg);
}
