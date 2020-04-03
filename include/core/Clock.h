#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL_timer.h>

typedef struct Clock Clock;

typedef struct FpsManger
{
    Uint64 StartWait;
    Uint64 EndWait;
    int DesiredFPS;
} FpsManger;
//typedef struct FpsManger FpsManger;

Clock *ClockCreate();
void ClockDestroy(Clock *clock);

/// Updates the deltatime (the clock's ticks)
void ClockTick(Clock *clock);
/// Returns the ammount of seconds passed since last update
const float ClockGetDeltaTime(Clock *clock);
const float ClockGetFPS(Clock *clock);
/// Returns the ammount of milliseconds passed since last update
const float ClockGetDeltaTimeMS(Clock *clock);

FpsManger *FpsMangerCreate();
void ClockFpsStart(FpsManger *FPSControls);
void ClockContorlFPS(FpsManger *FPSControls);

#endif