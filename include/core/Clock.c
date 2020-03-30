#include "Clock.h"

struct Clock
{
    int m_lastTickTime;
    int m_delta;
};

Clock *ClockCreate()
{
    Clock *clock_ret = (Clock *)SDL_malloc(sizeof(Clock));
    clock_ret->m_delta = 0;
    clock_ret->m_lastTickTime = 0;
    return clock_ret;
}
void ClockDestroy(Clock *clock)
{
    SDL_free(clock);
}

void ClockTick(Clock *clock)
{
    uint32_t tick_time = SDL_GetTicks();
    clock->m_delta = tick_time - clock->m_lastTickTime;
    clock->m_lastTickTime = tick_time;
}

const float ClockGetDeltaTime(Clock *clock)
{
    return (float)clock->m_delta / 1000.0f;
}

const float ClockGetFPS(Clock *clock)
{
    return 1 / ClockGetDeltaTime(clock);
}

const float ClockGetDeltaTimeMS(Clock *clock)
{
    return (float)clock->m_delta;
}
