#include "core/GameGraphics.h"
#include "core/Game.h"
#include "Resources.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

void GameExit(void *attributes[])
{
    Game *game = (Game *)attributes[GAttrib_Game];
    Resources *res = (Resources *)attributes[GAttrib_Resources];

    //SDL_free(myResource);
}
