#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include "allegro.h"

#define RESERVED_SAMPLES 16
#define MAX_SAMPLE_DATA 4

typedef enum
{
    SoundBlast,
    SoundExplosionSmall,
    SoundExplosionMedium,
    SoundExplosionLarge,
} Sound;

void audio_init();
void audio_play_loop(Sound sound);
void audio_play_once(Sound sound);
void audio_destroy();

#endif
