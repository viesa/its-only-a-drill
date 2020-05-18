#include "Settings.h"
Settings SettingsCreate(int skin, int resolutionW, int resolutionH, SDL_bool isFullscreen, SDL_bool vsync, size_t fps, SDL_Scancode keys[])
{
    Settings settings;

    settings.resolutionW = resolutionW;
    settings.resolutionH = resolutionH;

    settings.skin = skin;

    settings.isFullscreen = isFullscreen;

    settings.vsync = vsync;

    settings.fps = fps;

    SDL_memcpy(settings.keys, keys, sizeof(SDL_Scancode) * (AC_Count - 1));
    return settings;
}
void SettingsSave(Settings settings)
{
    FileIO file = FileIOCreate(SETTINGS_PATH);
    file.contents = (char *)MALLOC(Settings);
    SDL_memcpy(file.contents, &settings, sizeof(Settings));
    FileIOWriteBinary(&file);
    FileIODestroy(&file);
}
void SettingsApply(Player *player)
{
    Settings settings = SettingsGetFromFile(SETTINGS_PATH);
    if (settings.resolutionH != 1) // found settings file
    {
        WindowSetSize(settings.resolutionW, settings.resolutionH); //resolution

        PlayerSetSpriteSheet(player, (SpriteSheet)settings.skin); // skin

        WindowSetFullscreen(settings.isFullscreen); // fullscreen

        WindowSetVSync(settings.vsync); // vsync

        FPSManagerSetDesiredFPS(settings.fps); // fps

        SDL_memcpy(KeybindingGetKeys(), &settings.keys, sizeof(SDL_Scancode) * (AC_Count - 1)); // keybindings
    }
    SettingsDestroy(&settings);
}
Settings SettingsGetFromFile(char path[])
{
    Settings settings;
    settings.resolutionH = 1;
    FileIO file = FileIOCreate(path);
    if (!FileIOReadBinary(&file))
    {
        FileIODestroy(&file);
        return settings;
    }
    settings = *(Settings *)file.contents;
    FileIODestroy(&file);
    return settings;
}
void SettingsDestroy(Settings *settings)
{
}