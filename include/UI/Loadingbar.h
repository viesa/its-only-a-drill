#ifndef LOADINGBAR_H
#define LOADINGBAR_H

#include "Dependencies.h"
#include "Font.h"
#include "Clock.h"

typedef struct LoadingBar
{
    SDL_bool active;
    float progress;
} LoadingBar;

LoadingBar *LoadingBarCreate();
void LoadingBarDestroy(LoadingBar *bar);

void LoadingBarReset(LoadingBar *bar);
void LoadingBarUpdate(LoadingBar *bar);
void LoadingBarAdd(LoadingBar *bar, float progress);
void LoadingBarSet(LoadingBar *bar, float progress);
void LoadingBarShow(LoadingBar *bar);
void LoadingBarHide(LoadingBar *bar);

#endif