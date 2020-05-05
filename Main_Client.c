#include "Macros.h"
#include "Dependencies.h"
#include "Event.h"
#include "include/AppClient.h"

void InitSDL();
void QuitSDL();

int main()
{
    InitSDL();

    SDL_bool running = SDL_TRUE;

    InputInitialize();
    ClockInitialize();

    FPSManager *fpsManager = FPSManagerCreate();
    Event *event = EventCreate(&running);
    AppClient *app = AppClientCreate(&running, fpsManager);

    while (running)
    {
        FPSManagerStart(fpsManager);
        ClockTick();
        InputUpdateKeymaps();
        EventPollAll(event);
        AppClientRun(app);
        FPSManagerAdjust(fpsManager);
    }

    EventDestroy(event);
    AppClientDestroy(app);

    InputUninitialize();

    QuitSDL();

    return EXIT_SUCCESS;
}

void InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        log_error("Failed to init SDL: %s", SDL_GetError());
    if (SDLNet_Init() < 0)
        log_error("Failed to init SDL_net: %s", SDLNet_GetError());
    if (TTF_Init() < 0)
        log_error("Could not initialize SDL_ttf: %s", TTF_GetError());
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0)
        log_error("Could not initialize SDL_mixer: %s", Mix_GetError());
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0)
        log_error("Could not initialize SDL_image: %s", Mix_GetError());
}

void QuitSDL()
{
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
}