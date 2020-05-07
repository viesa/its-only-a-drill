#include "Music.h"

Music MusicCreate(MusicFile musicFile)
{
    Music ret;
    ret.m_music = AudioGetMusic(musicFile);
    ret.m_isPlaying = SDL_FALSE;
    return ret;
}
void MusicDestroy(Music *music)
{
}

void MusicPlay(Music *music, int loops)
{
    if (!music->m_isPlaying)
    {
        if (Mix_PlayMusic(music->m_music, loops) == -1)
        {
            log_warn("Could not play music: %s", Mix_GetError());
        }
        else
        {
            music->m_isPlaying = SDL_TRUE;
        }
    }
}
void MusicStop(Music *music)
{
    if (music->m_isPlaying)
    {
        if (Mix_HaltMusic() == -1)
        {
            log_warn("Could not halt music: %s", Mix_GetError());
        }
        else
        {
            music->m_isPlaying = SDL_FALSE;
        }
    }
}