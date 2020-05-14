#include "MapInfo.h"

// Wrapper around JSONFind() to print message if fail
json_value *MapInfoTryFind(JSON *mapData, json_value *start, json_type type, char *data, int size);

int LoadAllSpawnPoints(MapInfo *mapInfo, JSON *mapData, json_value *entitiesStart, int arrayItems);

MapInfo MapInfoCreate(char *filepath)
{
    JSON *mapData = JSONCreate(filepath);
    MapInfo mapInfo = MapInfoCreateFromJSON(mapData);
    JSONDestroy(mapData);
    return mapInfo;
}

MapInfo MapInfoCreateFromJSON(JSON *mapData)
{
    // This is returned if mapData failed to load
    MapInfo badInfo;
    badInfo.uid = -1;
    // This is returned if mapdata sucessfully loaded and was parsed
    MapInfo mapInfo;

    // Check if JSON is valid
    if (!mapData)
    {
        return badInfo;
    }

    json_value *json_mapInfo;
    json_value *json_helper;
#ifdef MAPINFO_DEBUG
    char *filepath = mapData->originalFilename;
#endif

    // Find mapinfo JSON-object
    char *to_find[6] =
        {
            "mapInfo",
            "uid",
            "name", // STRING
            "difficulty",
            "maxPlayers",
            "maxBots",
        };

    // Find mapinfo JSON-object
    if (!(json_helper = MapInfoTryFind(mapData, NULL, json_object, to_find[0], strlen(to_find[0]))))
        return badInfo;
    json_mapInfo = json_helper;

    // Find contents of mapInfo JSON-object
    if (!(json_helper = MapInfoTryFind(mapData, json_mapInfo, json_integer, to_find[1], strlen(to_find[1]))))
        return badInfo;
    mapInfo.uid = json_helper->u.integer;

    if (!(json_helper = MapInfoTryFind(mapData, json_mapInfo, json_string, to_find[2], strlen(to_find[2]))))
        return badInfo;
    if (json_helper->u.string.length > MAX_NAME_SIZE)
    {
#ifdef MAPINFO_DEBUG
        log_warn("Could not load mapInfo for %s: %s", filepath, "name was too long");
#endif
        return badInfo;
    }
    strcpy(mapInfo.name, json_helper->u.string.ptr);

    if (!(json_helper = MapInfoTryFind(mapData, json_mapInfo, json_integer, to_find[3], strlen(to_find[3]))))
        return badInfo;
    mapInfo.difficulty = json_helper->u.integer;

    if (!(json_helper = MapInfoTryFind(mapData, json_mapInfo, json_integer, to_find[4], strlen(to_find[4]))))
        return badInfo;
    mapInfo.maxPlayers = json_helper->u.integer;

    if (!(json_helper = MapInfoTryFind(mapData, json_mapInfo, json_integer, to_find[5], strlen(to_find[5]))))
        return badInfo;
    mapInfo.maxBots = json_helper->u.integer;

    if (strlen(mapData->originalFilename) > MAX_FILENAME_SIZE)
    {
#ifdef MAPINFO_DEBUG
        log_warn("Could not load mapInfo for %s: %s", filepath, "name was too long");
#endif
        return badInfo;
    }
    strcpy(mapInfo.filename, mapData->originalFilename);

    // Load in all spawnpoints
    if (!(json_helper = MapInfoTryFind(mapData, NULL, json_integer, "layers", 6)))
        return badInfo;
    int arrayItems = json_helper->u.integer;
    if (!(json_helper = MapInfoTryFind(mapData, NULL, json_array, (char *)&arrayItems, sizeof(int))))
        return badInfo;
    json_value *entitiesStart = json_helper;

    mapInfo.playerSpawns = VectorCreate(sizeof(SpawnPoint), 5);
    mapInfo.enemySpawns = VectorCreate(sizeof(SpawnPoint), 5);

    int result = LoadAllSpawnPoints(&mapInfo, mapData, entitiesStart, arrayItems);
    if (result == -1)
    {
#ifdef MAPINFO_DEBUG
        log_warn("Failed to load in spawnpoints");
#endif
        VectorDestroy(mapInfo.playerSpawns);
        VectorDestroy(mapInfo.enemySpawns);
        return badInfo;
    }

    return mapInfo;
}

json_value *MapInfoTryFind(JSON *mapData, json_value *start, json_type type, char *data, int size)
{

    json_value *json_helper;
    if (!(json_helper = JSONFind(mapData, start, type, data, size)))
    {
#ifdef MAPINFO_DEBUG
        char *filepath = mapData->originalFilename;
        log_warn("Could not load mapInfo for %s: %s", filepath, data);
#endif
    }
    return json_helper;
}

int LoadAllSpawnPoints(MapInfo *mapInfo, JSON *mapData, json_value *entitiesStart, int arrayItems)
{
    json_value *json_helper;
    for (int i = 0; i < arrayItems; i++)
    {
        char *type_str = NULL;
        int x, y;

        if (!(json_helper = MapInfoTryFind(mapData, entitiesStart->u.array.values[i], json_string, "type", 4)))
            return -1;
        type_str = json_helper->u.string.ptr;

        if (!(json_helper = MapInfoTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "x", 1)))
            return -1;
        x = json_helper->u.integer;

        if (!(json_helper = MapInfoTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "y", 1)))
            return -1;
        y = json_helper->u.integer;

        if (!strcmp(type_str, "player_spawn"))
        {
            SpawnPoint spawnPoint = SpawnPointCreate(Vec2Create(x, y), mapInfo->playerSpawns->size);
            VectorPushBack(mapInfo->playerSpawns, &spawnPoint);
            continue;
        }
        else if (!strcmp(type_str, "enemy_spawn"))
        {
            SpawnPoint spawnPoint = SpawnPointCreate(Vec2Create(x, y), mapInfo->enemySpawns->size);
            VectorPushBack(mapInfo->enemySpawns, &spawnPoint);
            continue;
        }
    }
    return 1;
}

void MapInfoDestroy(MapInfo *mapInfo)
{
    if (mapInfo->uid != -1)
    {
        VectorDestroy(mapInfo->playerSpawns);
        VectorDestroy(mapInfo->enemySpawns);
    }
}

SpawnPoint *MapInfoGetPlayerSpawns(MapInfo *mapInfo)
{
    return (SpawnPoint *)mapInfo->playerSpawns->data;
}

SpawnPoint *MapInfoGetEnemySpawns(MapInfo *mapInfo)
{
    return (SpawnPoint *)mapInfo->enemySpawns->data;
}