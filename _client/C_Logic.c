#include "../include/C_Includes.h"

void C_Logic(void *attributes[])
{
    GAME_GFX_AUIDO_RES_CAMERA_CLK_INPUT_CLIENT;

    if (InputGet(input, KEY_Q))
        CameraAddRotation(camera, -1.0f);
    if (InputGet(input, KEY_E))
        CameraAddRotation(camera, 1.0f);

    if (InputGet(input, KEY_A))
        res->cameraFollow.x -= 3.0f;
    if (InputGet(input, KEY_W))
        res->cameraFollow.y -= 3.0f;
    if (InputGet(input, KEY_D))
        res->cameraFollow.x += 3.0f;
    if (InputGet(input, KEY_S))
        res->cameraFollow.y += 3.0f;
    if (InputGet(input, KEY_M))
        SoundPlay(res->test, 0);
    if (InputGet(input, KEY_O))
        SoundStop(res->test);

    UpdateEntity(&res->entity);
    //MyResUpdateLogic(dt)
}