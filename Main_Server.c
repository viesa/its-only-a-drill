#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/_server/S_Game.h"
#include "include/core/Event.h"
#include "include/net/Server.h"

int main()
{
    SDL_bool m_running = SDL_TRUE;

    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input);
    Server *m_server = ServerCreate(4000);
    S_Game *m_game = S_GameCreate(m_clock, &m_running, m_input, m_server);

    while (m_running)
    {
        EventPollAll(m_event);
        if (InputGet(m_input, KEY_ESC) || InputGet(m_input, EVENT_QUIT))
            m_running = SDL_FALSE;

        ClockTick(m_clock);
        S_GameRun(m_game);
    }

    ClockDestroy(m_clock);
    InputDestroy(m_input);
    EventDestroy(m_event);
    ServerDestroy(m_server);
    S_GameDestroy(m_game);
    SDL_Quit();
}