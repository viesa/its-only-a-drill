#include "Notification.h"
#include "Clock.h"

struct
{
    float dt;

    int last;
    Notification buff[100];
} notifier;

void NotifyInitialize()
{
    notifier.last = -1;
}

void Notify(char text[32], float displayTime, NotificationType nt)
{
    if (notifier.last >= 99)
        return;
    notifier.last++;
    notifier.buff[notifier.last].nt = nt;
    notifier.buff[notifier.last].displayTime = displayTime;
    notifier.buff[notifier.last].totalTime = displayTime;
    notifier.buff[notifier.last].opacity = 0.0;
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

    // Update buffer
    if (notifier.buff[0].displayTime <= 0)
    {
        for (int i = 0; i < notifier.last; i++)
        {
            notifier.buff[i] = notifier.buff[i + 1];
        }
        notifier.last--;
        return;
    }

    float fadeTimer = .5;

    // Set increase, steady or decreasing opacity states
    if (notifier.buff[0].totalTime - notifier.buff[0].displayTime < fadeTimer)
    {
        notifier.buff[0].opacity += 180.0 / (fadeTimer / notifier.dt);
    }
    else if (notifier.buff[0].displayTime <= fadeTimer)
    {
        notifier.buff[0].opacity -= 180.0 / (fadeTimer / notifier.dt);
    }
    else
    {
        notifier.buff[0].opacity = 180.0;
    }

    // Rectify opacity
    // set to 1 because 0 glitches out, don't touch this...
    if (notifier.buff[0].opacity < 1)
        notifier.buff[0].opacity = 1;

    SDL_Color notificationColor;

    switch (notifier.buff[0].nt)
    {
    case NT_GOOD:
        notificationColor.r = 80;
        notificationColor.g = 255;
        notificationColor.b = 80;
        break;
    case NT_WARN:
        notificationColor.r = 255;
        notificationColor.g = 140;
        notificationColor.b = 0;
        break;
    case NT_ERROR:
        notificationColor.r = 255;
        notificationColor.g = 0;
        notificationColor.b = 0;
        break;
    default:
        notificationColor.r = 255;
        notificationColor.g = 255;
        notificationColor.b = 255;
        break;
    }
    notificationColor.a = (Uint8)notifier.buff[0].opacity;

    SDL_Color color = {0, 0, 0, notifier.buff[0].opacity};
    SDL_Rect bg;
    bg.w = FontGetWidth(FontGetDynamicSizing(), notifier.buff[0].text) + 20;
    bg.h = FontGetHeight(FontGetDynamicSizing()) + 20;
    bg.x = WindowGetWidth() - bg.w;
    bg.y = WindowGetHeight() - bg.h;
    GraphicsDrawRect(bg, color, SDL_TRUE);

    FontDraw(FontGetDynamicSizing(), notifier.buff[0].text, bg.x + 10, bg.y + 10, FAL_L, 0, notificationColor);
}