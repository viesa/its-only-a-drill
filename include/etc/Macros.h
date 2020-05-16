#define __USE_MINGW_ANSI_STDIO 1

// #define SMALLWINDOW_PUBLICSERVER_RAWPACKAGERLOGS

#ifdef SMALLWINDOW_PUBLICSERVER_RAWPACKAGERLOGS
#define WINDOW_DEBUG
#define PACKAGER_DEBUG_RAWINOUT
#define CLIENTMANAGER_DEBUG
#endif

// If defined, mapList will only import maps on start
#define WIN_DEBUG

#define ANY_DEBUG

#ifdef ANY_DEBUG
// #define MAIN_DEBUG

// #define APP_DEBUG
// #define APP_DEBUG_STRICT
// #define APPSERVER_DEBUG

// #define CLIENTMANAGER_DEBUG
// #define CLIENT_DEBUG
// #define CLIENT_IO
// #define SERVER_DEBUG
// #define PACKAGER_DEBUG_RAWINOUT

// #define MAP_DEBUG
// #define MAPINFO_DEBUG
// #define MAPLIST_DEBUG

// #define WINDOW_DEBUG
// #define GRAPHICS_DEBUG

// #define ENTITY_DEBUG
// #define WEAPON_DEBIG
// #define Debug_Weapon_GetHitInfo
// #define PLAYER_RESPAWN

// #define VECTOR_DEBUG
// #define VECTOR_DEBUG_STRICT
#endif

// If defined friction formula Real runs, else computed
//#define frictionReal

// If defined, client will connect with 127.0.0.1
// #define LOCAL_SERVER