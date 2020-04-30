#include "Event.h"
#include "Input.h"

struct Event
{
    SDL_Event events;

    SDL_bool *running;

    char text[32];
};

Event *EventCreate(SDL_bool *running)
{
    Event *event_ret = MALLOC(Event);
    event_ret->running = running;
    return event_ret;
}
void EventDestroy(Event *event)
{
    SDL_free(event);
}

void EventPollAll(Event *event)
{
    while (SDL_PollEvent(&event->events))
    {
        switch (event->events.type)
        {
        case SDL_QUIT:
            *event->running = SDL_FALSE;
            break;

        case SDL_KEYDOWN:
        {
            InputKeyDown(event->events.key.keysym.scancode);
            break;
        }
        case SDL_KEYUP:
        {
            InputKeyUp(event->events.key.keysym.scancode);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            InputMouseDown(event->events.button.button);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            InputMouseUp(event->events.button.button);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            Vec2 _new;
            _new.x = (float)event->events.motion.x;
            _new.y = (float)event->events.motion.y;
            InputMouseMove(_new);
            break;
        }
        case SDL_TEXTINPUT:
            /* Add new text onto the end of our text */
            //strcat(event->text, event->events.text.text);
            InputTypePortal(event->events.text.text[0]);
            strcpy(event->events.text.text, "");
            break;
        default:
            break;
        }
    }
}