#include "NPCManager.h"

struct
{
    Player *player;
    Vector *npcs;
} npcManager;

void NPCManagerInitialize(Player *player)
{
    npcManager.npcs = VectorCreate(sizeof(NPC *), 10);
}
void NPCManagerUninitialize()
{
    for (int i = 0; i < npcManager.npcs->size; i++)
    {
        NPC *npc = ((NPC **)npcManager.npcs->data)[i];
        NPCDestroy(npc);
    }
    VectorDestroy(npcManager.npcs);
}

void NPCManagerUpdate()
{
    for (int i = 0; i < npcManager.npcs->size; i++)
    {
        NPC *npc = ((NPC **)npcManager.npcs->data)[i];
        NPCUpdate(npc);
    }
}
void NPCManagerDrawAllNPCS(Camera *camera)
{
    for (int i = 0; i < npcManager.npcs->size; i++)
    {
        NPC *npc = ((NPC **)npcManager.npcs->data)[i];
        NPCDraw(npc, camera);
    }
}

void NPCManagerAddNew(Vec2 pos)
{
    NPC *_new = NPCCreate(pos, npcManager.player);
    VectorPushBack(npcManager.npcs, _new);
}

void NPCManagerResetNPCS()
{
    for (int i = 0; i < npcManager.npcs->size; i++)
    {
        NPC *npc = ((NPC **)npcManager.npcs->data)[i];
        NPCDestroy(npc);
    }
    VectorClear(npcManager.npcs);
}