#include "Notification.h"
#include "Clock.h"

struct
{
    float dt;
    Font *font;

    int last;
    Notification buff[100];
} notifier;

void NotifyInitialize(Font *font)
{
    notifier.font = font;
    notifier.last = 0;
}

void Notify(char text[32], float displayTime, NotificationType nt)
{
    notifier.last++;
    notifier.buff[notifier.last].nt = nt;
    notifier.buff[notifier.last].displayTime = displayTime;
    strcpy(notifier.buff[notifier.last].text, text);
}

void NotifierUpdate()
{
    if (notifier.last < 0)
        return;

    //Always 0 that is showing, if showing
    //Remove old notifications
    notifier.dt = ClockGetDeltaTime();
    notifier.buff[0].displayTime -= notifier.dt;

    if (notifier.buff[0].displayTime <= 0)
    {
        for (int i = 0; i < notifier.last; i++)
        {
            notifier.buff[i] = notifier.buff[i + 1];
        }
        notifier.last--;
    }

    SDL_Color notificationColor;

    switch (notifier.buff[0].nt)
    {
    case NT_WARN:
        notificationColor.r = 255;
        notificationColor.g = 140;
        notificationColor.b = 0;
        notificationColor.a = 180;
        break;
    case NT_ERROR:
        notificationColor.r = 255;
        notificationColor.g = 0;
        notificationColor.b = 0;
        notificationColor.a = 180;
        break;
    default:
        notificationColor.r = 255;
        notificationColor.g = 255;
        notificationColor.b = 255;
        notificationColor.a = 180;
        break;
    }

    SDL_SetRenderDrawColor(notifier.font->gfx->window->renderer, 0, 0, 0, 128);
    SDL_Rect bg = FontGetSize(notifier.font, FontGetDynamicSizing(notifier.font), notifier.buff[0].text);
    bg.w += 20;
    bg.h += 20;
    SDL_RenderFillRect(notifier.font->gfx->window->renderer, &bg);

    FontDraw(notifier.font, FontGetDynamicSizing(notifier.font), notifier.buff[0].text, 10, 10, FAL_L, 0, notificationColor);

    // log_info("Notification: %s", notifier.buff[0].text);
}