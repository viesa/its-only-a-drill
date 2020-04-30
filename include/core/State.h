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
    MS_Splash,
    MS_Name,
    MS_MainMenu,
    MS_JoinLobby,
    MS_HostLobby,
    MS_WaitingForLobby,
    MS_Lobby,
    MS_Options,
    MS_Resolution,
    MS_FPS,
    MS_CustomMap,
    MS_None,
    MS_NStates
} MenuState;

void StateInitialize();

GameState GameStateGet();
MenuState MenuStateGet();

void GameStateSet(GameState newState);
void MenuStateSet(MenuState newState);

#endif