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

    for (int i = 0; i < N_CHANNELS; i++)
        audio.m_availableChannels[i] = SDL_TRUE;

    //-------------------------------------------------
    //------------------ Sound files ------------------
    //-------------------------------------------------
    audio.m_chunks[SF_Test] = AudioLoadSound("assets/sound/test.wav");
    audio.m_chunks[SF_Door] = AudioLoadSound("assets/sound/doorOpen.wav");
    audio.m_chunks[SF_Footsteps] = AudioLoadSound("assets/sound/footsteps.wav");
    audio.m_chunks[SF_Gun] = AudioLoadSound("assets/sound/gun.wav");
    audio.m_chunks[SF_Knife] = AudioLoadSound("assets/sound/knife.wav");
    audio.m_chunks[SF_Bomp] = AudioLoadSound("assets/sound/Bomb_Exploding.wav");
    audio.m_chunks[SF_Sms] = AudioLoadSound("assets/sound/sms-alert.wav");
    audio.m_chunks[SF_Notification] = AudioLoadSound("assets/sound/notification.wav");
    audio.m_chunks[SF_MenuPress] = AudioLoadSound("assets/sound/Menu_Press.wav");
    audio.m_chunks[SF_MenuStep] = AudioLoadSound("assets/sound/Menu_Step.wav");
    audio.m_chunks[SF_PistolShoot] = AudioLoadSound("assets/sound/9MM.wav");
    audio.m_chunks[SF_M4A1Shoot] = AudioLoadSound("assets/sound/M4A1.wav");
    audio.m_chunks[SF_ShotgunShoot] = AudioLoadSound("assets/sound/SHOTGUN.wav");
    audio.m_chunks[SF_SniperShoot] = AudioLoadSound("assets/sound/SNIPER.wav");
    audio.m_chunks[SF_ReloadMag] = AudioLoadSound("assets/sound/RELOAD_General_Clip.wav");
    audio.m_chunks[SF_ReloadShotgun] = AudioLoadSound("assets/sound/SHOTGUN_Reload.wav");
    audio.m_chunks[SF_SwordSwing] = AudioLoadSound("assets/sound/SWORD.wav");
    audio.m_chunks[SF_Nothing] = AudioLoadSound("assets/sound/NOTHING.wav");

    //-------------------------------------------------
    //------------------ Music files ------------------
    //-------------------------------------------------
    audio.m_music[MF_MainMusic] = AudioLoadMusic("assets/music/Omega Spectrum - Spectrum.mp3");
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

void AudioSetMaster(double multiplier)
{
    for (int i = 0; i < SF_Count; i++)
    {
        audio.m_chunks[i]->volume *= multiplier;
    }

    Mix_VolumeMusic(Mix_VolumeMusic(-1) * multiplier);
}
void AudioSetSFX(Uint8 volume)
{
    for (int i = 0; i < SF_Count; i++)
    {
        audio.m_chunks[i]->volume = volume;
    }
}

void AudioSetMusic(Uint8 volume)
{
    Mix_VolumeMusic(volume);
}

Mix_Chunk *AudioLoadSound(char *path)
{
    Mix_Chunk *sound = Mix_LoadWAV(path);
    if (!sound)
        log_warn("Could not load %s", path);
    return sound;
}
Mix_Music *AudioLoadMusic(char *path)
{
    Mix_Music *music = Mix_LoadMUS(path);
    if (!music)
        log_warn("Could not load %s", path);
    return music;
}