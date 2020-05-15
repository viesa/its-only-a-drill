#include "NPC.h"

struct MovePattern
{
    Vec2 points[20];
    int nPoints;
};

MovePattern MovePatternCreate()
{
    MovePattern pattern;
    pattern.points[0] = Vec2Create(200.0f, 200.0f);
    pattern.points[1] = Vec2Create(283.0f, 0.0f);
    pattern.points[2] = Vec2Create(200.0f, -200.0f);
    pattern.points[3] = Vec2Create(0.0f, -283.0f);
    pattern.points[4] = Vec2Create(-200.0f, 200.0f);
    pattern.points[5] = Vec2Create(-283.0f, 0.0f);
    pattern.points[6] = Vec2Create(-200.0f, -200.0f);
    pattern.points[7] = Vec2Create(50.0f, 0.0f);
    pattern.points[8] = Vec2Create(150.0f, 100.0f);
    pattern.points[9] = Vec2Create(200.0f, 200.0f);
    pattern.points[10] = Vec2Create(100.0f, 100.0f);
    pattern.points[11] = Vec2Create(-100.0f, -100.0f);
    pattern.nPoints = 12;
    return pattern;
}

struct NPC
{
    Player *player;

    EntityIndexP entity;
    NPCState state;
    SpriteSheet spriteSheet;

    Anim leg;
    Anim body;
    Vec2 forward;
    InventoryListItems inventory;

    Vec2 desiredPos;
    int movePatternIndex;
    MovePattern movePattern;

    float movementSpeed;
    float aggravationRadius;
    int DPBoxSize;
};

NPC *NPCCreate(Vec2 pos, Player *player)
{
    NPC *npc = MALLOC(NPC);
    ALLOC_ERROR_CHECK(npc);
    npc->player = player;
    npc->entity = EntityManagerAdd(ET_Player, pos);
    npc->spriteSheet = SS_Character_Prisoner;
    npc->leg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, SS_Character_ChernobylWorker, 4, 0.05f);
    npc->body = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, SS_Character_ChernobylWorker, 4, 0.05f);
    npc->forward = Vec2Create(1.0f, 0.0f);
    npc->inventory = InventoryCreate();
    npc->desiredPos = pos;
    npc->movePatternIndex = 0;
    npc->movePattern = MovePatternCreate();
    npc->movementSpeed = 500.0f;
    npc->aggravationRadius = 250.0f;
    npc->DPBoxSize = 6;
    return npc;
}
void NPCDestroy(NPC *npc)
{
    FREE(npc);
}

void NPCUpdate(NPC *npc)
{
    if (ENTITY_ARRAY[*npc->entity].health <= 0)
    {
        npc->state = NPC_Dead;
    }
    NPCUpdateAnimation(npc);
}
void NPCDraw(NPC *npc, Camera *camera)
{
    EntityDrawIndex(npc->entity, camera);
}

void NPCSetSpriteSheet(NPC *npc, SpriteSheet spriteSheet)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    for (int i = 0; i < entity->nDrawables; i++)
        entity->drawables[i].spriteSheet = spriteSheet;
    AnimChangeSpriteSheet(&npc->body, spriteSheet);
    AnimChangeSpriteSheet(&npc->leg, spriteSheet);
    npc->spriteSheet = spriteSheet;
}

void NPCKill(NPC *npc)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    entity->health = 0;
    entity->drawables[0] = DrawableCreate((SDL_Rect){0, 0, 70, 70}, (SDL_Rect){entity->position.x, entity->position.y, 77, 63}, npc->spriteSheet);
    entity->nDrawables = 1;
    entity->hitboxIndex = 0;
    npc->state = NPC_Dead;
}

