#include "Weapon.h"
#include "Entity.h"

void shoot(Player *player, Camera *camera, Entity e[], Item item)
{
    int pos_x = 0;
    int pos_y = 0;

    SDL_GetMouseState(&pos_x, &pos_y);
    Vec2 mousePos = Vec2Create((float)pos_x, (float)pos_y);
    // Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Create((float)player->entity.drawable.dst.x, (float)player->entity.drawable.dst.y);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);
    Vec2 unitPlayerToMouse = Vec2Unit(playerToMouse);
    Vec2 itemFalloff = Vec2MulL(unitPlayerToMouse, item.Stats.falloff);
    pos_x = (int)(player->entity.drawable.dst.x + (player->entity.drawable.dst.w / 2)) + (int)itemFalloff.x;
    pos_y = (int)(player->entity.drawable.dst.x + (player->entity.drawable.dst.w / 2)) + (int)itemFalloff.y;

    SDL_Point point;
    point.x = player->entity.drawable.dst.x + (player->entity.drawable.dst.w / 2);
    point.y = player->entity.drawable.dst.y + (player->entity.drawable.dst.h / 2);
    int tmpPosX, tmpPosY, tmpPointX, tmpPointY;
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
    srand(time(0));
    for (int i = 0; i < 3; i++)
    {
        tmpPosX = pos_x;
        tmpPosY = pos_y;
        tmpPointX = point.x + (rand() % 20 - 10) / item.Stats.accuracy;
        tmpPointY = point.y + (rand() % 20 - 10) / item.Stats.accuracy;
        if (SDL_IntersectRectAndLine(&e[i].drawable.dst, &tmpPointX, &tmpPointY, &tmpPosX, &tmpPosY))
        { // reduce accuracy
            e[i].health -= item.Stats.Damage;
            log_info("entity %d: health = %d\n", i, e[i].health);
        }
    }
}