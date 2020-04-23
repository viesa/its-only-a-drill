#ifndef EVENT_H
#define EVENT_H

#include "Dependencies.h"
#include "Input.h"

typedef struct Event Event;

Event *EventCreate(Input *input, SDL_bool *running);
void EventDestroy(Event *event);

/// Pops every event on the event-stack
/// and handles them respectively
void EventPollAll(Event *event);

#endif