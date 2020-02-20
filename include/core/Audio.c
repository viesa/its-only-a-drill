#include "Audio.h"

#include <SDL2/SDL.h>

#define N_CHANNELS 128

struct Audio
{
    Mix_Chunk *m_chunks[SF_Count];
    SDL_bool m_availableChannels[N_CHANNELS];
};

Audio *AudioCreate()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        fprintf(stderr, "Error: Could not initialize audio");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
        fprintf(stderr, "Error: %s\n", Mix_GetError());

    Audio *ret = (Audio *)SDL_malloc(sizeof(Audio));

    //Audio files
    Mix_Chunk *test = Mix_LoadWAV("assets/sound/test.wav");
    if (!test)
        fprintf(stderr, "Error: Could not load test.wave!\n");
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