#ifndef MUSIC_H
#define MUSIC_H

#include "Dependencies.h"
#include "Audio.h"

typedef struct Music
{
    Mix_Music *m_music;

    SDL_bool m_isPlaying;
} Music;

Music MusicCreate(MusicFile musicFile);
void MusicDestroy(Music *music);

/// \param loops: if loops == -1, player forever
/// Otherwise, play loops + 1 times
void MusicPlay(Music *music, int loops);
void MusicStop(Music *music);

#endif