#include "State.h"

struct State
{
    GameState gameState;
    MenuState menuState;
    ConState conState;
    CLIState CLIState;
} state;

void StateInitialize()
{
    state.gameState = GS_None;
    state.menuState = MS_None;
    state.conState = CON_None;
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

ConState ConStateGet()
{
    return state.conState;
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

void ConStateSet(ConState newState)
{
    state.conState = newState;
}

void CLIStateSet(CLIState newState)
{
    state.CLIState = newState;
}