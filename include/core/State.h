#ifndef STATE_H
#define STATE_H
typedef enum GameState
{
    GS_Menu,
    GS_Playing,
    GS_None,
    GS_NStates
} GameState;

typedef enum MenuState
{
    MS_Options,
    MS_MainMenu,
    MS_Resolution,
    MS_FPS,
    MS_CustomMap,
    MS_None,
    MS_NStates
} MenuState;

typedef struct State
{
    GameState gameState;
    MenuState menuState;
} State;

State StateCreate();

#endif