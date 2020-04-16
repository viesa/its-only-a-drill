#ifndef MAPLIST_H
#define MAPLIST_H

#ifdef linux
#include <dirent.h>
#endif
#ifdef _WIN32
#include "core/dirent.h"
#endif
#include <string.h>
#include "core/List.h"
#include "core/JSON.h"

#include "Map.h"

typedef struct MapListEntry
{
    uint32_t uid;
    char *name;
    char *filename;
    uint8_t difficulty;
    uint8_t maxPlayers;
} MapListEntry;

typedef struct MapList
{
    char *directory;
    List allMaps;
} MapList;

///\param directory: Specifies which directory to load the map files from
MapList MapListCreate(char *directory);
void MapListDestroy(MapList *mapList);

void MapListUpdate(MapList *mapList);

void MapListClear(MapList *mapList);

#endif