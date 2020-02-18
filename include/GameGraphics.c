#include "core/GameGraphics.h"
#include "core/Game.h"
#include "Resources.h"

void GameComposeFrame(void *attributes[])
{
    Game *game = (Game *)attributes[GAttrib_Game];
    Graphics *gfx = (Graphics *)attributes[GAttrib_Graphics];
    Resources *res = (Resources *)attributes[GAttrib_Resources];
    Camera *camera = (Camera *)attributes[GAttrib_Camera];
    Input *input = (Input *)attributes[GAttrib_Input];

    for (int i = 0; i < 4; i++)
        CameraDraw(camera, res->db[i]);

    //GameDraw( myTriList);
}
