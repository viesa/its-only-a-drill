#include "Audio.h"

#include <SDL2/SDL.h>

#include "Log.h"

#define N_CHANNELS 128

struct Audio
{
    Mix_Chunk *m_chunks[SF_Count];
    SDL_bool m_availableChannels[N_CHANNELS];
};

Audio *AudioCreate()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        log_error("Could not initialize audio: %s", SDL_GetError());

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
        log_error("Could configure audio: %s", Mix_GetError());

    Audio *ret = (Audio *)SDL_malloc(sizeof(Audio));

    //Audio files
    Mix_Chunk *test = Mix_LoadWAV("assets/sound/test.wav");
    if (!test)
        log_warn("Could not log test.wave");
    ret->m_chunks[SF_Test] = test;

    for (int i = 0; i < N_CHANNELS; i++)
        ret->m_availableChannels[i] = SDL_TRUE;
    return ret;
}

void AudioDestroy(Audio *audio)
{
    for (int i = 0; i < SF_Count; i++)
        Mix_FreeChunk(audio->m_chunks[i]);
    Mix_Quit();
    Mix_CloseAudio();
    SDL_free(audio);
}

Mix_Chunk *AudioGet(Audio *audio, SoundFile soundFile)
{
    return audio->m_chunks[soundFile];
}

int AudioGenChannel(Audio *audio)
{
    for (int i = 0; i < N_CHANNELS; i++)
        if (audio->m_availableChannels[i])
        {
            audio->m_availableChannels[i] = SDL_FALSE;
            return i;
        }
    return -1;
}

void AudioFreeChannel(Audio *audio, int channel)
{
    audio->m_availableChannels[channel] = SDL_TRUE;
}