#ifndef WINDOW_H
#define WINDOW_H

#include "Dependencies.h"

void WindowInitialize(const char *title);
void WindowUninitialize();

/// Removes everything on screen
void WindowClear();
/// Swap back-buffer and front-buffer
/// to display everything on screen
void WindowPresent();

// Move the window to the center
void WindowCenter();

int WindowGetWidth();
int WindowGetHeight();
SDL_Rect WindowGetScreenRect();
SDL_Renderer *WindowGetRenderer();
SDL_bool WindowIsFullscreen();
SDL_bool WindowIsVSyncEnabled();

void WindowSetAntiAliasing(int level);
void WindowSetVSync(SDL_bool onoff);
void WindowSetFullscreen(SDL_bool onoff);
void WindowSetTitle(const char *title);
void WindowSetSize(int width, int height);
void WindowSetIcon(const char *filepath);
#endif