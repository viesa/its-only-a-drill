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
void playerShoot(Graphics *gfx, EntityIndexP index, Camera *camera, Item *item)
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
        Vec2 itemFalloff = Vec2MulL(unitPlayerToMouse, item->Stats.falloff);
        mousePos.x = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.x;
        mousePos.y = (float)(ENTITY_ARRAY[*index].drawables[0].dst.y + (ENTITY_ARRAY[*index].drawables[0].dst.h / 2)) + itemFalloff.y;

        SDL_Point point;
        point.x = ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2);
        point.y = ENTITY_ARRAY[*index].drawables[0].dst.y + (ENTITY_ARRAY[*index].drawables[0].dst.h / 2);

        // push back
        ENTITY_ARRAY[*index].Force.x -= itemFalloff.x;
        ENTITY_ARRAY[*index].Force.y -= itemFalloff.y;
        //bullet(index, mousePos, point, item, unitPlayerToMouse);

        rayMarchingTest(gfx, index, &unitPlayerToMouse, camera, &item->Stats);
    }
}

void entityShoot(Graphics *gfx, int *index, Vec2 Desierdpoint, Item *item, Camera *camera)
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
        GraphicsDrawLine(gfx, point.x - cameraPos.x, point.y - cameraPos.y, makeDestination.x - cameraPos.x, makeDestination.y - cameraPos.y, (SDL_Color){255, 50, 50, 150});
        // push back
        ENTITY_ARRAY[*index].Force.x -= itemFalloff.x;
        ENTITY_ARRAY[*index].Force.y -= itemFalloff.y;
        for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
        {
            if (i != *index && ENTITY_ARRAY[i].isNPC == 0)
                RayScanSingelplayer(i, makeDestination, point, item, itemFalloff);
        }
    }
}

void RayScan(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 ForceDir)
{
    if (ENTITY_ARRAY[index].isCollider == SDL_TRUE) // take aways this if statment for fun time with map
    {
        int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
        tmpPosX = Destination.x;
        tmpPosY = Destination.y;
        tmpPointX = point.x + (rand() % 20 - 10) / item->Stats.accuracy;
        tmpPointY = point.y + (rand() % 20 - 10) / item->Stats.accuracy;
        if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[index].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
        { // reduce accuracy
            //ENTITY_ARRAY[index].health -= item->Stats.Damage;
            Data sendData;
            sendData.id = ENTITY_ARRAY[index].id;
            sendData.damage = item->Stats.Damage;

            ClientTCPSend(15, &sendData, sizeof(int) * 2);

            ENTITY_ARRAY[index].Force.x += ForceDir.x;
            ENTITY_ARRAY[index].Force.y += ForceDir.y;
            log_debug("Sent packet entity = %d damage = %d", sendData.id, sendData.damage);
        }
    }
}
void RayScanSingelplayer(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 ForceDir)
{
    if (ENTITY_ARRAY[index].isCollider == SDL_TRUE) // take aways this if statment for fun time with map
    {
        int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
        tmpPosX = Destination.x;
        tmpPosY = Destination.y;
        tmpPointX = point.x + (rand() % 20 - 10) / item->Stats.accuracy;
        tmpPointY = point.y + (rand() % 20 - 10) / item->Stats.accuracy;
        if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[index].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
        { // reduce accuracy
            ENTITY_ARRAY[index].health -= item->Stats.Damage;
            ENTITY_ARRAY[index].Force.x += ForceDir.x;
            ENTITY_ARRAY[index].Force.y += ForceDir.y;
            log_info("entity index = %d, id = %d, health = %d\n", index, ENTITY_ARRAY[index].id, ENTITY_ARRAY[index].health);
        }
    }
}

void rayMarchingTest(Graphics *gfx, EntityIndexP index, Vec2 *direction, Camera *camera, WeaponStats *stats)
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

        GraphicsDrawLine(gfx, RayOrgin.x - cameraPos.x, RayOrgin.y - cameraPos.y, point.x - cameraPos.x, point.y - cameraPos.y, (SDL_Color){255, 50, 50, 150});
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
        if (ENTITY_ARRAY[i].isCollider == SDL_TRUE && *ignoreEntity != i) // take aways this if statment for fun time with map
        {
            int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
            tmpPosX = start.x;
            tmpPosY = start.y;
            tmpPointX = end.x;
            tmpPointY = end.y;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                ENTITY_ARRAY[i].health -= *damage;
                log_info("entity index = %d, id = %d, health = %d\n", i, ENTITY_ARRAY[i].id, ENTITY_ARRAY[i].health);
                return 1;
            }
        }
    }
    return 0;
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
