#ifndef GAME_H
#define GAME_H
#include "Graphics.h"
#include "Camera.h"
#include "Clock.h"
#include "Input.h"
#include "GameLogic.h"
#include "GameGraphics.h"
#include "GameInit.h"
#include "GameExit.h"

typedef enum GAttributes
{
    GAttrib_Game = 0,
    GAttrib_Graphics,
    GAttrib_Resources,
    GAttrib_Camera,
    GAttrib_Clock,
    GAttrib_Input,
    GAttrib_Count
} GAttributes;

typedef struct Game Game;

Game *GameCreate(Clock *clock, SDL_bool *running, Input *input);
void GameDestroy(Game *game);

void GameRun(Game *game);

void GameUpdateComponents(Game *game);

void Draw(Game *game, Drawable drawable);

#endif