#include "include/core/Dependencies.h"
#include "include/core/AppClient.h"
#include "include/core/Event.h"

int main()
{
    SDL_bool m_running = SDL_TRUE;
    // Has to be the first
    UDPClientInitialize();

    FPSManager *m_fpsManager = FPSManagerCreate();
    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input, &m_running);
    AppClient *m_app = AppClientCreate(&m_running, m_clock, m_input, m_fpsManager);

    while (m_running)
    {
        FPSManagerStart(m_fpsManager);
        ClockTick(m_clock);
        InputUpdate(m_input);
        EventPollAll(m_event);
        AppClientRun(m_app);
        FPSManagerAdjust(m_fpsManager);
    }

    ClockDestroy(m_clock);
    InputDestroy(m_input);
    EventDestroy(m_event);
    AppClientDestroy(m_app);
    //Has to be the last
    UDPClientUninitialize();

    SDL_Quit();

    return EXIT_SUCCESS;
}