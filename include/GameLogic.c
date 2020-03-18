#include "GameIncludes.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

void GameUpdateLogic(void *attributes[])
{
    GAME_AUIDO_RES_CAMERA_CLK_INPUT;

    if (InputGet(input, KEY_Q))
        CameraAddRotation(camera, -1.0f);
    if (InputGet(input, KEY_E))
        CameraAddRotation(camera, 1.0f);

    if (InputGet(input, KEY_A))
        res->cameraFollow.x -= 1.0f;
    if (InputGet(input, KEY_W))
        res->cameraFollow.y -= 1.0f;
    if (InputGet(input, KEY_D))
        res->cameraFollow.x += 1.0f;
    if (InputGet(input, KEY_S))
        res->cameraFollow.y += 1.0f;
    if (InputGet(input, KEY_M))
        SoundPlay(res->test, 0);
    if (InputGet(input, KEY_O))
        SoundStop(res->test);

    NetworkMgrPollAll(res->netMgr);
    //MyResUpdateLogic(dt)
}