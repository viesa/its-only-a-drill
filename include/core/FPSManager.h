#ifndef FPSMANAGER_H
#define FPSMANAGER_H

#include "Dependencies.h"

void FPSManagerInitialize();
void FPSManagerUninitialize();

// Start the performance counter
void FPSManagerStart();
// Apply the wait to reach desired FPS
void FPSManagerAdjust();
// Returns the desired FPS
int FPSManagerGetDesiredFPS();
/// Sets the desired FPS that FPSManagerAdjust() adjusts to
///\param fps: if fps == 0, inactive the delay
void FPSManagerSetDesiredFPS(int fps);

#endif