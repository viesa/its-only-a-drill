#include "../include/S_Includes.h"

void S_Logic(void *attributes[])
{
    GAME_AUIDO_RES_CAMERA_CLK_INPUT_SERVER;

    SDL_Delay(100);
    ServerBroadcast(server, Test, "ALSO A TEST", 12);
}