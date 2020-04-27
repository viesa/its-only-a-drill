#include "Weapon.h"
#include "Entity.h"

void playerShoot(EntityIndexP index, Camera *camera, Input *input, Item item)
{
    Vec2 mousePos = InputLastMousePos(input);
    Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Sub(RectMid(ENTITY_ARRAY[*index].drawables[0].dst), cameraPos);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);
    Vec2 unitPlayerToMouse = Vec2Unit(playerToMouse);
    Vec2 itemFalloff = Vec2MulL(unitPlayerToMouse, item.Stats.falloff);
    mousePos.x = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.x;
    mousePos.y = (float)(ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2)) + itemFalloff.y;

    SDL_Point point;
    point.x = ENTITY_ARRAY[*index].drawables[0].dst.x + (ENTITY_ARRAY[*index].drawables[0].dst.w / 2);
    point.y = ENTITY_ARRAY[*index].drawables[0].dst.y + (ENTITY_ARRAY[*index].drawables[0].dst.h / 2);

    // push back
    ENTITY_ARRAY[*index].Force.x -= itemFalloff.x;
    ENTITY_ARRAY[*index].Force.y -= itemFalloff.y;
#ifdef ENTITY_DEBUG
    printf("line pos:\n");
    printf("X1: %d\n", point.x);
    printf("Y1: %d\n", point.y);
    printf("X2: %f\n", mousePos.x);
    printf("Y2: %f\n", mousePos.y);
    printf("The Vector:\n");
    printf("X: %f\n", playerToMouse.x);
    printf("Y: %f\n", playerToMouse.y);
#endif
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (i != *index)
            RayScan(i, mousePos, point, item, itemFalloff);
    }
}

void entityShoot(int index, Vec2 Desierdpoint, Item item)
{
    Vec2 entityToPoint = Vec2Sub(Desierdpoint, RectMid(ENTITY_ARRAY[index].drawables[0].dst));
    Vec2 unit = Vec2Unit(entityToPoint);
    Vec2 itemFalloff = Vec2MulL(unit, item.Stats.falloff);
    Vec2 makeDestination;
    makeDestination.x = (float)(ENTITY_ARRAY[index].drawables[0].dst.x + (ENTITY_ARRAY[index].drawables[0].dst.w / 2)) + itemFalloff.x;
    makeDestination.y = (float)(ENTITY_ARRAY[index].drawables[0].dst.x + (ENTITY_ARRAY[index].drawables[0].dst.w / 2)) + itemFalloff.y;

    SDL_Point point;
    point.x = ENTITY_ARRAY[index].drawables[0].dst.x + (ENTITY_ARRAY[index].drawables[0].dst.w / 2);
    point.y = ENTITY_ARRAY[index].drawables[0].dst.y + (ENTITY_ARRAY[index].drawables[0].dst.h / 2);

    // push back
    ENTITY_ARRAY[index].Force.x -= itemFalloff.x;
    ENTITY_ARRAY[index].Force.y -= itemFalloff.y;
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (i != index)
            RayScan(i, makeDestination, point, item, itemFalloff);
    }
}

void RayScan(int index, Vec2 Destination, SDL_Point point, Item item, Vec2 ForceDir)
{
    if (ENTITY_ARRAY[index].isCollider == SDL_TRUE) // take aways this if statment for fun time with map
    {
        int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
        tmpPosX = Destination.x;
        tmpPosY = Destination.y;
        tmpPointX = point.x + (rand() % 20 - 10) / item.Stats.accuracy;
        tmpPointY = point.y + (rand() % 20 - 10) / item.Stats.accuracy;
        if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[index].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
        { // reduce accuracy
            ENTITY_ARRAY[index].health -= item.Stats.Damage;
            ENTITY_ARRAY[index].Force.x += ForceDir.x;
            ENTITY_ARRAY[index].Force.y += ForceDir.y;
            log_info("entity %d: health = %d\n", index, ENTITY_ARRAY[index].health);
        }
    }
}