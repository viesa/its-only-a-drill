#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

typedef enum SoundFile
{
    SF_Test,
    SF_Door,
    SF_Footsteps,
    SF_Gun,
    SF_Sms,
    SF_Knife,
    SF_Bomp,
    SF_Notification,
    SF_Count
} SoundFile;

typedef enum MusicFile
{
    MF_Test,
    MF_Count
} MusicFile;

typedef struct Audio Audio;

Audio *AudioCreate();
void AudioDestroy(Audio *audio);

Mix_Chunk *AudioGetSound(Audio *audio, SoundFile soundFile);
Mix_Music *AudioGetMusic(Audio *audio, MusicFile musicFile);

int AudioGenChannel(Audio *audio);
void AudioFreeChannel(Audio *audio, int channel);

#endif