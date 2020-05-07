#include "MapList.h"

typedef struct MapList
{
    char directory[20];
    // Vector of mapInfo-structs, NOT the map-entities
    Vector *allMaps;
    float lastUpdate;
} MapList;

MapList *MapListCreate(char *directory)
{
    MapList *mapList = MALLOC(MapList);
    ALLOC_ERROR_CHECK(mapList);

    if (strlen(directory) > 20)
    {
#ifdef MAPLIST_DEBUG
        log_error("Directory string size too big for mapList");
#endif
        return NULL;
    }
    strcpy(mapList->directory, directory);
    mapList->allMaps = VectorCreate(sizeof(MapInfo), 5);
    mapList->lastUpdate = 1.0f;
    MapListUpdate(mapList);
    return mapList;
}
void MapListDestroy(MapList *mapList)
{
    for (int i = 0; i < mapList->allMaps->size; i++)
    {
        MapInfoDestroy(&MapListGetMaps(mapList)[i]);
    }
    if (mapList)
    {
        FREE(mapList->directory);
        VectorDestroy(mapList->allMaps);
    }
    FREE(mapList);
}

void MapListUpdate(MapList *mapList)
{
    if (mapList->lastUpdate < 1.0f)
    {
        mapList->lastUpdate += ClockGetDeltaTime();
        return;
    }
    mapList->lastUpdate = 0.0f;

    DIR *d;
    struct dirent *dir;
    d = opendir(mapList->directory);
    if (d)
    {
        VectorClear(mapList->allMaps);
        for (int i = 0; (dir = readdir(d)) != NULL; i++)
        {
            // Skip "." and ".." if they appear
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
            {
                size_t bytes = (strlen(mapList->directory) + 1 + strlen(dir->d_name));

                // Create a fullpath to the file to be loaded
                char *fullpath = CALLOC(bytes, char);
                ALLOC_ERROR_CHECK(fullpath);
                sprintf(fullpath, "%s%c%s", mapList->directory, '/', dir->d_name);

                MapInfo mapInfo = MapInfoCreate(fullpath);
                if (mapInfo.uid != -1)
                {
                    VectorPushBack(mapList->allMaps, &mapInfo);
                }

                // Clean up
                SDL_free(fullpath);
            }
        }

        // Only attempt to close "d" if it was opened
        closedir(d);
    }
}

MapInfo *MapListGetMaps(MapList *mapList)
{
    return (MapInfo *)mapList->allMaps->data;
}

size_t MapListGetNumMaps(MapList *mapList)
{
    return mapList->allMaps->size;
}