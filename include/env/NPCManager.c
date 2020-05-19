#include "NPCManager.h"

struct
{
    Vector *npcs;
} npcManager;

void NPCManagerInitialize()
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
void NPCManagerDrawAllNPCS()
{
    for (int i = 0; i < npcManager.npcs->size; i++)
    {
        NPC *npc = ((NPC **)npcManager.npcs->data)[i];
        NPCDraw(npc);
    }
}

void NPCManagerAddNew(Vec2 pos)
{
    NPC *_new = NPCCreate(pos);
    VectorPushBack(npcManager.npcs, &_new);
}

void NPCManagerClearNPCS()
{
    for (int i = 0; i < npcManager.npcs->size; i++)
    {
        NPC *npc = ((NPC **)npcManager.npcs->data)[i];
        NPCDestroy(npc);
    }
    VectorClear(npcManager.npcs);
}