#include "../include/S_Includes.h"

void S_Init(void *attributes[])
{
    GAME_AUIDO_RES_CAMERA_CLK_INPUT_SERVER

    ServerSetPort(server, 1337);
    ServerStart(server);
}