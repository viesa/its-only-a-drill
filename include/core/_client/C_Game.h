#ifndef C_GAME_H
#define C_GAME_H
#include "../Graphics.h"
#include "../Camera.h"
#include "../Clock.h"
#include "../Input.h"
#include "../Sound.h"
#include "../../net/Client.h"
#include "C_Logic.h"
#include "C_Graphics.h"
#include "C_Init.h"
#include "C_Exit.h"

typedef enum GAttributes
{
    GAttrib_Game = 0,
    GAttrib_Graphics,
    GAttrib_Audio,
    GAttrib_Resources,
    GAttrib_Camera,
    GAttrib_Clock,
    GAttrib_Input,
    GAttrib_Client,
    GAttrib_Count
} GAttributes;

typedef struct C_Game C_Game;

C_Game *C_GameCreate(Clock *clock, SDL_bool *running, Input *input, Client *client);
void C_GameDestroy(C_Game *game);

void C_GameRun(C_Game *game);

void C_GameUpdateComponents(C_Game *game);

void Draw(C_Game *game, Drawable drawable);

#endif