#ifndef MAP_H
#define MAP_H

#include "core/Dependencies.h"
#include "core/EntityManager.h"
#include "core/JSON.h"

void MapInitialize();
void MapUninitialize();

void MapGenerateNew(JSON *mapdata);

void MapDraw(Camera *camera);

EntityIndexP *MapGetContents();
unsigned int MapGetContnetSize();

#endif