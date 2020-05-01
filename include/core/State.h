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

// Used only on server
typedef enum CLIState
{
    CS_Main,
    CS_PlayerList,
    CS_SessionList,
    CS_Traffic,
    CS_None,
    CS_Count
} CLIState;

void StateInitialize();

GameState GameStateGet();
MenuState MenuStateGet();
CLIState CLIStateGet();

void GameStateSet(GameState newState);
void MenuStateSet(MenuState newState);
void CLIStateSet(CLIState newState);

#endif