#include "State.h"

struct State
{
    GameState gameState;
    MenuState menuState;
    CLIState CLIState;
} state;

void StateInitialize()
{
    state.gameState = GS_None;
    state.menuState = MS_None;
    state.CLIState = CS_None;
}

GameState GameStateGet()
{
    return state.gameState;
}

MenuState MenuStateGet()
{
    return state.menuState;
}

CLIState CLIStateGet()
{
    return state.CLIState;
}

void GameStateSet(GameState newState)
{
    state.gameState = newState;
}

void MenuStateSet(MenuState newState)
{
    state.menuState = newState;
}

void CLIStateSet(CLIState newState)
{
    state.CLIState = newState;
}