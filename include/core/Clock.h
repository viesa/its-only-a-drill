#ifndef CLOCK_H
#define CLOCK_H

#include "Dependencies.h"
void ClockInitialize();

/// Updates the deltatime (the clock's ticks)
void ClockTick();
/// Returns the ammount of seconds passed since last update
float ClockGetDeltaTime();
float ClockGetFPS();
/// Returns the ammount of milliseconds passed since last update
float ClockGetDeltaTimeMS();

#endif