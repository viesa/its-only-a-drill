#include "FPSManager.h"

FPSManager *FPSManagerCreate()
{
    FPSManager *fpsManager = MALLOC(FPSManager);
    fpsManager->startWait = 0;
    fpsManager->endWait = 0;
    fpsManager->desiredFPS = 60;
    return fpsManager;
}
void FPSManagerDestroy(FPSManager *fpsManager)
{
    SDL_free(fpsManager);
}

void FPSManagerStart(FPSManager *fpsManager)
{
    fpsManager->startWait = SDL_GetPerformanceCounter();
}
void FPSManagerAdjust(FPSManager *fpsManager)
{
    fpsManager->endWait = SDL_GetPerformanceCounter();
    float difference = (fpsManager->endWait - fpsManager->startWait) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    if (fpsManager->desiredFPS != 0)
        SDL_Delay(((((float)(1000 / fpsManager->desiredFPS)) - difference) < 0) ? 0 : (((float)(1000 / fpsManager->desiredFPS)) - difference));
}
