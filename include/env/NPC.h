#ifndef NPC_H
#define NPC_H

#include "Dependencies.h"
#include "Player.h"
#include "EntityManager.h"
#include "Anim.h"
#include "Weapon.h"
#include "CollisionDetection.h"

typedef struct MovePattern MovePattern;
typedef struct NPC NPC;

typedef enum NPCState
{
    NPC_Passive,
    NPC_Neutral,
    NPC_Fight,
    NPC_Aggressive,
    NPC_Dead,
    NPC_Count
} NPCState;

NPC *NPCCreate(Vec2 pos);
void NPCDestroy(NPC *npc);

void NPCUpdate(NPC *npc);
void NPCDraw(NPC *npc);

void NPCSetSpriteSheet(NPC *npc, SpriteSheet spriteSheet);

void NPCKill(NPC *npc);

void NPCUpdateBehavior(NPC *npc);
void NPCUpdateBehaviorNeutral(NPC *npc);
void NPCUpdateBehaviorFight(NPC *npc);
void NPCUpdateBehaviorAggressive(NPC *npc);

void NPCMoveTo(NPC *npc, Vec2 moveTo);

void NPCSwitchBehaviorState(NPC *npc);

void NPCShoot(NPC *npc);

void NPCUpdateAnimation(NPC *npc);

#endif