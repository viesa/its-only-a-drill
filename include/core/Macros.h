//#define ANY_DEBUG

#ifdef ANY_DEBUG
#define MAIN_DEBUG

#define APP_DEBUG
#define APP_DEBUG_STRICT

#define UDPCLIENT_DEBUG
// #define UDPCLIENT_DEBUG_STRICT
// #define UDPSERVER_DEBUG_RAWINOUT

#define UDPSERVER_DEBUG
// #define UDPSERVER_DEBUG_STRICT
// #define UDPSERVER_DEBUG_RAWINOUT

#define WINDOW_DEBUG
#define GRAPHICS_DEBUG

//#define ENTITY_DEBUG
//#define WEAPON_DEBIG

#define VECTOR_DEBUG
#define VECTOR_DEBUG_STRICT
#endif

// If defined friction formula Real runs, else computed
//#define frictionReal

// If defined, multiple connection can be created from the same computer. It's a quick fix to prevent unused connections on the PI-server
// #define UDPSERVER_LOCAL