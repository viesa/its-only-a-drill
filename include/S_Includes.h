#define GAME_AUIDO_RES_CAMERA_CLK_INPUT_SERVER                       \
    S_Game *game = (S_Game *)attributes[GAttrib_Game];               \
    S_Resources *res = (S_Resources *)attributes[GAttrib_Resources]; \
    Clock *clk = (Clock *)attributes[GAttrib_Clock];                 \
    Input *input = (Input *)attributes[GAttrib_Input];               \
    Server *server = (Server *)attributes[GAttrib_Server];

#pragma GCC diagnostic ignored "-Wunused-variable"

#include <SDL2/SDL.h>

#include "core/_server/S_Game.h"
#include "core/_server/S_Logic.h"
#include "net/NetworkMgr.h"
#include "../_server/S_Resources.h"
