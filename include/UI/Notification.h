#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Dependencies.h"
#include "Font.h"

typedef enum NotificationType
{
    NT_INFO,
    NT_WARN,
    NT_ERROR
} NotificationType;

typedef struct Notification
{
    float displayTime;
    char text[32];
    SDL_bool displayed;
    NotificationType nt;
} Notification;

void NotifyInitialize();
void Notify(char text[32], float displayTime, NotificationType nt);
void NotifierUpdate();

#endif