#include "FPSManager.h"

typedef struct FPSManager
{
    unsigned long startWait;
    unsigned long endWait;
    int desiredFPS;
} FPSManager;

static FPSManager *fpsManager;

void FPSManagerInitialize()
{
    fpsManager = MALLOC(FPSManager);
    ALLOC_ERROR_CHECK(fpsManager);

    fpsManager->startWait = 0;
    fpsManager->endWait = 0;
    fpsManager->desiredFPS = 60;
}
void FPSManagerUninitialize()
{
    FREE(fpsManager);
}

void FPSManagerStart()
{
    fpsManager->startWait = SDL_GetPerformanceCounter();
}
void FPSManagerAdjust()
{
    if (fpsManager->desiredFPS != 0)
    {
        fpsManager->endWait = SDL_GetPerformanceCounter();
        float difference = (fpsManager->endWait - fpsManager->startWait) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        SDL_Delay(((((float)(1000 / fpsManager->desiredFPS)) - difference) < 0) ? 0 : (((float)(1000 / fpsManager->desiredFPS)) - difference));
    }
}

int FPSManagerGetDesiredFPS()
{
    return fpsManager->desiredFPS;
}

void FPSManagerSetDesiredFPS(int fps)
{
    fpsManager->desiredFPS = fps;
}