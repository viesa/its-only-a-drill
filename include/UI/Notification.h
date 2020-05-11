#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Dependencies.h"
#include "Font.h"

typedef enum NotificationType
{
    NT_GOOD,
    NT_INFO,
    NT_WARN,
    NT_ERROR
} NotificationType;

typedef struct Notification
{
    float totalTime;
    float displayTime;
    float opacity;
    int framesDisplayed;
    char text[32];
    NotificationType nt;
} Notification;

void NotifyInitialize();
void Notify(char text[32], float displayTime, NotificationType nt);
void NotifierUpdate();

#endif