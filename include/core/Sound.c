#include "Sound.h"

#include "Log.h"

Sound SoundCreate(Audio *audio, SoundFile soundFile)
{
    Sound ret;
    ret.m_audio = audio;
    ret.m_chunk = AudioGet(audio, soundFile);
    ret.m_channel = AudioGenChannel(audio);
    return ret;
}
void SoundDestroy(Sound *sound)
{
    AudioFreeChannel(sound->m_audio, sound->m_channel);
}

void SoundPlay(Sound sound, int loops)
{
    if (Mix_PlayChannel(sound.m_channel, sound.m_chunk, loops) == -1)
    {
        log_warn("Could not play channel: %s", Mix_GetError());
    }
}
void SoundStop(Sound sound)
{
    if (Mix_HaltChannel(sound.m_channel) == -1)
    {
        log_warn("Could not halt channel: %s", Mix_GetError());
    }
}