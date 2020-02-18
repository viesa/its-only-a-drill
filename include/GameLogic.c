#include "core/GameLogic.h"
#include "core/Game.h"
#include "Resources.h"
#include <SDL2/SDL.h>

void GameUpdateLogic(void *attributes[])
{
    Game *game = (Game *)attributes[GAttrib_Game];
    Resources *res = (Resources *)attributes[GAttrib_Resources];
    Camera *camera = (Camera *)attributes[GAttrib_Camera];
    Clock *clk = (Clock *)attributes[GAttrib_Clock];
    Input *input = (Input *)attributes[GAttrib_Input];

    if (InputGet(input, KEY_Q))
        CameraAddRotation(camera, -1.0f);
    if (InputGet(input, KEY_E))
        CameraAddRotation(camera, 1.0f);

    if (InputGet(input, KEY_A))
        res->follow.x -= 1.0f;
    if (InputGet(input, KEY_W))
        res->follow.y -= 1.0f;
    if (InputGet(input, KEY_D))
        res->follow.x += 1.0f;
    if (InputGet(input, KEY_S))
        res->follow.y += 1.0f;
    //MyResUpdateLogic(dt)
}