#include "core/GameLogic.h"
#include "core/Game.h"
#include "Resources.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

void GameInit(void *attributes[])
{
    Game *game = (Game *)attributes[GAttrib_Game];
    Audio *audio = (Audio *)attributes[GAttrib_Audio];
    Resources *res = (Resources *)attributes[GAttrib_Resources];

    res->test = SoundCreate(audio, SF_Test);

    res->db[0] = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){0, 10, 50, 50}, SS_Tilemap);
    res->db[1] = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){0, 10, 50, 50}, SS_Tilemap);
    res->db[2] = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){64, 64, 50, 50}, SS_Tilemap);
    res->db[3] = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){64, 128, 50, 50}, SS_Tilemap);

    res->follow = (Vec2){0.0f, 0.0f};
    //MyResCreate();
}