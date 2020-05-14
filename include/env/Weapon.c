#include "Weapon.h"

#include "Entity.h"

#define unhitable 500
#define max_steps 10

void weaponUpdate(Item *item)
{
    // counts the cooldown
    item->Stats.currentTime -= ClockGetDeltaTimeMS();

    // reserved for bullet update
}
void playerShoot(EntityIndexP index, Camera *camera, Item *item, SDL_Renderer *renderer)
{
#ifdef WEAPON_DEBIG
    log_debug("current cooldown %f", item->Stats.currentTime);
#endif
    if (item->Stats.currentTime <= 0)
    {
        item->Stats.currentTime = item->Stats.cooldownMS;
        Vec2 mousePos = InputLastMousePos();
        Vec2 cameraPos = CameraGetPos(camera);
        Vec2 playerPos = Vec2Sub(RectMid(ENTITY_ARRAY[*index].drawables[0].dst), cameraPos);

        Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);
        Vec2 unitPlayerToMouse = Vec2Unit(playerToMouse);

        // push back
        ENTITY_ARRAY[*index].Force.x -= item->Stats.falloff;
        ENTITY_ARRAY[*index].Force.y -= item->Stats.falloff;
        //bullet(index, mousePos, point, item, unitPlayerToMouse);

        // change to closestray function THIS IS A MEM NOTE DON*T FOR GET IT if // if you find this send the following "error: 322756"
        RayScanClosest(index, &unitPlayerToMouse, camera, renderer, &item->Stats);
    }
}

void entityShoot(int *index, Vec2 Desierdpoint, Item *item, SDL_Renderer *renderer, Camera *camera)
{
    item->Stats.currentTime -= ClockGetDeltaTimeMS();
    if (item->Stats.currentTime <= 0)
    {
        item->Stats.currentTime = item->Stats.cooldownMS;
#ifdef WEAPON_DEBIG
        log_debug("Entity %d: SHOOT!", *index);
        log_debug("Entity %d: aim at X:%f Y:%f", *index, Desierdpoint.x, Desierdpoint.y);
#endif
        Vec2 entityToPoint = Vec2Sub(Desierdpoint, RectMid(ENTITY_ARRAY[*index].drawables[0].dst));
        Vec2 unit = Vec2Unit(entityToPoint);
        Vec2 itemFalloff = Vec2MulL(unit, item->Stats.falloff);
        Vec2 makeDestination;
        makeDestination.x = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.x;
        makeDestination.y = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.y;

        SDL_Point point;
        point.x = ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2);
        point.y = ENTITY_ARRAY[*index].drawables[0].dst.y + (ENTITY_ARRAY[*index].drawables[0].dst.h / 2);

        Vec2 cameraPos = CameraGetPos(camera);
        DrawLineOnCanvas(renderer, point.x - cameraPos.x, point.y - cameraPos.y, makeDestination.x - cameraPos.x, makeDestination.y - cameraPos.y);
        // push back
        ENTITY_ARRAY[*index].Force.x -= itemFalloff.x;
        ENTITY_ARRAY[*index].Force.y -= itemFalloff.y;
        for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
        {
            // if (i != *index && ENTITY_ARRAY[i].isNPC == 0)
            //     RayScanSingelplayer(i, makeDestination, point, item, itemFalloff);
        }
    }
}

void RayScan(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats)
{
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
    Vec2 playerCenter = RectMid(ENTITY_ARRAY[*index].drawables[0].dst);
    Vec2 range = Vec2MulL(*direction, stats->falloff);
    Vec2 rangeWithOffset = Vec2Add(playerCenter, range);
    Vec2 cameraPos = CameraGetPos(camera);

    DrawLineOnCanvas(renderer, playerCenter.x - cameraPos.x, playerCenter.y - cameraPos.y, rangeWithOffset.x - cameraPos.x, rangeWithOffset.y - cameraPos.y);
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isCollider == SDL_TRUE && *index != i) // take aways this if statment for fun time with map
        {
            tmpPosX = playerCenter.x;
            tmpPosY = playerCenter.y;
            tmpPointX = rangeWithOffset.x;
            tmpPointY = rangeWithOffset.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                Data sendData;
                sendData.id = ENTITY_ARRAY[i].id;
                sendData.damage = stats->Damage;

                ClientTCPSend(15, &sendData, sizeof(int) * 2);

                ENTITY_ARRAY[i].Force.x += direction->x * (float)(stats->falloff / 10);
                ENTITY_ARRAY[i].Force.y += direction->x * (float)(stats->falloff / 10);
#ifdef Debug_Weapon_GetHitInfo
                log_info("entity index = %d, id = %d, health = %d\n", i, ENTITY_ARRAY[i].id, ENTITY_ARRAY[i].health);
#endif
            }
        }
    }
}
void RayScanSingelplayer(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats)
{
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
    Vec2 playerCenter = RectMid(ENTITY_ARRAY[*index].drawables[0].dst);
    Vec2 range = Vec2MulL(*direction, stats->falloff);
    Vec2 rangeWithOffset = Vec2Add(playerCenter, range);
    Vec2 cameraPos = CameraGetPos(camera);

    DrawLineOnCanvas(renderer, playerCenter.x - cameraPos.x, playerCenter.y - cameraPos.y, rangeWithOffset.x - cameraPos.x, rangeWithOffset.y - cameraPos.y);
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isCollider == SDL_TRUE && *index != i) // take aways this if statment for fun time with map
        {
            tmpPosX = playerCenter.x;
            tmpPosY = playerCenter.y;
            tmpPointX = rangeWithOffset.x;
            tmpPointY = rangeWithOffset.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                ENTITY_ARRAY[i].health -= stats->Damage;
                ENTITY_ARRAY[i].Force.x += direction->x * (float)(stats->falloff / 10);
                ENTITY_ARRAY[i].Force.y += direction->x * (float)(stats->falloff / 10);
#ifdef Debug_Weapon_GetHitInfo
                log_info("entity index = %d, id = %d, health = %d\n", i, ENTITY_ARRAY[i].id, ENTITY_ARRAY[i].health);
#endif
            }
        }
    }
}

