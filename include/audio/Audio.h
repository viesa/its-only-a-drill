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
    SF_Count
} SoundFile;

// Enum used in array of pre-loaded music
typedef enum MusicFile
{
    MF_Test,
    MF_MainMusic,
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

#endif