#include "Window.h"

typedef struct Window
{
    SDL_Window *sdl_window;
    SDL_Renderer *renderer;
    int width;
    int height;
    SDL_bool isFullscreen;
    SDL_bool vsyncEnabled;
} Window;

static Window *window;

void WindowInitialize(const char *title)
{
    window = MALLOC(Window);
    SDL_DisplayMode displaymode;
    SDL_GetCurrentDisplayMode(0, &displaymode);
#ifdef WINDOW_DEBUG
    window->width = 720;
    window->height = 480;
#else
    window->width = displaymode.w - 50;
    window->height = displaymode.h - 100; // Remove 100 pixels to account for window not being in fullscreen, and compensate for menu bars.
#endif

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    window->sdl_window = SDL_CreateWindow(title,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          window->width,
                                          window->height,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!window->sdl_window)
        log_fatal("Could not create window: %s", SDL_GetError());

    window->renderer = SDL_CreateRenderer(window->sdl_window,
                                          -1,
                                          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window->renderer)
        log_fatal("Could not create renderer: %s", SDL_GetError());
    SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    WindowSetIcon("assets/window_icon.bmp");

    WindowSetFullscreen(SDL_FALSE);
    WindowSetVSync(SDL_TRUE);
}

void WindowUninitialize()
{
    WindowSetFullscreen(SDL_FALSE);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdl_window);
    SDL_free(window);
}

void WindowCenter()
{
    SDL_SetWindowPosition(window->sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

int WindowGetWidth()
{
    return window->width;
}

int WindowGetHeight()
{
    return window->height;
}

SDL_Rect WindowGetScreenRect()
{
    return (SDL_Rect){0, 0, window->width, window->height};
}

SDL_Renderer *WindowGetRenderer()
{
    return window->renderer;
}

SDL_bool WindowIsFullscreen()
{
    return window->isFullscreen;
}

SDL_bool WindowIsVSyncEnabled()
{
    return window->vsyncEnabled;
}

void WindowClear()
{
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->renderer);
}
void WindowPresent()
{
    SDL_RenderPresent(window->renderer);
}
void WindowSetAntiAliasing(const int level)
{
    if (level < 0 || level > 8)
    {
        log_warn("Anti-aliasing level not supported: %d", level);
    }
    char alevel[2];
    SDL_itoa(level, alevel, 10);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, alevel);
}

void WindowSetVSync(SDL_bool onoff)
{
    SDL_GL_SetSwapInterval((window->vsyncEnabled = onoff));
}

void WindowSetFullscreen(SDL_bool onoff)
{
    SDL_SetWindowFullscreen(window->sdl_window, (window->isFullscreen = onoff) ? SDL_WINDOW_FULLSCREEN : 0);
    if (!WindowIsFullscreen())
        WindowCenter();
}

void WindowSetTitle(const char *title)
{
    SDL_SetWindowTitle(window->sdl_window, title);
}

void WindowSetSize(int width, int height)
{
    SDL_SetWindowSize(window->sdl_window, (window->width = width), (window->height = height));
    WindowCenter();
}

void WindowSetIcon(const char *filepath)
{
    SDL_Surface *win_icon = SDL_LoadBMP(filepath);
    if (!win_icon)
        log_error("Could not load window icon: [%s]", filepath);
    SDL_SetWindowIcon(window->sdl_window, win_icon);
    if (win_icon)
        SDL_FreeSurface(win_icon);
}