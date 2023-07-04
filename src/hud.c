#include "hud.h"

ALLEGRO_FONT *font;
ALLEGRO_COLOR color;

void hud_init()
{
    font = al_create_builtin_font();
    color = al_map_rgb(255, 255, 255);
}

void _draw_life(Vector2D pos)
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_translate_transform(&transform, pos.x, pos.y);
    al_use_transform(&transform);

    al_draw_line(-8, 9, 0, -11, color, 3.0f);
    al_draw_line(0, -11, 8, 9, color, 3.0f);
    al_draw_line(-6, 4, -1, 4, color, 3.0f);
    al_draw_line(6, 4, 1, 4, color, 3.0f);
}

void hud_draw(int score, int lifes)
{
    char buffer[20];
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, 2.5f, 2.5f);
    al_use_transform(&transform);

    sprintf(buffer, "%d", score);
    al_draw_text(font, color, 10, 7, 0, buffer);

    for (int i = 0; i < lifes; i++)
        _draw_life((Vector2D){30.0f + (i * 20.0f), 60.0f});
}

void hud_destroy()
{
    al_destroy_font(font);
}
