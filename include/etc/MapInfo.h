#ifndef MAPINFO_H
#define MAPINFO_H

#include "Dependencies.h"
#include "JSON.h"
#include "Vector.h"
#include "SpawnPoint.h"

#define MAX_FILENAME_SIZE 50
#define MAX_NAME_SIZE 50

typedef struct MapInfo
{
    int uid;
    char name[MAX_FILENAME_SIZE];
    char filename[MAX_NAME_SIZE];
    int difficulty;
    int maxPlayers;
    int maxBots;
    Vector *playerSpawns;
    Vector *enemySpawns;
} MapInfo;

MapInfo MapInfoCreate(char *filepath);
MapInfo MapInfoCreateFromJSON(JSON *mapdata);

void MapInfoDestroy(MapInfo *mapInfo);

SpawnPoint *MapInfoGetPlayerSpawns(MapInfo *mapInfo);
SpawnPoint *MapInfoGetEnemySpawns(MapInfo *mapInfo);

#endif