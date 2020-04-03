#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include "include/core/Event.h"
#include "include/net/Client.h"

#include "include/core/AppClient.h"

int main()
{
    SDL_bool m_running = SDL_TRUE;
    FpsManger *m_FPS = FpsMangerCreate();
    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input, &m_running);
    Client *m_client = ClientCreate("127.0.0.1", 4000);
    AppClient *m_app = AppClientCreate(&m_running, m_clock, m_input, m_client, m_FPS);

    while (m_running)
    {
        ClockFpsStart(m_FPS);
        ClockTick(m_clock);
        InputUpdate(m_input);
        EventPollAll(m_event);
        AppClientRun(m_app);
        ClockContorlFPS(m_FPS);
    }

    ClockDestroy(m_clock);
    InputDestroy(m_input);
    EventDestroy(m_event);
    ClientDestroy(m_client);
    AppClientDestroy(m_app);
    SDL_Quit();

    return EXIT_SUCCESS;
}