void RayScanClosest(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats)
{
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY, closestEntity = 0, endPointX, endPointY;
    float closestLenght = 99999.0f, testLenght; // there isen't a weapon that kan shoot longer than 99999 units, yet....
    Vec2 playerCenter = RectMid(ENTITY_ARRAY[*index].drawables[0].dst);
    Vec2 range = Vec2MulL(*direction, stats->falloff);
    Vec2 rangeWithOffset = Vec2AddL(playerCenter, range);
    Vec2 cameraPos = CameraGetPos(camera);
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (*index != i && ENTITY_ARRAY[i].isCollider == 1)
        {
            tmpPointX = playerCenter.x;
            tmpPointY = playerCenter.y;
            tmpPosX = rangeWithOffset.x;
            tmpPosY = rangeWithOffset.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                tmpPosX = ENTITY_ARRAY[i].drawables[0].dst.x - tmpPointX;
                tmpPosY = ENTITY_ARRAY[i].drawables[0].dst.y - tmpPointY;
                testLenght = sqrt(pow(tmpPosX, 2) + pow(tmpPosX, 2));
                if (testLenght < closestLenght)
                {
                    closestEntity = i;
                    endPointX = tmpPointX;
                    endPointY = tmpPointY;
                }
            }
        }
    }
    if (closestEntity <= 0)
    {
        DrawLineOnCanvas(renderer, (int)playerCenter.x - cameraPos.x, (int)playerCenter.y - cameraPos.y, (int)rangeWithOffset.x - cameraPos.x, (int)rangeWithOffset.y - cameraPos.y);
    }
    else
    {
        DrawLineOnCanvas(renderer, (int)playerCenter.x - cameraPos.x, (int)playerCenter.y - cameraPos.y, endPointX - cameraPos.x, endPointY - cameraPos.y);
        ENTITY_ARRAY[closestEntity].health -= stats->Damage;
        ENTITY_ARRAY[closestEntity].Force.x += direction->x * (float)(stats->falloff / 10);
        ENTITY_ARRAY[closestEntity].Force.y += direction->y * (float)(stats->falloff / 10);
#ifdef Debug_Weapon_GetHitInfo
        log_debug("closest entity[%d] health=%d ", closestEntity, ENTITY_ARRAY[closestEntity].health);
#endif
    }
}

void rayMarchingTest(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats)
{
    Vec2 RayOrgin = RectMid(ENTITY_ARRAY[*index].drawables[0].dst);
    Vec2 point;
    float StepSize = 0.0f, DirectionScale = 0.0f;
    Vec2 previousPoint = RayOrgin;
    Vec2 cameraPos = CameraGetPos(camera);
    float reach = stats->falloff;
    for (int i = 0; i < max_steps; i++)
    {
        point = Vec2Add(previousPoint, Vec2MulL(*direction, StepSize));
        DirectionScale = maxDistenBeforeColision(point, index, reach);
        StepSize += DirectionScale;
        reach -= fabsf(DirectionScale);
        DrawLineOnCanvas(renderer, RayOrgin.x - cameraPos.x, RayOrgin.y - cameraPos.y, point.x - cameraPos.x, point.y - cameraPos.y);
        // if testLineWithEntitys is true you hit something, if reach is >0 you hit the max distance, stepsize if the step is too big, DirectionScale you hit something unhitable
        if (testLineWithEntitys(previousPoint, point, index, &stats->Damage) || reach <= 0 || StepSize > unhitable || DirectionScale <= 0)
            break;
        previousPoint = point;
    }
}

float maxDistenBeforeColision(Vec2 point, EntityIndexP index, float maxDistance)
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

SDL_bool testLineWithEntitys(Vec2 start, Vec2 end, EntityIndexP ignoreEntity, int *damage)
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
                log_info("entity index = %d, id = %d, health = %d, Weight = %f isCollider = %d\n", i, ENTITY_ARRAY[i].id, ENTITY_ARRAY[i].health, ENTITY_ARRAY[i].mass, ENTITY_ARRAY[i].isCollider);
                return 1;
            }
        }
    }
    return 0;
}

void DrawLineOnCanvas(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 150);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
// void bullet(int index, Vec2 Destination, SDL_Point point, Item item, Vec2 Direction)
// {
//     // to create offset so you don't shoot your self
//     Vec2 creationWithOffset = Vec2MulL(Direction, 50);
//     EntityManagerAdd(ET_Bullet, creationWithOffset);
// }

void DectectIntersectionKeep()
{
}
