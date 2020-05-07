#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "Vec2.h"

typedef struct SpawnPoint
{
    Vec2 position;
    int id;
} SpawnPoint;

SpawnPoint SpawnPointCreate(Vec2 position, int id);

#endif