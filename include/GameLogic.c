#include "core/GameLogic.h"
#include "core/Game.h"
#include "Resources.h"
#include <SDL2/SDL.h>

#pragma GCC diagnostic ignored "-Wunused-variable"

void GameUpdateLogic(void *attributes[])
{
    Game *game = (Game *)attributes[GAttrib_Game];
    Audio *audio = (Audio *)attributes[GAttrib_Audio];
    Resources *res = (Resources *)attributes[GAttrib_Resources];
    Camera *camera = (Camera *)attributes[GAttrib_Camera];
    Clock *clk = (Clock *)attributes[GAttrib_Clock];
    Input *input = (Input *)attributes[GAttrib_Input];

    if (InputGet(input, KEY_Q))
        CameraAddRotation(camera, -1.0f);
    if (InputGet(input, KEY_E))
        CameraAddRotation(camera, 1.0f);

    if (InputGet(input, KEY_A))
        res->follow.x -= 3.0f;
    if (InputGet(input, KEY_W))
        res->follow.y -= 3.0f;
    if (InputGet(input, KEY_D))
        res->follow.x += 3.0f;
    if (InputGet(input, KEY_S))
        res->follow.y += 3.0f;
    if (InputGet(input, KEY_M))
        SoundPlay(res->test, 0);
    if (InputGet(input, KEY_O))
        SoundStop(res->test);
    //MyResUpdateLogic(dt)
}