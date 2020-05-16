#ifndef MAP_H
#define MAP_H

#include "Dependencies.h"
#include "EntityManager.h"
#include "JSON.h"
#include "MapInfo.h"
#include "SpawnPoint.h"

void MapInitialize();
void MapUninitialize();

int MapGenerateNew(JSON *mapdata);

void MapDraw();

EntityIndexP *MapGetContents();
unsigned int MapGetContentSize();

MapInfo MapGetInfo();

#endif