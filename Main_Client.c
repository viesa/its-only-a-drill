#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/_client/C_Game.h"
#include "include/core/Event.h"
#include "include/net/Client.h"

int main()
{

    SDL_bool m_running = SDL_TRUE;

    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input);
    Client *m_client = ClientCreate("127.0.0.1", 4000);
    C_Game *m_game = C_GameCreate(m_clock, &m_running, m_input, m_client);

    while (m_running)
    {
        EventPollAll(m_event);
        if (InputGet(m_input, KEY_ESC) || InputGet(m_input, EVENT_QUIT))
            m_running = SDL_FALSE;

        ClockTick(m_clock);
        C_GameRun(m_game);
    }

    ClockDestroy(m_clock);
    InputDestroy(m_input);
    EventDestroy(m_event);
    ClientDestroy(m_client);
    C_GameDestroy(m_game);
    SDL_Quit();
}