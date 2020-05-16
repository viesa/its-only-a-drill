#ifndef NPCMANAGER_H
#define NPCMANAGER_H

#include "Dependencies.h"
#include "Vector.h"
#include "NPC.h"

void NPCManagerInitialize(Player *player);
void NPCManagerUninitialize();

void NPCManagerUpdate();
void NPCManagerDrawAllNPCS();

void NPCManagerAddNew(Vec2 pos);

void NPCManagerClearNPCS();

#endif