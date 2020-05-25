#include "Weapon.h"

#include "ClientManager.h"

void WeaponUpdate(Item *item)
{
    // counts the cooldown
    item->Stats.currentTime -= ClockGetDeltaTimeMS();
    item->Stats.currentTime = (item->Stats.currentTime <= 0.0f) ? -1 : item->Stats.currentTime;
}

const void WeaponMultiplayerHandler(int *index, Vec2 *direction, WeaponStats *Stats)
{
    Entity *entity = &ENTITY_ARRAY[*index];
    if (entity->type == ET_Player)
    {
        HitData hitData = {entity->id, Stats->Damage};
        ClientTCPSend(PT_PlayerHit, &hitData, sizeof(HitData));
    }
    else if (entity->type == ET_NPC)
    {
        entity->health -= Stats->Damage;
    }

    entity->Force.x += direction->x * (float)(Stats->falloff / 10);
    entity->Force.y += direction->y * (float)(Stats->falloff / 10);
}

const void WeaponSingleplayerHandler(int *index, Vec2 *direction, WeaponStats *Stats)
{
    ENTITY_ARRAY[*index].health -= Stats->Damage;
    ENTITY_ARRAY[*index].Force.x += direction->x * (float)(Stats->falloff / 10);
    ENTITY_ARRAY[*index].Force.y += direction->y * (float)(Stats->falloff / 10);
}

void WeaponRayScan(EntityIndexP source, Vec2 *direction, WeaponStats *stats, void *pointerToFunc)
{
    void (*Func)(int *, Vec2 *, WeaponStats *);
    Func = pointerToFunc;
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
    Vec2 playerCenter = RectMid(ENTITY_ARRAY[*source].drawables[0].dst);
    Vec2 range = Vec2MulL(*direction, stats->falloff);
    Vec2 rangeWithOffset = Vec2Add(playerCenter, range);

    CameraDrawLine(playerCenter.x, playerCenter.y, rangeWithOffset.x, rangeWithOffset.y, (SDL_Color){255, 50, 50, 150});
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isCollider == SDL_TRUE && *source != i) // take aways this if statment for fun time with map
        {
            tmpPosX = playerCenter.x;
            tmpPosY = playerCenter.y;
            tmpPointX = rangeWithOffset.x;
            tmpPointY = rangeWithOffset.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                Func(&i, direction, stats);
#ifdef Debug_Weapon_GetHitInfo
                log_info("entity index = %d, id = %d, health = %d\n", i, ENTITY_ARRAY[i].id, ENTITY_ARRAY[i].health);
#endif
            }
        }
    }
}

void WeaponRayScanClosest(EntityIndexP source, Vec2 *direction, WeaponStats *stats, void *pointerToFunc)
{
    void (*Func)(int *, Vec2 *, WeaponStats *);
    Func = pointerToFunc;
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY, closestEntity = 0, endPointX, endPointY;
    float closestLength = 99999.0f, testLength; // there isen't a weapon that kan shoot longer than 99999 units, yet....
    Vec2 playerCenter = RectMid(ENTITY_ARRAY[*source].drawables[0].dst);
    Vec2 range = Vec2MulL(*direction, stats->falloff);
    Vec2 rangeWithOffset = Vec2Add(playerCenter, range);
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isCollider == 1 && *source != i)
        {
            tmpPointX = playerCenter.x;
            tmpPointY = playerCenter.y;
            tmpPosX = rangeWithOffset.x;
            tmpPosY = rangeWithOffset.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                tmpPosX = ENTITY_ARRAY[i].drawables[0].dst.x - tmpPointX;
                tmpPosY = ENTITY_ARRAY[i].drawables[0].dst.y - tmpPointY;
                testLength = sqrt(pow(tmpPosX, 2) + pow(tmpPosX, 2));
                if (testLength < closestLength)
                {
                    closestEntity = i;
                    endPointX = tmpPointX;
                    endPointY = tmpPointY;
                    closestLength = testLength;
                }
            }
        }
    }
    if (closestEntity <= 0)
    {
        ShootData shootData = {playerCenter, rangeWithOffset, Vec2Unit(Vec2Sub(rangeWithOffset, playerCenter))};
        ClientManagerAddShootingLine(shootData);
        ClientTCPSend(PT_PlayerShoot, &shootData, sizeof(shootData));
    }
    else
    {
        Func(&closestEntity, direction, stats);
        Vec2 end = Vec2Create(endPointX, endPointY);
        ShootData shootData = {playerCenter, end, Vec2Unit(Vec2Sub(end, playerCenter))};
        ClientManagerAddShootingLine(shootData);
        ClientTCPSend(PT_PlayerShoot, &shootData, sizeof(shootData));
#ifdef Debug_Weapon_GetHitInfo
        // "Entity" finns inte
        // log_debug("closest entity[%d] health=%d ", closestEntity, entity->health);
#endif
    }
}

void WeaponRayMarchingTest(EntityIndexP source, Vec2 *direction, WeaponStats *stats, void *pointerToFunc)
{
    Vec2 RayOrgin = RectMid(ENTITY_ARRAY[*source].drawables[0].dst);
    Vec2 point;
    float StepSize = 0.0f, DirectionScale = 0.0f;
    Vec2 previousPoint = RayOrgin;
    float reach = stats->falloff;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        point = Vec2Add(previousPoint, Vec2MulL(*direction, StepSize));
        DirectionScale = WeaponMaxDistanceBeforeColision(point, source, reach);
        StepSize += DirectionScale;
        reach -= fabsf(DirectionScale);

        CameraDrawLine(RayOrgin.x, RayOrgin.y, point.x, point.y, (SDL_Color){255, 50, 50, 150});
        // if testLineWithEntitys is true you hit something, if reach is >0 you hit the max distance, stepsize if the step is too big, DirectionScale you hit something unhitable
        if (WeaponTestLineWithEntities(previousPoint, point, source, &stats->Damage) || reach <= 0 || StepSize > UNINHABITABLE || DirectionScale <= 0)
            break;
        previousPoint = point;
    }
}

float WeaponMaxDistanceBeforeColision(Vec2 point, EntityIndexP index, float maxDistance)
{
    Vec2 vector;
    float VectorLength;
    float closestObject = maxDistance;
    for (int i = 0; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (*index != i || ENTITY_ARRAY[i].isCollider == 1)
        {
            vector = Vec2Sub(RectMid(ENTITY_ARRAY[i].drawables[0].dst), point);
            VectorLength = Vec2Len(vector);
            closestObject = (closestObject > VectorLength && VectorLength != 0) ? VectorLength : closestObject;
        }
    }
    return closestObject = (closestObject > maxDistance) ? maxDistance : closestObject;
}

SDL_bool WeaponTestLineWithEntities(Vec2 start, Vec2 end, EntityIndexP ignoreEntity, int *damage)
{
    for (int i = 0; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isCollider == 1 && *ignoreEntity != i) // take aways this if statment for fun time with map
        {
            int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
            tmpPosX = start.x;
            tmpPosY = start.y;
            tmpPointX = end.x;
            tmpPointY = end.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                ENTITY_ARRAY[i].health -= *damage;
#ifdef Debug_Weapon_GetHitInfo
                log_info("entity index = %d, id = %d, health = %d, Weight = %f isCollider = %d\n", i, ENTITY_ARRAY[i].id, ENTITY_ARRAY[i].health, ENTITY_ARRAY[i].mass, ENTITY_ARRAY[i].isCollider);
#endif
                return 1;
            }
        }
    }
    return 0;
}