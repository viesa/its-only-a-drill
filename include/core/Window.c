#include "Window.h"

#include <string.h>
#include <SDL2/SDL_hints.h>

#include "Log.h"

Window *WindowCreate(const char *title)
{
    Window *window = (Window *)SDL_malloc(sizeof(Window));
    window->title = (char *)SDL_malloc(sizeof(char) * strlen(title));
    strcpy(window->title, title);
    window->width = 720;
    window->height = 480;
    SDL_DisplayMode displaymode;
    SDL_GetCurrentDisplayMode(0, &displaymode);
#ifdef WINDOW_DEBUG
    window->width = 720;
    window->height = 480;
#else
    window->width = displaymode.w - 50;
    window->height = displaymode.h - 100; // Remove 50 pixels to account for window not being in fullscreen, and compensate for menu bars.
#endif

    window->sdl_window = SDL_CreateWindow(window->title,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          window->width,
                                          window->height,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window->sdl_window)
        log_fatal("Could not create window: %s", SDL_GetError());

    window->renderer = SDL_CreateRenderer(window->sdl_window,
                                          -1,
                                          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window->renderer)
        log_fatal("Could not create renderer: %s", SDL_GetError());
    SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);

    WindowSetIcon(window, "assets/window_icon.bmp");

    return window;
}

void WindowDestroy(Window *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdl_window);
    SDL_free(window->title);
    SDL_free(window);
}

void WindowClear(Window *window)
{
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->renderer);
}
void WindowPresent(Window *window)
{
    SDL_RenderPresent(window->renderer);
}
void WindowSetAntiAliasing(Window *window, const int level)
{
    if (level < 0 || level > 8)
    {
        log_warn("Anti-aliasing level not supported: %d", level);
    }
    char alevel[2];
    SDL_itoa(level, alevel, 10);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, alevel);
}

void WindowSetVSync(Window *window, SDL_bool onoff)
{
    if (onoff)
    {
        SDL_GL_SetSwapInterval(1);
        window->vsyncEnabled = SDL_TRUE;
    }
    else
    {
        SDL_GL_SetSwapInterval(0);
        window->vsyncEnabled = SDL_FALSE;
    }
}

void WindowSetFullscreen(Window *window, SDL_bool onoff)
{
    if (onoff)
    {
        SDL_SetWindowFullscreen(window->sdl_window, SDL_WINDOW_FULLSCREEN);
        window->isFullscreen = SDL_TRUE;
    }
    else
    {
        SDL_SetWindowFullscreen(window->sdl_window, 0);
        window->isFullscreen = SDL_FALSE;
    }
}

void WindowSetTitle(Window *window, const char *title)
{
    SDL_free(window->title);
    window->title = (char *)SDL_malloc(sizeof(char) * strlen(title));
    strcpy(window->title, title);
    SDL_SetWindowTitle(window->sdl_window, window->title);
}

void WindowSetSize(Window *window, int width, int height)
{
    window->width = width;
    window->height = height;
    SDL_SetWindowSize(window->sdl_window, window->width, window->height);
}

void WindowSetIcon(Window *window, const char *filepath)
{
    SDL_Surface *win_icon = SDL_LoadBMP(filepath);
    if (!win_icon)
        log_error("Could not load win_icon: [%s]", filepath);
    SDL_SetWindowIcon(window->sdl_window, win_icon);
    if (win_icon)
        SDL_FreeSurface(win_icon);
}