#include "Sound.h"

Sound SoundCreate(SoundFile soundFile)
{
    Sound ret;
    ret.m_chunk = AudioGetSound(soundFile);
    ret.m_channel = AudioGenChannel();
    ret.m_isPlaying = SDL_FALSE;
    return ret;
}
void SoundDestroy(Sound *sound)
{
    AudioFreeChannel(sound->m_channel);
}

void SoundPlay(Sound *sound, int loops)
{
    if (!sound->m_isPlaying)
    {
        if (Mix_PlayChannel(sound->m_channel, sound->m_chunk, loops) == -1)
        {
            log_warn("Could not play channel: %s", Mix_GetError());
        }
        else
        {
            sound->m_isPlaying = SDL_TRUE;
        }
    }
}
void SoundStop(Sound *sound)
{
    if (sound->m_isPlaying)
    {
        if (Mix_HaltChannel(sound->m_channel) == -1)
        {
            log_warn("Could not halt channel: %s", Mix_GetError());
        }
        else
        {
            sound->m_isPlaying = SDL_FALSE;
        }
    }
}