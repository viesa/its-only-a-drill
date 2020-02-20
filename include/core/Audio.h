#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

typedef enum SoundFile
{
    SF_Test,
    SF_Count
} SoundFile;

typedef struct Audio Audio;

Audio *AudioCreate();
void AudioDestroy(Audio *audio);

Mix_Chunk *AudioGet(Audio *audio, SoundFile soundFile);

int AudioGenChannel(Audio *audio);
void AudioFreeChannel(Audio *audio, int channel);

#endif