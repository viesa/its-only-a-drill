#ifndef S_GAME_H
#define S_GAME_H
#include "../Clock.h"
#include "../Input.h"
#include "S_Logic.h"
#include "S_Init.h"
#include "S_Exit.h"

typedef enum GAttributes
{
    GAttrib_Game = 0,
    GAttrib_Resources,
    GAttrib_Clock,
    GAttrib_Input,
    GAttrib_Count
} GAttributes;

typedef struct S_Game S_Game;

S_Game *S_GameCreate(Clock *clock, SDL_bool *running, Input *input);
void S_GameDestroy(S_Game *game);

void S_GameRun(S_Game *game);

void S_GameUpdateComponents(S_Game *game);

#endif