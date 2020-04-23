#include "Event.h"
#include "Input.h"

struct Event
{
    SDL_Event m_events;
    Input *m_input;

    SDL_bool *m_running;
};

Event *EventCreate(Input *input, SDL_bool *running)
{
    Event *event_ret = MALLOC(Event);
    event_ret->m_input = input;
    event_ret->m_running = running;
    return event_ret;
}
void EventDestroy(Event *event)
{
    SDL_free(event);
}

void EventPollAll(Event *event)
{
    while (SDL_PollEvent(&event->m_events))
    {
        switch (event->m_events.type)
        {
        case SDL_QUIT:
            *event->m_running = SDL_FALSE;
            break;

        case SDL_KEYDOWN:
        {
            InputKeyDown(event->m_input, event->m_events.key.keysym.scancode);
            break;
        }
        case SDL_KEYUP:
        {
            InputKeyUp(event->m_input, event->m_events.key.keysym.scancode);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            InputMouseDown(event->m_input, event->m_events.button.button);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            InputMouseUp(event->m_input, event->m_events.button.button);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            Vec2 _new;
            _new.x = (float)event->m_events.motion.x;
            _new.y = (float)event->m_events.motion.y;
            InputMouseMove(event->m_input, _new);
            break;
        }
        default:
            break;
        }
    }
}