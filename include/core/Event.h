#ifndef EVENT_H
#define EVENT_H
#include <SDL2/SDL_events.h>
#include "Input.h"

typedef struct Event Event;

Event *EventCreate(Input *input);
void EventDestroy(Event *event);

void EventPollAll(Event *event);

#endif