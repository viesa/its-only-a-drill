#include "Clock.h"

struct Clock
{
    int lastTickTime;
    int delta;
} appClock;

void ClockInitialize()
{
    appClock.delta = 0;
    appClock.lastTickTime = SDL_GetTicks();
}

void ClockTick()
{
    uint32_t tick_time = SDL_GetTicks();
    appClock.delta = tick_time - appClock.lastTickTime;
    appClock.lastTickTime = tick_time;
}

float ClockGetDeltaTime()
{
    return (float)appClock.delta / 1000.0f;
}

float ClockGetDeltaTimeMS()
{
    return (float)appClock.delta;
}

float ClockGetFPS()
{
    return 1.0f / ClockGetDeltaTime();
}
