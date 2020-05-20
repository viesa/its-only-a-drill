#include "State.h"
#include "Menu.h"

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
    if (newState == GS_Playing && GameStateGet() == GS_Menu)
        ScoreboardClear();
    state.gameState = newState;
}

void MenuStateSet(MenuState newState)
{
    MenuResetActiveIndex();
    MenuResetFetchLobbyTimer();
    MenuResetFetchSessionsTimer();

    if (newState == MS_MainMenu)
    {
        ScoreboardClear();
    }
    if (newState == MS_Name)
    {
        InputClearPortalContent();
    }
    if (newState != MS_None && newState != MS_InGameMenu)
    {
        int x = WindowGetWidth() / 2;
        int y = WindowGetHeight() / 2;
        int w = WindowGetWidth() / 3;
        int h = WindowGetHeight() / 3;
        CameraSetViewPort((SDL_Rect){x, y, w, h});
        CameraSetScale(0.33f);
    }
    else if (newState != MS_InGameMenu)
    {
        int x = 0;
        int y = 0;
        int w = WindowGetWidth();
        int h = WindowGetHeight();
        CameraSetViewPort((SDL_Rect){x, y, w, h});
        CameraSetScale(1.0f);
    }
    if (state.menuState == MS_None && newState == MS_InGameMenu)
    {
        TransitionStart(TT_MapToMenu, 0.30f);
        MenuSetStartedInTransistion(SDL_TRUE);
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