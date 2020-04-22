#ifndef FPSMANAGER_H
#define FPSMANAGER_H

typedef struct FPSManager
{
    unsigned long startWait;
    unsigned long endWait;
    int desiredFPS;
} FPSManager;

FPSManager *FPSManagerCreate();
void FPSManagerDestroy(FPSManager *fpsManager);

void FPSManagerStart(FPSManager *fpsManager);
void FPSManagerAdjust(FPSManager *fpsManager);

#endif