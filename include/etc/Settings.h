#ifndef SETTINGS_H
#define SETTINGS_H
#include "FileIO.h"
// Only for storage purposes
#include "Keybinding.h"
// Only for storage purposes
#define SETTINGS_PATH "settings.bin"

typedef struct Settings
{
    int skin, resolutionW, resolutionH;
    Keybinding keys;
    SDL_bool isFullscreen, vsync;
    size_t fps;
} Settings;
// Generates a settings structure based on the arguments (not allocated)
Settings SettingsCreate(int skin, int resolutionW, int resolutionH, Keybinding keys, SDL_bool isFullscreen, SDL_bool vsync, size_t fps);
// Generates a settings structure based on the contents of a file (if failed to parse file the resolutionW is 1)
Settings SettingsGetFromFile(char path[]);
// Saves the settings to SETTINGS_PATH
void SettingsSave(Settings settings);
// Not needed because nothing is allocated in settings
void SettingsDestroy(Settings *settings);
#endif