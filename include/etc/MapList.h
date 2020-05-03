#ifndef MAPLIST_H
#define MAPLIST_H

#include "Dependencies.h"
#include "Dirent.h"
#include "JSON.h"

#include "Map.h"

#define MAX_MAPS 5

typedef struct MapListEntry
{
    uint32_t uid;
    char name[50];
    char filename[50];
    uint8_t difficulty;
    uint8_t maxPlayers;
} MapListEntry;

typedef struct MapList
{
    char directory[20];
    MapListEntry allMaps[MAX_MAPS];
    uint16_t nMaps;
    float lastUpdate;
} MapList;

///\param directory: Specifies which directory to load the map files from
MapList MapListCreate(char *directory);
void MapListDestroy(MapList *mapList);

void MapListUpdate(MapList *mapList);

#endif