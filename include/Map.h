#ifndef MAP_H
#define MAP_H

#include "core/EntityManager.h"
#include "core/JSON.h"

#define MAIN_LENGTH 3
#define MAPINFO_LENGTH 4
#define LIST_LENGTH 10
#define SRC_LENGTH 4

#define MAPINFO_INDEX 0
#define LAYER_INDEX 1
#define LIST_INDEX 2
#define SRC_INDEX 9
#define MAPINFO_STRING_INDEX 1

typedef struct Map
{
    Entity **contents;
    uint32_t n;
    EntityManager *entityManager;
} Map;

Map MapCreate(JSON *mapdata, EntityManager *entityManager);
void MapDestroy(Map *map);

void MapDraw(Map *map, Camera *camera);

#endif