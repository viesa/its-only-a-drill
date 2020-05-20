#ifndef STATE_H
#define STATE_H

typedef enum GameState
{
    GS_Menu,
    GS_Playing,
    GS_RoundFinished,
    GS_MatchFinished,
    GS_None,
    GS_Count
} GameState;

typedef enum MenuState
{
    MS_Splash,
    MS_Name,
    MS_MainMenu,
    MS_InGameMenu,
    MS_JoinLobby,
    MS_HostLobby,
    MS_RoundsLobby,
    MS_WaitingForLobby,
    MS_Lobby,
    MS_Options,
    MS_Resolution,
    MS_FPS,
    MS_KEYBINDING,
    MS_Audio,
    MS_CustomMap,
    MS_Skin,
    MS_Summary,
    MS_None,
    MS_Count
} MenuState;

typedef enum ConState
{
    CON_Offline,
    CON_Online,
    CON_None,
    CON_Count
} ConState;

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
ConState ConStateGet();
CLIState CLIStateGet();

void GameStateSet(GameState newState);
void MenuStateSet(MenuState newState);
void ConStateSet(ConState newState);
void CLIStateSet(CLIState newState);

#endif