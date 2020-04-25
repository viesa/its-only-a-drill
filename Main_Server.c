#include "include/core/AppServer.h"
int main()
{
    AppServer app = AppServerCreate();
    printf("Server running...\n");
    while (app.isRunning)
    {
        AppServerGo(&app);
    }
    AppServerDestroy(&app);
    SDL_Quit();

    return EXIT_SUCCESS;
}