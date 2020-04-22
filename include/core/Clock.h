#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL_timer.h>

typedef struct Clock Clock;

Clock *ClockCreate();
void ClockDestroy(Clock *clock);

/// Updates the deltatime (the clock's ticks)
void ClockTick(Clock *clock);
/// Returns the ammount of seconds passed since last update
const float ClockGetDeltaTime(Clock *clock);
const float ClockGetFPS(Clock *clock);
/// Returns the ammount of milliseconds passed since last update
const float ClockGetDeltaTimeMS(Clock *clock);

#endif