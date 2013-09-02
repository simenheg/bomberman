#include "audio.h"

void uninit_audio(void)
{
  alDeleteSources(N_SOUNDS, sources);
  alcCloseDevice(dev);
}

int init_audio(void)
{
  int err;

  dev = alcOpenDevice(NULL);
  if (dev == NULL)
    fprintf(stderr, "Could not get device\n");

  context = alcCreateContext(dev, NULL);
  err = alcGetError(dev);
  if (context == NULL || err != ALC_NO_ERROR)
    fprintf(stderr, "Could not set context\n");

  alcMakeContextCurrent(context);
  err = alcGetError(dev);
  if (err != ALC_NO_ERROR)
    fprintf(stderr, "OpenAL error: %s", alGetString(err));

  alGenSources(N_SOUNDS, sources);

  err = alGetError();
  if (err != ALC_NO_ERROR)
    fprintf(stderr, "OpenAL error: %s", alGetString(err));

  alGenBuffers(N_SOUNDS, buffers);

  err = alGetError();
  if (err != ALC_NO_ERROR)
    fprintf(stderr, "OpenAL error: %s", alGetString(err));

  init_audio_files();
  init_listener();
  atexit(uninit_audio);
  
  return 0;
}

void init_audio_files(void)
{
  bind_vorbis("data/sound/explosion.ogg", SOUND_EXPLOSION);
  bind_vorbis("data/sound/powerup.ogg", SOUND_POWERUP);

  for (int i = 0; i < N_SOUNDS; ++i)
    alSourcei(sources[i], AL_BUFFER, buffers[i]);
}

/* Loads a vorbis file into a memory buffer and returns the format and
 * frequency. */
void load_vorbis(char *filename, char *buffer, ALenum *format, ALsizei *freq,
                 int *len)
{
  int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
  int bit_stream;
  long bytes;
  char array[BUFFER_SIZE];
  FILE *f;

  f = fopen(filename, "rb");

  if (f == NULL)
    {
      fprintf(stderr, "Could not open \"%s\" for reading.\n", filename);
      return;
    }

  OggVorbis_File file;

  // Try opening the given file
  if (ov_open(f, &file, NULL, 0) != 0)
    {
      fprintf(stderr, "Error opening %s for decoding\n", filename);
      return;
    }

  vorbis_info *pInfo = ov_info(&file, -1);
  *freq = pInfo->rate;
  if (pInfo->channels == 1)
    *format = AL_FORMAT_MONO16;
  else
    *format = AL_FORMAT_STEREO16;

  int offset = 0;

  do
    {
      bytes = ov_read(&file, array, BUFFER_SIZE, endian, 2, 1,
                      &bit_stream);

      if (bytes < 0)
        {
          ov_clear(&file);
          fprintf(stderr, "Error decoding %s\n", filename);
          return;
        }

      memcpy(buffer + offset, array, bytes);
      offset += bytes;

    } while (bytes > 0);

  *len = offset;
  ov_clear(&file);
}

void init_listener(void)
{
  ALfloat pos[3] = {0.5, 0.5, 0.0};
  ALfloat ori[6] = {0.0, 0.0, -1.0,
                    0.0, 1.0, 0.0};

  alListenerfv(AL_POSITION, pos);
  alListenerfv(AL_ORIENTATION, ori);

  int err = alGetError();
  if (err != ALC_NO_ERROR)
    fprintf(stderr, "OpenAL error: %s", alGetString(err));
}

void bind_vorbis(char *filename, SOUND_EFFECT se)
{
  char    *data = calloc(100000, sizeof(char));
  ALenum   format;
  ALsizei  freq;
  int      len;

  load_vorbis(filename, data, &format, &freq, &len);
  alBufferData(buffers[se], format, data, len, freq);
  free(data);
}

void play_sound(int x, int y, SOUND_EFFECT se)
{
  ALfloat pos[3] = {(float)x/tile_x, (float)y/tile_y, 0.0};
  alSourcefv(sources[se], AL_POSITION, pos);
  alSourcePlay(sources[se]);

  int err = alGetError();
  if (err != ALC_NO_ERROR)
    fprintf(stderr, "OpenAL error: %s", alGetString(err));
}
