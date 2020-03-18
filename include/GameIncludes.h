#define GAME_AUIDO_RES_CAMERA_CLK_INPUT                          \
    Game *game = (Game *)attributes[GAttrib_Game];               \
    Audio *audio = (Audio *)attributes[GAttrib_Audio];           \
    Resources *res = (Resources *)attributes[GAttrib_Resources]; \
    Camera *camera = (Camera *)attributes[GAttrib_Camera];       \
    Clock *clk = (Clock *)attributes[GAttrib_Clock];             \
    Input *input = (Input *)attributes[GAttrib_Input];

#include <SDL2/SDL.h>

#include "core/Game.h"
#include "core/GameLogic.h"
#include "core/GameGraphics.h"
#include "net/NetworkMgr.h"

#include "Resources.h"
