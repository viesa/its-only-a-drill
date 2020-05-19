#include "NPC.h"

struct MovePattern
{
    Vec2 points[20];
    int nPoints;
};

MovePattern MovePatternCreate()
{
    MovePattern pattern;
    pattern.points[0] = Vec2Create(-50.0f, -50.0f);
    pattern.points[1] = Vec2Create(0.0f, -71.0f);
    pattern.points[2] = Vec2Create(50.0f, -50.0f);
    pattern.points[3] = Vec2Create(71.0f, 0.0f);
    pattern.points[4] = Vec2Create(50.0f, 50.0f);
    pattern.points[5] = Vec2Create(0.0f, 71.0f);
    pattern.points[6] = Vec2Create(-50.0f, 50.0f);
    pattern.points[7] = Vec2Create(-71.0f, 0.0f);
    pattern.points[8] = Vec2Create(-50.0f, -50.0f);
    pattern.points[9] = Vec2Create(0.0f, 0.0f);
    pattern.points[10] = Vec2Create(-50.0f, -50.0f);
    pattern.points[11] = Vec2Create(50.0f, 50.0f);
    pattern.nPoints = 12;
    return pattern;
}

struct NPC
{
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
    float radius;
};

NPC *NPCCreate(Vec2 pos)
{
    NPC *npc = MALLOC(NPC);
    ALLOC_ERROR_CHECK(npc);
    npc->entity = EntityManagerAdd(ET_Player, pos);
    npc->state = NPC_Neutral;
    npc->spriteSheet = SS_Character_ChernobylWorker;
    npc->leg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, npc->spriteSheet, 4, 0.05f);
    npc->body = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, npc->spriteSheet, 4, 0.05f);
    npc->forward = Vec2Create(1.0f, 0.0f);
    npc->inventory = InventoryCreate();
    npc->desiredPos = pos;
    npc->movePatternIndex = 0;
    npc->movePattern = MovePatternCreate();
    npc->movementSpeed = 500.0f;
    npc->aggravationRadius = 150.0f;
    npc->radius = 10.0f;
    return npc;
}
void NPCDestroy(NPC *npc)
{
    FREE(npc);
}

void NPCUpdate(NPC *npc)
{
    NPCUpdateBehavior(npc);
    if (npc->state != NPC_Dead)
        NPCUpdateAnimation(npc);
    if (npc->state == NPC_Fight)
        npc->forward = Vec2Unit(Vec2Sub(PlayerGetEntity()->position, ENTITY_ARRAY[*npc->entity].position));
    float vecAngle = toDegrees(Vec2Ang(Vec2Create(1.0f, 0.0f), npc->forward));
    float degrees = npc->forward.y > 0.0f ? vecAngle : 360 - vecAngle;
    EntityRotateAll(npc->entity, degrees);
}
void NPCDraw(NPC *npc)
{
    EntityDrawIndex(npc->entity);
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
    entity->isCollider = SDL_FALSE;
    npc->state = NPC_Dead;
    AnimStop(&npc->leg);
    AnimStop(&npc->body);
}

void NPCUpdateBehavior(NPC *npc)
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
        NPCUpdateBehaviorFight(npc);
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

    // circle around desired position
    Circle tmp;
    tmp.Position = RectMid(entity->drawables[0].dst);
    tmp.Radius = npc->radius;
    if (CollisionCircleWithPoint(&npc->desiredPos,&tmp))
    {
        
        npc->desiredPos = Vec2Add( RectMid(entity->drawables[0].dst), npc->movePattern.points[npc->movePatternIndex]);
        npc->movePatternIndex = (npc->movePatternIndex < 10 || npc->movePatternIndex == 11) ? 10 : npc->movePatternIndex + 1;
    }
}
void NPCUpdateBehaviorFight(NPC *npc)
{
    NPCShoot(npc);
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    Entity *pEntity = PlayerGetEntity();
    Vec2 enemyToPlayer = Vec2Sub(pEntity->position, entity->position);    
    if (PlayerGetEntity()->health < 0)
    {
        npc->state = NPC_Neutral;
    }
    if (Vec2Len(enemyToPlayer) > npc->aggravationRadius)
    {
        npc->state = NPC_Aggressive;
    }
}
void NPCUpdateBehaviorAggressive(NPC *npc)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    NPCMoveTo(npc, npc->desiredPos);

    // circle around desired position
    Circle tmp;
    tmp.Position = RectMid(entity->drawables[0].dst);
    tmp.Radius = npc->radius;
    if (CollisionCircleWithPoint(&npc->desiredPos,&tmp))
    {
        npc->desiredPos = Vec2Add( RectMid(entity->drawables[0].dst), npc->movePattern.points[npc->movePatternIndex]);
        npc->movePatternIndex = npc->movePatternIndex >= 7 ? 0 : npc->movePatternIndex + 1;
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
    npc->forward = Vec2Unit(entity->Force);
}

void NPCSwitchBehaviorState(NPC *npc)
{
    Entity *entity = &ENTITY_ARRAY[*npc->entity];
    Entity *pEntity = PlayerGetEntity();
    if (entity->health <= 0 && npc->state != NPC_Dead)
    {
        NPCKill(npc);
    }
    else if (npc->state != NPC_Dead)
    {
        Vec2 enemyToPlayer = Vec2Sub(pEntity->position, entity->position);
        //npc->state = NPC_Neutral;
        if (entity->health < 100 && npc->state != NPC_Fight)
        {
            npc->state = NPC_Aggressive;
        }
        if (Vec2Len(enemyToPlayer) <= npc->aggravationRadius)
        {
            npc->state = NPC_Fight;
        }
    }
}

void NPCShoot(NPC *npc)
{
    Item *item = &npc->inventory.contents[npc->inventory.top - 1];
    item->Stats.currentTime -= ClockGetDeltaTimeMS();
    if (item->Stats.currentTime <= 0)
    {
        Entity *entity = &ENTITY_ARRAY[*npc->entity];
        Entity *pEntity = PlayerGetEntity();

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

        CameraDrawLine(point.x, point.y, makeDestination.x, makeDestination.y, (SDL_Color){255, 50, 50, 150});
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

    if (Vec2LenSq(ENTITY_ARRAY[*npc->entity].Velocity) < 0.1f || Vec2LenSq(ENTITY_ARRAY[*npc->entity].Force) < 0.1f || npc->state == NPC_Fight)
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