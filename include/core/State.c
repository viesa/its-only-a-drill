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
    state.menu->activeIndex = 0;
    if (state.menu)
    {
        state.menu->fetchSessionsTimer = FETCH_SESSIONS_INTERVAL;
        state.menu->fetchLobbyTimer = FETCH_LOBBY_INTERVAL;
    }
    if (newState == MS_Name)
    {
        InputClearPortalContent();
    }
    if (newState != MS_None && newState != MS_InGameMenu)
    {
        int x = state.menu->gfx->window->width / 2;
        int y = state.menu->gfx->window->height / 2;
        int w = state.menu->gfx->window->width / 3;
        int h = state.menu->gfx->window->height / 3;
        CameraSetViewPort(state.menu->camera, (SDL_Rect){x, y, w, h});
        CameraSetScale(state.menu->camera, 0.33f);
    }
    else if (newState != MS_InGameMenu)
    {
        int x = 0;
        int y = 0;
        int w = state.menu->gfx->window->width;
        int h = state.menu->gfx->window->height;
        CameraSetViewPort(state.menu->camera, (SDL_Rect){x, y, w, h});
        CameraSetScale(state.menu->camera, 1.0f);
    }
    if (state.menuState == MS_None && newState == MS_InGameMenu)
    {
        TransitionStart(TT_MapToMenu, 0.30f);
        state.menu->startedInTransition = SDL_TRUE;
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