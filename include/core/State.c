#include "State.h"
#include "Menu.h"

struct State
{
    GameState gameState;
    MenuState menuState;
    ConState conState;
    CLIState CLIState;

    Menu *menu;
} state;

void StateInitialize()
{
    state.gameState = GS_None;
    state.menuState = MS_None;
    state.conState = CON_None;
    state.CLIState = CS_None;

    state.menu = NULL;
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
    if (state.menu)
    {
        state.menu->fetchSessionsTimer = FETCH_SESSIONS_INTERVAL;
        state.menu->fetchLobbyTimer = FETCH_LOBBY_INTERVAL;
    }
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

void StateSetMenu(void *menu)
{
    state.menu = (Menu *)menu;
}