#include "GameIncludes.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

void GameComposeFrame(void *attributes[])
{
    GAME_AUIDO_RES_CAMERA_CLK_INPUT;

    for (int i = 0; i < 2880; i++)
        Draw(game, res->db[i]);

    PlayerDraw(camera, res->db[2999]);

    //GameDraw( myTriList);
}
