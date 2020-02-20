#include <SDL2/SDL.h>
#include "include/core/Game.h"
#include "include/core/Event.h"

int main()
{
    SDL_bool m_running = SDL_TRUE;

    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input);
    Game *m_game = GameCreate(m_clock, &m_running, m_input);

    while (m_running)
    {
        EventPollAll(m_event);
        if (InputGet(m_input, KEY_ESC) || InputGet(m_input, EVENT_QUIT))
            m_running = SDL_FALSE;

        ClockTick(m_clock);
        GameRun(m_game);
    }

    ClockDestroy(m_clock);
    InputDestroy(m_input);
    EventDestroy(m_event);
    GameDestroy(m_game);
}