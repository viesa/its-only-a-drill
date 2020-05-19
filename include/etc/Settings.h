#ifndef SETTINGS_H
#define SETTINGS_H
#include "FileIO.h"
// Only for storage purposes
#include "Keybinding.h"
#include "Player.h"
#include "Window.h"
#include "FPSManager.h"
// Only for storage purposes
#define SETTINGS_PATH "settings.bin"

void SettingsInitialize();
void SettingsUninitialize();

// Generates a Settings based on current settings in the game
void SettingsGenerate();
// Try generates a Settings based on the contents of a file
SDL_bool SettingsTryLoad(char *path);
// Applies the Settings in the Settings structure
void SettingsApply();
// Saves the Settings to path
void SettingsSave(char *path);

#endif