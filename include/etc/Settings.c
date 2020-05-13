#include "Settings.h"
Settings SettingsCreate(int skin, int resolutionW, int resolutionH, Keybinding keys)
{
    Settings settings;

    settings.resolutionW = resolutionW;
    settings.resolutionH = resolutionH;

    settings.skin = skin;
    settings.keys = keys;

    return settings;
}
void SettingsSave(Settings settings)
{
    FileIO file = FileIOCreate(SETTINGS_PATH);
    file.contents = (char *)MALLOC(Settings);
    SDL_memcpy(file.contents, &settings, sizeof(Settings));
    FileIOWriteBinary(&file);
    FileIODestroy(file);
}
Settings SettingsGetFromFile(char path[])
{
    Settings settings;
    settings.resolutionH = 1;
    FileIO file = FileIOCreate(path);
    if (!FileIOReadBinary(&file))
    {
        FileIODestroy(file);
        return settings;
    }
    settings = *(Settings *)file.contents;
    FileIODestroy(file);
    return settings;
}
void SettingsDestroy(Settings *settings)
{
}