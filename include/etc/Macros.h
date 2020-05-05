//#define __USE_MINGW_ANSI_STDIO 1

// #define SMALLWINDOW_PUBLICSERVER_RAWPACKAGERLOGS

#ifdef SMALLWINDOW_PUBLICSERVER_RAWPACKAGERLOGS
#define WINDOW_DEBUG
#define PACKAGER_DEBUG_RAWINOUT
#endif

// #define ANY_DEBUG

#ifdef ANY_DEBUG
// #define MAIN_DEBUG

// #define APP_DEBUG
// #define APP_DEBUG_STRICT

// // #define CLIENT_DEBUG
// #define CLIENT_IO
// #define SERVER_DEBUG
// #define PACKAGER_DEBUG_RAWINOUT

#define WINDOW_DEBUG
// #define GRAPHICS_DEBUG

//#define ENTITY_DEBUG
//#define WEAPON_DEBIG

// #define VECTOR_DEBUG
// #define VECTOR_DEBUG_STRICT
#endif

// If defined friction formula Real runs, else computed
//#define frictionReal

// If defined, client will connect with 127.0.0.1
// #define LOCAL_SERVER