#ifndef SETTINGS_H
#define SETTINGS_H
#include "FileIO.h"
#include "Keybinding.h"
#define SETTINGS_PATH "settings.bin"

typedef struct Settings
{
    int skin, resolutionW, resolutionH;
    Keybinding keys;
    SDL_bool isFullscreen, vsync;
    size_t fps;
} Settings;
Settings SettingsCreate(int skin, int resolutionW, int resolutionH, Keybinding keys, SDL_bool isFullscreen, SDL_bool vsync, size_t fps);
Settings SettingsGetFromFile(char path[]);
void SettingsSave(Settings settings);
void SettingsDestroy(Settings *settings);
#endif