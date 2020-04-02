#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_bits.h>

#include "Audio.h"

typedef struct Music
{
    Audio *m_audio;
    Mix_Music *m_music;

    SDL_bool m_isPlaying;
} Music;

Music MusicCreate(Audio *audio, MusicFile musicFile);
void MusicDestroy(Music *music);

/// \param loops: if loops == -1, player forever
/// Otherwise, play loops + 1 times
void MusicPlay(Music *music, int loops);
void MusicStop(Music *music);

#endif