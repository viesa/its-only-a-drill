#include "State.h"

struct State
{
    GameState gameState;
    MenuState menuState;
} state;

void StateInitialize()
{
    state.gameState = GS_None;
    state.menuState = MS_None;
}

GameState GameStateGet()
{
    return state.gameState;
}

MenuState MenuStateGet()
{
    return state.menuState;
}

void GameStateSet(GameState newState)
{
    state.gameState = newState;
}

void MenuStateSet(MenuState newState)
{
    state.menuState = newState;
}