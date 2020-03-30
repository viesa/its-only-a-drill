#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL_timer.h>

typedef struct Clock Clock;

Clock *ClockCreate();
void ClockDestroy(Clock *clock);

void ClockTick(Clock *clock);
const float ClockGetDeltaTime(Clock *clock);
const float ClockGetFPS(Clock *clock);
const float ClockGetDeltaTimeMS(Clock *clock);

#endif