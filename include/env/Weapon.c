#include "Weapon.h"

#include "Entity.h"

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
        Vec2 itemFalloff = Vec2MulL(unitPlayerToMouse, item->Stats.falloff);
        mousePos.x = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.x;
        mousePos.y = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.y;

        SDL_Point point;
        point.x = ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2);
        point.y = ENTITY_ARRAY[*index].drawables[0].dst.y + (ENTITY_ARRAY[*index].drawables[0].dst.h / 2);

        DrawLineOnCanvas(renderer, point.x - cameraPos.x, point.y - cameraPos.y, mousePos.x - cameraPos.x, mousePos.y - cameraPos.y);

        // push back
        ENTITY_ARRAY[*index].Force.x -= itemFalloff.x;
        ENTITY_ARRAY[*index].Force.y -= itemFalloff.y;
        //bullet(index, mousePos, point, item, unitPlayerToMouse);
        for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
        {
            if (i != *index)
                RayScan(i, mousePos, point, item, itemFalloff);
        }
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
