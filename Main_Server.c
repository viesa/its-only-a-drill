#include "include/core/AppServer.h"

int main()
{
    SDL_bool isRunning = SDL_TRUE;
    // Has to be the first
    UDPServerInitialize();
    Clock *m_clock = ClockCreate();

    AppServer *app = AppServerCreate(&isRunning, m_clock);

    printf("Server running...\n");
    while (isRunning)
    {
        ClockTick(m_clock);
        AppServerGo(app);
    }

    AppServerDestroy(app);
    //Has to be the last
    UDPServerUninitialize();

    SDL_Quit();

    return EXIT_SUCCESS;
}