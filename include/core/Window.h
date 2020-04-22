#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_render.h>

typedef struct Window
{
    SDL_Window *sdl_window;
    SDL_Renderer *renderer;

    char *title;
    int width;
    int height;
    SDL_bool isFullscreen;
    SDL_bool vsyncEnabled;
} Window;

Window *WindowCreate(const char *title);
void WindowDestroy(Window *window);

/// Removes everything on screen
void WindowClear(Window *window);
/// Swap back-buffer and front-buffer
/// to display everything on screen
void WindowPresent(Window *window);

void WindowSetAntiAliasing(Window *window, int level);
void WindowSetVSync(Window *window, SDL_bool onoff);
void WindowSetFullscreen(Window *window, SDL_bool onoff);
void WindowSetTitle(Window *window, const char *title);
void WindowSetSize(Window *window, int width, int height);
#endif