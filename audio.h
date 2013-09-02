#ifndef AUDIO_H
#define AUDIO_H

#include <vorbis/vorbisfile.h>
#include <AL/alc.h>
#include <AL/al.h>

#include "common.h"
#include "map.h"

#define BUFFER_SIZE 32768 // 32 KB buffers

typedef enum {
  SOUND_EXPLOSION = 0,
  SOUND_POWERUP,
  N_SOUNDS
} SOUND_EFFECT;

ALCdevice  *dev;
ALCcontext *context;
ALuint      sources[N_SOUNDS];
ALuint      buffers[N_SOUNDS];

int init_audio(void);
void init_audio_files(void);
void load_vorbis(char *filename, char *buffer, ALenum *format, ALsizei *freq,
                 int *len);
void init_listener(void);
void bind_vorbis(char *filename, SOUND_EFFECT se);
void start_audio(void);
void play_sound(int x, int y, SOUND_EFFECT se);

#endif
