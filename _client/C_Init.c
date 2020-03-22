#include "../include/C_Includes.h"

void C_Init(void *attributes[])
{
    GAME_GFX_AUIDO_RES_CAMERA_CLK_INPUT_CLIENT;

    int cnt = 0;

    //Cut tiles from spritesheets
    //SS_Legacy
    SDL_Rect tileSand = {16, 112, 16, 16};
    SDL_Rect tileCowHead = {352, 1088, 16, 16};

    //SS_Characters
    SDL_Rect tileWomanDefaultGun = {0, 44, 57, 43};

    //SS_Tiles
    SDL_Rect tileWood = {1036, 74, 64, 64};

    short floorTileSize = 64;

    for (int row = 0; row < 45; row++)
    {
        for (int col = 0; col < 64; col++)
        {
            res->db[cnt] = DrawableCreate(tileWood, (SDL_Rect){(col - 32) * floorTileSize, (row - 22.5) * floorTileSize, floorTileSize, floorTileSize}, SS_Tiles);
            cnt++;
        }
    }

    res->db[2999] = DrawableCreate(tileWomanDefaultGun, (SDL_Rect){496, 344, 57, 43}, SS_Characters);

    res->test = SoundCreate(audio, SF_Test);

    res->cameraFollow = (Vec2){0.0f, 0.0f};

    ClientSetNet(client, "85.226.233.210", 1337);
    ClientStart(client);
    ClientSend(client, Test, "THIS IS A TEST", 15);

    res->entities[0] = EntityCreate(0, 0, 100, 20, EntityWoman, SDL_TRUE);
    res->entities[0].move_x = 500;
    res->entities[1] = EntityCreate(300, 0, 100, 20, EntityWoman, SDL_TRUE);

    res->item = ItemCreate(ItemWoodenSword);
    //MyResCreate();
}