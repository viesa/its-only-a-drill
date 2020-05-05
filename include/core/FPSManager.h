#ifndef FPSMANAGER_H
#define FPSMANAGER_H

#include "Dependencies.h"

typedef struct FPSManager
{
    unsigned long startWait;
    unsigned long endWait;
    int desiredFPS;
} FPSManager;

// creates space on the heap
FPSManager *FPSManagerCreate();
// removes the data for the heap
void FPSManagerDestroy(FPSManager *fpsManager);

// gets the start proformens data
void FPSManagerStart(FPSManager *fpsManager);
// applys the proformens data to limit fps
void FPSManagerAdjust(FPSManager *fpsManager);

#endif