#include "Settings.h"

typedef struct Settings
{
    SpriteSheet playerSkin;
    int windowWidth, windowHeight;
    SDL_bool isFullscreen, vsyncEnabled;
    size_t desiredFPS;
    SDL_Scancode bidnings[AC_Count];
    double volumeMaster;
    Uint8 volumeSFX;
    Uint8 volumeMusic;
} Settings;

static Settings *settings;

void SettingsInitialize()
{
    settings = MALLOC(Settings);
    ALLOC_ERROR_CHECK(settings);
    *settings = (Settings){0};
}

void SettingsUninitialize()
{
    FREE(settings);
}

void SettingsGenerate()
{
    settings->playerSkin = PlayerGetSkin();
    settings->windowWidth = WindowGetWidth();
    settings->windowHeight = WindowGetHeight();
    settings->isFullscreen = WindowIsFullscreen();
    settings->vsyncEnabled = WindowIsVSyncEnabled();
    settings->desiredFPS = FPSManagerGetDesiredFPS();
    settings->volumeMaster = AudioGetMasterVolume();
    settings->volumeSFX = AudioGetSFXVolume();
    settings->volumeMusic = AudioGetMusicVolume();
    SDL_memcpy(settings->bidnings, KeybindingGetKeys(), sizeof(SDL_Scancode) * AC_Count);
}

void SettingsApply()
{
    Settings notInitialized = {0};
    if (!SDL_memcmp(settings, &notInitialized, sizeof(Settings)))
    {
        if (!SettingsTryLoad(SETTINGS_PATH))
        {
            SettingsGenerate();
        }
    }
    PlayerSetSpriteSheet(settings->playerSkin);
    WindowSetSize(settings->windowWidth, settings->windowHeight);
    WindowSetFullscreen(settings->isFullscreen);
    WindowSetVSync(settings->vsyncEnabled);
    FPSManagerSetDesiredFPS(settings->desiredFPS);
    SDL_memcpy(KeybindingGetKeys(), &settings->bidnings, sizeof(SDL_Scancode) * (AC_Count - 1));
    AudioSetMasterVolume(settings->volumeMaster);
    AudioSetSFXVolume(settings->volumeSFX);
    AudioSetMusicVolume(settings->volumeMusic);
}

SDL_bool SettingsTryLoad(char *path)
{
    FileIO file = FileIOCreate(path);
    if (!FileIOReadBinary(&file))
    {
        FileIODestroy(&file);
        return SDL_FALSE;
    }
    *settings = *(Settings *)file.contents;
    FileIODestroy(&file);
    return SDL_TRUE;
}

void SettingsSave(char *path)
{
    FileIO file = FileIOCreate(SETTINGS_PATH);
    FileIOCopy(&file, settings, sizeof(Settings));
    FileIOWriteBinary(&file);
    FileIODestroy(&file);
}