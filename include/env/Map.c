#include "Map.h"

#include "Library.h"

// Wrapper around JSONFind() to print message if fail
json_value *MapTryFind(JSON *mapData, json_value *start, json_type type, char *data, int size);
int LoadAllEnties(JSON *mapData, json_value *entitiesStart, EntityIndexP **destination);

struct
{
    MapInfo info;
    EntityIndexP *contents;
    int n;
} map;

struct
{
    MapInfo info;
    EntityIndexP *contents;
    int n;
} bufferMap;

void MapInitialize()
{
    map.info = (MapInfo){0};
    map.contents = NULL;
    map.n = 0;
    bufferMap.info = (MapInfo){0};
    bufferMap.contents = NULL;
    bufferMap.n = 0;
}

void MapUninitialize()
{
    MapInfoDestroy(&map.info);
    if (map.contents)
        EntityManagerRemoveRange(*map.contents[0], *map.contents[map.n - 1] + 1);
    FREE(map.contents);
    map.info = (MapInfo){0};
    map.contents = NULL;
    map.n = 0;
}

int MapGenerateNew(JSON *mapData)
{
    json_value *json_helper;
#ifdef MAP_DEBUG
    char *filepath = mapData->originalFilename;
#endif
    bufferMap.info = MapInfoCreateFromJSON(mapData);
    if (bufferMap.info.uid == -1)
    {
#ifdef MAP_DEBUG
        log_warn("Failed to load map %s: Bad mapInfo", filepath);
#endif
        return 0;
    }

    if (!(json_helper = MapTryFind(mapData, NULL, json_integer, "layers", 6)))
        return 0;
    bufferMap.n = json_helper->u.integer;

    if (!(json_helper = MapTryFind(mapData, NULL, json_array, (char *)&bufferMap.n, sizeof(int))))
    {
        bufferMap.info.uid = -1;
        return 0;
    }
    json_value *entitiesStart = json_helper;

    if (entitiesStart->u.array.length != bufferMap.n)
    {
#ifdef MAP_DEBUG
        log_warn("Layers number does not match actual array size");
#endif

        bufferMap.info.uid = -1;
        return 0;
    }

    // Load in all entities
    bufferMap.contents = MALLOC_N(EntityIndexP, bufferMap.n);
    ALLOC_ERROR_CHECK(bufferMap.contents);

    int numLoadedEntites = LoadAllEnties(mapData, entitiesStart, &bufferMap.contents);
    if (numLoadedEntites != bufferMap.n)
    {
#ifdef MAP_DEBUG
        log_warn("Failed to load in entities");
#endif
        if (numLoadedEntites)
            EntityManagerRemoveRange(*bufferMap.contents[0], *bufferMap.contents[numLoadedEntites - 1] + 1);
        FREE(bufferMap.contents);
        bufferMap.info.uid = -1;
        return 0;
    }

    // If old map exist, delete it before copying in bufferMap
    if (map.contents && map.n > 0)
    {
        EntityManagerRemoveRange(*map.contents[0], *map.contents[map.n - 1] + 1);
        FREE(map.contents);
    }

    // Copy bufferMap into map
    map.info = MapInfoCreateFromJSON(mapData);
    if (map.info.uid == -1)
    {
#ifdef MAP_DEBUG
        log_warn("Failed to load map %s: Bad mapInfo", filepath);
#endif
        return 0;
    }
    map.contents = MALLOC_N(EntityIndexP, bufferMap.n);
    SDL_memcpy(map.contents, bufferMap.contents, sizeof(EntityIndexP) * bufferMap.n);
    map.n = bufferMap.n;

    // Clear bufferMap
    VectorClear(bufferMap.info.playerSpawns);
    VectorClear(bufferMap.info.enemySpawns);
    bufferMap.info.uid = -1;
    FREE(bufferMap.contents);
    bufferMap.n = 0;

    return 1;
}

void MapDraw()
{
    if (map.contents)
        for (int i = 0; i < map.n; i++)
        {
            EntityDrawIndex(map.contents[i]);
        }
}

EntityIndexP *MapGetContents()
{
    return map.contents;
}

unsigned int MapGetContentSize()
{
    return map.n;
}

MapInfo MapGetInfo()
{
    return map.info;
}

json_value *MapTryFind(JSON *mapData, json_value *start, json_type type, char *data, int size)
{

    json_value *json_helper;
    if (!(json_helper = JSONFind(mapData, start, type, data, size)))
    {
#ifdef MAPINFO_DEBUG
        char *filepath = mapData->originalFilename ? mapData->originalFilename : "Unknown";
        log_warn("Could not load mapData for %s: %s", filepath, data);
#endif
    }
    return json_helper;
}

int LoadAllEnties(JSON *mapData, json_value *entitiesStart, EntityIndexP **destination)
{
    json_value *json_helper;
    int numEnttiesLoaded = 0;
    int loops = bufferMap.n;
    const int IDOffset = 10000;
    int nextID = 0;
    for (int i = 0; i < loops; i++)
    {
        char *type_str = NULL;
        int x, y, w, h, m, c, r;
        SDL_Rect src = {0};

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_string, "type", 4)))
            return numEnttiesLoaded;
        type_str = json_helper->u.string.ptr;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "x", 1)))
            return numEnttiesLoaded;
        x = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "y", 1)))
            return numEnttiesLoaded;
        y = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "w", 1)))
            return numEnttiesLoaded;
        w = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "h", 1)))
            return numEnttiesLoaded;
        h = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "m", 1)))
            return numEnttiesLoaded;
        m = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "c", 1)))
            return numEnttiesLoaded;
        c = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_integer, "r", 1)))
            return numEnttiesLoaded;
        r = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, entitiesStart->u.array.values[i], json_object, "src", 3)))
            return numEnttiesLoaded;
        json_value *json_src = json_helper;

        if (!(json_helper = MapTryFind(mapData, json_src, json_integer, "x", 1)))
            return numEnttiesLoaded;
        src.x = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, json_src, json_integer, "y", 1)))
            return numEnttiesLoaded;
        src.y = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, json_src, json_integer, "w", 1)))
            return numEnttiesLoaded;
        src.w = json_helper->u.integer;

        if (!(json_helper = MapTryFind(mapData, json_src, json_integer, "h", 1)))
            return numEnttiesLoaded;
        src.h = json_helper->u.integer;

        EntityType type;
        if (strcmp(type_str, "player_spawn") && strcmp(type_str, "enemy_spawn"))
        {
            type = ET_MapObject;
        }
        else
        {
            bufferMap.n--;
            continue;
        }

        SDL_Rect dst = {x, y, w, h};

        EntityIndexP index = EntityManagerAdd(type, Vec2Create(x, y));
        Entity *entity = &ENTITY_ARRAY[*index];
        entity->id = IDOffset + nextID++;
        entity->drawables[0].dst = dst;
        entity->drawables[0].src = src;
        entity->drawables[0].rot = r;
        entity->drawables[0].rot_anchor = Vec2Create(0.5f, 0.5f);
        entity->nDrawables = 1;
        entity->mass = m;
        entity->isCollider = (SDL_bool)c;
        entity->isMovable = (SDL_bool)(m != 100000);
        EntityChangeHitboxOffset(entity, 0, 0);
        EntityChangeHitboxSize(entity, entity->drawables[0].dst.w, entity->drawables[0].dst.h);

        bufferMap.contents[numEnttiesLoaded] = index;
        numEnttiesLoaded++;
    }
    return numEnttiesLoaded;
}