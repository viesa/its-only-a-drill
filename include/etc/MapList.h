#ifndef MAPLIST_H
#define MAPLIST_H

#include "Dependencies.h"
#include "Dirent.h"
#include "JSON.h"
#include "Map.h"

typedef struct MapList MapList;

///\param directory: Specifies which directory to load the map files from
MapList *MapListCreate(char *directory);
void MapListDestroy(MapList *mapList);

// Loads in all the JSON-files from directory
void MapListUpdate(MapList *mapList);

MapInfo *MapListGetMaps(MapList *mapList);
size_t MapListGetNumMaps(MapList *mapList);

#endif