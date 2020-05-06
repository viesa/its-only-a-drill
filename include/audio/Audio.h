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
    MF_Count
} MusicFile;

typedef struct Audio Audio;

Audio *AudioCreate();
void AudioDestroy(Audio *audio);

/// Returns a pre-loaded soundfile (.wav)
Mix_Chunk *AudioGetSound(Audio *audio, SoundFile soundFile);
/// Returns a pre-loaded musicfile (.mp3)
Mix_Music *AudioGetMusic(Audio *audio, MusicFile musicFile);

/// Generates a channel for a sound to play on
int AudioGenChannel(Audio *audio);
/// Free a channel when it is no longer used
void AudioFreeChannel(Audio *audio, int channel);

#endif