void NPCUpdateBehavior(NPC *npc, Camera *camera)
{
    NPCSwitchBehaviorState(npc);

    switch (npc->state)
    {
    case NPC_Passive:
    {
        break;
    }
    case NPC_Neutral:
    {
        NPCUpdateBehaviorNeutral(npc);
        break;
    }
    case NPC_Fight:
    {
        NPCUpdateBehaviorFight(npc, camera);
        break;
    }
    case NPC_Aggressive:
    {
        NPCUpdateBehaviorAggressive(npc);
        break;
    }
    case NPC_Dead:
    {
        break;
    }
    default:
        break;
    }
}
void NPCUpdateBehaviorNeutral(NPC *npc)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    NPCMoveTo(npc, npc->desiredPos);

    // Box around desired position
    SDL_Rect boxDP = {(int)npc->desiredPos.x - npc->DPBoxSize / 2,
                      (int)npc->desiredPos.y - npc->DPBoxSize / 2,
                      npc->DPBoxSize,
                      npc->DPBoxSize};

    if (SDL_HasIntersection(&entity->drawables[entity->hitboxIndex].dst, &boxDP))
    {
        npc->desiredPos = Vec2Add(entity->position, npc->movePattern.points[npc->movePatternIndex]);
        if (npc->movePatternIndex == 11 || npc->movePatternIndex <= 9)
        {
            npc->movePatternIndex = 10;
        }
        else
        {
            npc->movePatternIndex++;
        }
    }
}
void NPCUpdateBehaviorFight(NPC *npc, Camera *camera)
{
    NPCShoot(npc, camera);
    if (PlayerGetEntity(npc->player)->health < 0)
    {
        npc->state = NPC_Neutral;
    }
}
void NPCUpdateBehaviorAggressive(NPC *npc)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    NPCMoveTo(npc, npc->desiredPos);

    // Box around desired position
    SDL_Rect boxDP = {(int)npc->desiredPos.x - npc->DPBoxSize / 2,
                      (int)npc->desiredPos.y - npc->DPBoxSize / 2,
                      npc->DPBoxSize,
                      npc->DPBoxSize};

    if (SDL_HasIntersection(&entity->drawables[entity->hitboxIndex].dst, &boxDP))
    {
        npc->desiredPos = Vec2Add(entity->position, npc->movePattern.points[npc->movePatternIndex]);
        npc->movePatternIndex = npc->movePatternIndex >= 9 ? 0 : npc->movePatternIndex + 1;
    }
}

void NPCMoveTo(NPC *npc, Vec2 moveTo)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    Vec2 enemyVec = Vec2Sub(moveTo, RectMid(entity->drawables[0].dst));
    Vec2 unitenemyVec = Vec2Unit(enemyVec);
    Vec2 enemyMovement = Vec2MulL(unitenemyVec, npc->movementSpeed);
    entity->Force.x += enemyMovement.x;
    entity->Force.y += enemyMovement.y;
}

void NPCSwitchBehaviorState(NPC *npc)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    Entity *pEntity = PlayerGetEntity(npc->player);
    if (entity->health <= 0)
    {
        npc->state = NPC_Dead;
    }
    else if (npc->state != NPC_Dead)
    {
        Vec2 enemyToPlayer = Vec2Sub(pEntity->position, entity->position);
        if (entity->health < 100 || Vec2Len(enemyToPlayer) <= npc->aggravationRadius)
        {
            npc->state = NPC_Aggressive;
            npc->state = NPC_Aggressive;
        }
    }
}

void NPCShoot(NPC *npc, Camera *camera)
{
    Item *item = &npc->inventory.contents[npc->inventory.top - 1];
    item->Stats.currentTime -= ClockGetDeltaTimeMS();
    if (item->Stats.currentTime <= 0)
    {
        Entity *entity = &ENTITY_ARRAY[*npc->entity];
        Entity *pEntity = PlayerGetEntity(npc->player);

        item->Stats.currentTime = item->Stats.cooldownMS;

        Vec2 entityToPoint = Vec2Sub(pEntity->position, entity->position);
        Vec2 unit = Vec2Unit(entityToPoint);
        Vec2 itemFalloff = Vec2MulL(unit, item->Stats.falloff);
        Vec2 makeDestination;
        makeDestination.x = (float)entity->drawables[0].dst.x + (entity->drawables[0].dst.w / 2) + itemFalloff.x;
        makeDestination.y = (float)entity->drawables[0].dst.x + (entity->drawables[0].dst.w / 2) + itemFalloff.y;

        SDL_Point point;
        point.x = entity->drawables[0].dst.x + (entity->drawables[0].dst.w / 2);
        point.y = entity->drawables[0].dst.y + (entity->drawables[0].dst.h / 2);

        CameraDrawLine(camera, point.x, point.y, makeDestination.x, makeDestination.y, (SDL_Color){255, 50, 50, 150});
        // push back
        entity->Force.x -= itemFalloff.x;
        entity->Force.y -= itemFalloff.y;
        // for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
        // {
        // if (i != *index && ENTITY_ARRAY[i].isNPC == 0)
        //     RayScanSingelplayer(i, makeDestination, point, item, itemFalloff);
        // }
    }
}

void NPCUpdateAnimation(NPC *npc)
{
    AnimUpdate(&npc->leg, ClockGetDeltaTime());
    AnimUpdate(&npc->body, ClockGetDeltaTime());

    if (Vec2LenSq(ENTITY_ARRAY[*npc->entity].Velocity) == 0.0f || Vec2LenSq(ENTITY_ARRAY[*npc->entity].Force) == 0.0f)
    {
        AnimStop(&npc->leg);
        AnimPause(&npc->body);
        if (rand() % 100 < 3)
            AnimRandomFrame(&npc->body);
    }
    else
    {
        AnimResume(&npc->leg);
        AnimResume(&npc->body);
    }
    AnimApplyToDrawable(&npc->leg, &ENTITY_ARRAY[*npc->entity].drawables[0], 1.5f);
    AnimApplyToDrawable(&npc->body, &ENTITY_ARRAY[*npc->entity].drawables[1], 1.5f);
}