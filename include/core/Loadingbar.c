#include "Loadingbar.h"

LoadingBar *LoadingBarCreate(Graphics *gfx)
{
    LoadingBar *bar = MALLOC(LoadingBar);
    bar->gfx = gfx;
    bar->active = SDL_FALSE;
    bar->progress = 0;
    return bar;
}

void LoadingBarReset(LoadingBar *bar)
{
    bar->progress = 0;
}

void LoadingBarUpdate(LoadingBar *bar)
{
    if (bar->active)
    {
        if (bar->progress > 100)
            bar->active = SDL_FALSE;

        int winW = bar->gfx->window->width;
        int winH = bar->gfx->window->height;
        int w = winW / 2;
        int h = winH / 5;
        int progW = w / 100 * bar->progress - 10;

        SDL_SetRenderDrawColor(bar->gfx->window->renderer, 255, 255, 255, 255);
        SDL_Rect bg = {winW / 2 - w / 2, winH / 2 - h / 2, w, h};
        SDL_RenderFillRect(bar->gfx->window->renderer, &bg);

        SDL_SetRenderDrawColor(bar->gfx->window->renderer, 0, 0, 0, 255);
        SDL_Rect fg = {winW / 2 - w / 2 + 5, winH / 2 - h / 2 + 5, progW, h - 10};
        SDL_RenderFillRect(bar->gfx->window->renderer, &fg);
    }
}

void LoadingBarAdd(LoadingBar *bar, float progress)
{
    bar->progress += progress;
}

void LoadingBarSet(LoadingBar *bar, float progress)
{
    bar->progress = progress;
}

void LoadingBarShow(LoadingBar *bar)
{
    bar->active = SDL_TRUE;
}
void LoadingBarHide(LoadingBar *bar)
{
    bar->active = SDL_FALSE;
}
void LoadingBarDestroy(LoadingBar *bar)
{
    SDL_free(bar);
}