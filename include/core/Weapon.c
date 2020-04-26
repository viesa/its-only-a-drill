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
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY;

    // push back
    ENTITY_ARRAY[*index].Force.x -= itemFalloff.x;
    ENTITY_ARRAY[*index].Force.y -= itemFalloff.y;
#ifdef DegBug
    printf("line pos:\n");
    printf("X1: %d\n", point.x);
    printf("Y1: %d\n", point.y);
    printf("X2: %d\n", pos_x);
    printf("Y2: %d\n", pos_y);
    printf("The Vector:\n");
    printf("X: %f\n", playerToMouse.x);
    printf("Y: %f\n", playerToMouse.y);
#endif
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isCollider == SDL_TRUE) // take aways this if statment for fun time with map
        {
            tmpPosX = mousePos.x;
            tmpPosY = mousePos.y;
            tmpPointX = point.x + (rand() % 20 - 10) / item.Stats.accuracy;
            tmpPointY = point.y + (rand() % 20 - 10) / item.Stats.accuracy;
            if (SDL_IntersectRectAndLine(&ENTITY_ARRAY[i].drawables[0].dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
            { // reduce accuracy
                ENTITY_ARRAY[i].health -= item.Stats.Damage;
                ENTITY_ARRAY[i].Force.x += itemFalloff.x;
                ENTITY_ARRAY[i].Force.y += itemFalloff.y;
                log_info("entity %d: health = %d\n", i, ENTITY_ARRAY[i].health);
            }
        }
    }
}