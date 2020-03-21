#include "../include/C_Includes.h"

void C_Graphics(void *attributes[])
{
    GAME_GFX_AUIDO_RES_CAMERA_CLK_INPUT_CLIENT;

    for (int i = 0; i < 2880; i++)
        Draw(game, res->db[i]);
    ItemDraw(camera, &res->item);
    EntityDraw(camera, res->entity);
    PlayerDraw(camera, res->db[2999]);
    //GameDraw( myTriList);
}
