#include "include/core/Macros.h"
#include "include/core/AppClient.h"
#include "include/core/Event.h"
int main()
{
    SDL_bool m_running = SDL_TRUE;
    FPSManager *m_fpsManager = FPSManagerCreate();
    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input, &m_running);
#ifndef MAIN_DEBUG
    UDPClient m_client = UDPClientCreate("85.226.160.126", 1337); //drill.pierrelf.com port 1337
#endif
#ifdef MAIN_DEBUG
    UDPClient m_client = UDPClientCreate("127.0.0.1", 1337);
#endif
    AppClient *m_app = AppClientCreate(&m_running, m_clock, m_input, &m_client, m_fpsManager);

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
    UDPClientDestroy(&m_client);
    AppClientDestroy(m_app);
    SDL_Quit();

    return EXIT_SUCCESS;
}