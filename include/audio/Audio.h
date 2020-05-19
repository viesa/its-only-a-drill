#ifndef AUDIO_H
#define AUDIO_H

#include "Dependencies.h"

// Enum used in array of pre-loaded sounds
typedef enum SoundFile
{
    SF_Test,
    SF_Door,
    SF_Footsteps,
    SF_Gun,
    SF_Sms,
    SF_Knife,
    SF_Bomp,
    SF_Notification,
    SF_MenuPress,
    SF_MenuStep,
    SF_PistolShoot,
    SF_M4A1Shoot,
    SF_ShotgunShoot,
    SF_SniperShoot,
    SF_ReloadMag,
    SF_ReloadShotgun,
    SF_SwordSwing,
    SF_Nothing,
    SF_Count
} SoundFile;

// Enum used in array of pre-loaded music
typedef enum MusicFile
{
    MF_Test,
    MF_MainMusic,
    MF_MainMusicTwo,
    MF_GameMusic,
    MF_Count
} MusicFile;

void AudioInitialize();
void AudioUninitialize();

/// Returns a pre-loaded soundfile (.wav)
Mix_Chunk *AudioGetSound(SoundFile soundFile);
/// Returns a pre-loaded musicfile (.mp3)
Mix_Music *AudioGetMusic(MusicFile musicFile);

/// Generates a channel for a sound to play on
int AudioGenChannel();
/// Free a channel when it is no longer used
void AudioFreeChannel(int channel);

double AudioGetMasterVolume();
Uint8 AudioGetMusicVolume();
Uint8 AudioGetSFXVolume();

void AudioSetMasterVolume(double multiplier);
void AudioSetSFXVolume(Uint8 volume);
void AudioSetMusicVolume(Uint8 volume);

Mix_Chunk *AudioLoadSound(char *path);
Mix_Music *AudioLoadMusic(char *path);

#endif