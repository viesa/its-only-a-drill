#ifndef MAP_H
#define MAP_H

#include "core/Entity.h"
#include "core/JSON.h"

typedef struct Map
{
    Entity *contents;
    uint32_t n;
} Map;

Map MapCreate(JSON *mapdata);
void MapDestroy(Map *map);

#endif