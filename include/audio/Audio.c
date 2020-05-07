#include "Audio.h"

#define N_CHANNELS 128

struct Audio
{
    Mix_Chunk *m_chunks[SF_Count];
    Mix_Music *m_music[MF_Count];
    SDL_bool m_availableChannels[N_CHANNELS];
} audio;

void AudioInitialize()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
        log_error("Could configure audio: %s", Mix_GetError());

    //-------------------------------------------------
    //------------------ Sound files ------------------
    //-------------------------------------------------
    Mix_Chunk *testSound = Mix_LoadWAV("assets/sound/test.wav");
    if (!testSound)
        log_warn("Could not load test.wav");
    audio.m_chunks[SF_Test] = testSound;

    Mix_Chunk *door = Mix_LoadWAV("assets/sound/doorOpen.wav");
    if (!door)
        log_warn("Could not load doorOpen.wav");
    audio.m_chunks[SF_Door] = door;

    Mix_Chunk *foot = Mix_LoadWAV("assets/sound/footsteps.wav");
    if (!foot)
        log_warn("Could not load footsteps.wav");
    audio.m_chunks[SF_Footsteps] = foot;

    Mix_Chunk *gun = Mix_LoadWAV("assets/sound/gun.wav");
    if (!gun)
        log_warn("Could not load gun.wav");
    audio.m_chunks[SF_Gun] = gun;

    Mix_Chunk *knife = Mix_LoadWAV("assets/sound/knife.wav");
    if (!knife)
        log_warn("Could not load knife.wav");
    audio.m_chunks[SF_Knife] = knife;

    Mix_Chunk *bomp = Mix_LoadWAV("assets/sound/Bomb_Exploding.wav");
    if (!bomp)
        log_warn("Could not load Bomb_Exploding.wav");
    audio.m_chunks[SF_Bomp] = bomp;

    Mix_Chunk *sms = Mix_LoadWAV("assets/sound/sms-alert.wav");
    if (!sms)
        log_warn("Could not load sms-alert.wav");
    audio.m_chunks[SF_Sms] = sms;

    Mix_Chunk *notification = Mix_LoadWAV("assets/sound/notification.wav");
    if (!notification)
        log_warn("Could not load notification.wav");
    audio.m_chunks[SF_Notification] = notification;

    Mix_Chunk *menupress = Mix_LoadWAV("assets/sound/Menu_Press.wav");
    if (!menupress)
        log_warn("Could not load Menu_Press.wav");
    audio.m_chunks[SF_MenuPress] = menupress;

    Mix_Chunk *menustep = Mix_LoadWAV("assets/sound/Menu_Step.wav");
    if (!menustep)
        log_warn("Could not load Menu_Step.wav");
    audio.m_chunks[SF_MenuStep] = menustep;

    //-------------------------------------------------
    //------------------ Music files ------------------
    //-------------------------------------------------

    //This always fails in mac environment, if someone has a fix, please fix.
    /*Mix_Music *testMusic = Mix_LoadMUS("./assets/music/test.mp3");
    if (!testMusic)
        log_warn("Could not load test.mp3");
    audio.m_music[MF_Test] = testMusic;*/

    Mix_Music *mainMusic = Mix_LoadMUS("assets/music/Omega Spectrum - Spectrum.mp3");
    if (!mainMusic)
        log_warn("Could not load test.mp3");
    audio.m_music[MF_MainMusic] = mainMusic;

    for (int i = 0; i < N_CHANNELS; i++)
        audio.m_availableChannels[i] = SDL_TRUE;
}

void AudioUninitialize()
{
    for (int i = 0; i < SF_Count; i++)
        Mix_FreeChunk(audio.m_chunks[i]);
    Mix_CloseAudio();
}

Mix_Chunk *AudioGetSound(SoundFile soundFile)
{
    return audio.m_chunks[soundFile];
}

Mix_Music *AudioGetMusic(MusicFile musicFile)
{
    return audio.m_music[musicFile];
}

int AudioGenChannel()
{
    for (int i = 0; i < N_CHANNELS; i++)
        if (audio.m_availableChannels[i])
        {
            audio.m_availableChannels[i] = SDL_FALSE;
            return i;
        }
    return -1;
}

void AudioFreeChannel(int channel)
{
    audio.m_availableChannels[channel] = SDL_TRUE;
}