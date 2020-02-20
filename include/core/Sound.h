#ifndef SOUND_H
#define SOUND_H

#include "Audio.h"

typedef struct Sound
{
    Audio *m_audio;
    Mix_Chunk *m_chunk;
    int m_channel;
} Sound;

Sound SoundCreate(Audio *audio, SoundFile soundFile);
void SoundDestroy(Sound *sound);

void SoundPlay(Sound sound, int loops);
void SoundStop(Sound sound);

#endif