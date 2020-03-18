#define GAME_GFX_AUIDO_RES_CAMERA_CLK_INPUT_CLIENT                   \
    C_Game *game = (C_Game *)attributes[GAttrib_Game];               \
    Graphics *gfx = (Graphics *)attributes[GAttrib_Graphics];        \
    Audio *audio = (Audio *)attributes[GAttrib_Audio];               \
    C_Resources *res = (C_Resources *)attributes[GAttrib_Resources]; \
    Camera *camera = (Camera *)attributes[GAttrib_Camera];           \
    Clock *clk = (Clock *)attributes[GAttrib_Clock];                 \
    Input *input = (Input *)attributes[GAttrib_Input];               \
    Client *client = (Client *)attributes[GAttrib_Client];

#pragma GCC diagnostic ignored "-Wunused-variable"

#include <SDL2/SDL.h>

#include "core/_client/C_Game.h"
#include "core/_client/C_Logic.h"
#include "core/_client/C_Graphics.h"
#include "net/NetworkMgr.h"
#include "../_client/C_Resources.h"
