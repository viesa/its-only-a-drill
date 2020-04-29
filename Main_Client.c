#include "include/core/Dependencies.h"
#include "include/core/AppClient.h"
#include "include/core/Event.h"

void InitSDL();
void QuitSDL();

int main()
{
    InitSDL();

    SDL_bool m_running = SDL_TRUE;

    FPSManager *m_fpsManager = FPSManagerCreate();
    Clock *m_clock = ClockCreate();
    Input *m_input = InputCreate();
    Event *m_event = EventCreate(m_input, &m_running);
    AppClient *m_app = AppClientCreate(&m_running, m_clock, m_input, m_fpsManager);

    while (m_running)
    {
        FPSManagerStart(m_fpsManager);
        ClockTick(m_clock);
        InputUpdate(m_input);
        EventPollAll(m_event);
        AppClientRun(m_app);
        FPSManagerAdjust(m_fpsManager);
    }

    ClockDestroy(m_clock);
    InputDestroy(m_input);
    EventDestroy(m_event);
    AppClientDestroy(m_app);

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