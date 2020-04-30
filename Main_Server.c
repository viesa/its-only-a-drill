#include "include/core/AppServer.h"

void InitSDL();
void QuitSDL();

int main()
{
    InitSDL();

    SDL_bool isRunning = SDL_TRUE;
    // Has to be the first
    ServerInitialize();
    ClockInitialize();

    AppServer *app = AppServerCreate(&isRunning);

    printf("Server running...\n");
    while (isRunning)
    {
        ClockTick();
        AppServerGo(app);
    }

    AppServerDestroy(app);
    //Has to be the last
    ServerUninitialize();

    QuitSDL();

    return EXIT_SUCCESS;
}

void InitSDL()
{
    if (SDL_Init(SDL_INIT_TIMER) < 0)
        log_error("Failed to init SDL_timer: %s", SDL_GetError());
    if (SDLNet_Init() < 0)
        log_error("Failed to init SDL_net: %s", SDLNet_GetError());
}

void QuitSDL()
{
    SDLNet_Quit();
    SDL_Quit();
}