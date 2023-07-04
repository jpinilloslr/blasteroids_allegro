#include "audio.h"

static ALLEGRO_SAMPLE *_sample_data[MAX_SAMPLE_DATA] = {NULL};

void audio_init()
{
  al_init_acodec_addon();

  if (!al_install_audio())
  {
    printf("[Error] Could not init sound!\n");
    exit(1);
  }

  if (!al_reserve_samples(RESERVED_SAMPLES))
  {
    printf("[Error] Could not set up voice and mixer.\n");
    exit(1);
  }

  memset(_sample_data, 0, sizeof(_sample_data));
  char sound_files[][20] = {
      "data/blast.wav",
      "data/exp_small.wav",
      "data/exp_medium.wav",
      "data/exp_big.wav"};

  for (int i = 0; i < MAX_SAMPLE_DATA; i++)
  {
    _sample_data[i] = al_load_sample(sound_files[i]);
    if (!_sample_data[i])
      printf("[Warning] Could not load sample \"%s\".\n", sound_files[i]);
  }
}

void audio_play_loop(Sound sound)
{
  al_play_sample(_sample_data[sound], 1.0, 0.0, 1.0,
                 ALLEGRO_PLAYMODE_LOOP, NULL);
}

void audio_play_once(Sound sound)
{
  al_play_sample(_sample_data[sound], 1.0, 0.0, 1.0,
                 ALLEGRO_PLAYMODE_ONCE, NULL);
}

void audio_destroy()
{
  for (int i = 0; i < MAX_SAMPLE_DATA; i++)
  {
    if (_sample_data[i])
      al_destroy_sample(_sample_data[i]);
  }
  al_uninstall_audio();
}
