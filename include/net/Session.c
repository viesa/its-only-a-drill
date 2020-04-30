#include "Session.h"

// For map parsing purposes
#define MAIN_LENGTH 3
#define MAPINFO_LENGTH 4
#define LIST_LENGTH 10
#define SRC_LENGTH 4
#define MAPINFO_INDEX 0
#define LAYER_INDEX 1
#define LIST_INDEX 2
#define SRC_INDEX 9
#define MAPINFO_STRING_INDEX 1

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size)
{
    Session badSession;
    badSession.id = -1;

    Session session;
    session.id = id;
    session.host = host;
    session.playersP = VectorCreate(sizeof(NetPlayer *), 10);
    VectorPushBack(session.playersP, session.host);
    session.rawMap = MALLOC_N(char, size);
    ALLOC_ERROR_CHECK(session.rawMap);
    SDL_memcpy(session.rawMap, rawmap, size);

    JSON *json = JSONCreateFromArray(session.rawMap, size);

    if (json == NULL ||
        json->value->type != json_object ||
        json->value->u.object.length != MAIN_LENGTH)
    {
        log_error("Could not load mapinfo from mapfile: JSON-data was badly formatted (Main)");
        return badSession;
    }

    json_value *mapInfo = JSONGetValue(json, (uint32_t[]){0}, 1);

    if (mapInfo == NULL ||
        mapInfo->type != json_object ||
        mapInfo->u.object.length != MAPINFO_LENGTH)
    {
        log_error("Could not load mapinfo from mapfile: JSON-data was badly formatted (MapInfo)");
        return badSession;
    }

    SDL_bool badLoad = SDL_FALSE;
    json_object_entry *mapInfoEntries = mapInfo->u.object.values;
    for (uint32_t i = 0; i < MAPINFO_LENGTH; i++)
    {
        if (i == MAPINFO_STRING_INDEX)
        {
            if (mapInfoEntries[i].value->type != json_string)
            {
                badLoad = SDL_TRUE;
                break;
            }
        }
        else
        {
            if (mapInfoEntries[i].value->type != json_integer)
            {
                badLoad = SDL_TRUE;
                break;
            }
        }
    }
    if (badLoad)
    {
        log_error("Could not load mapinfo from mapfile: JSON-data was badly formatted (MapInfo)");
        return badSession;
    }
    //---------------

    // Fetch map info
    session.mapName = MALLOC_N(char, mapInfoEntries[1].value->u.string.length);
    strcpy(session.mapName, mapInfoEntries[1].value->u.string.ptr);
    session.mapMaxPlayers = mapInfoEntries[3].value->u.integer;
    // -------------------

    JSONDestroy(json);
    return session;
}

void SessionDestroy(Session *session)
{
    FREE(session->rawMap);
    FREE(session->mapName);
}

NetPlayer **SessionGetPlayers(Session *session)
{
    return (NetPlayer **)session->playersP->data;
}
