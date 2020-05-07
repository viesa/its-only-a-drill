#ifndef SOUND_H
#define SOUND_H

#include "Dependencies.h"

#include "Audio.h"

typedef struct Sound
{
    Mix_Chunk *m_chunk;
    int m_channel;

    SDL_bool m_isPlaying;
} Sound;

Sound SoundCreate(SoundFile soundFile);
void SoundDestroy(Sound *sound);

/// \param loops: if loops == -1, player forever
/// Otherwise, play loops + 1 times
void SoundPlay(Sound *sound, int loops);
void SoundStop(Sound *sound);

#endif