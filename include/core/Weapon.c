#include "Weapon.h"
#include "Entity.h"

void shoot(Player *player, Camera *camera, Entity e[], Item item)
{
    printf("function ran\n");
    int pos_x = 0;
    int pos_y = 0;

    SDL_GetMouseState(&pos_x, &pos_y);
    Vec2 mousePos = Vec2Create((float)pos_x, (float)pos_y);
    // Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Create((float)player->entity.drawable.dst.x, (float)player->entity.drawable.dst.y);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);
    Vec2 unitPlayerToMouse = Vec2Unit(playerToMouse);
    Vec2 itemFalloff = Vec2MulL(player->forward, item.Stats.falloff);

    SDL_Point point;
    point.x = itemFalloff.x;
    point.y = itemFalloff.y;

    //SDL_bool pointInArea;

    for (int i = 0; i < 3; i++)
    {

        if (SDL_PointInRect(&point, &e[i].drawable.dst))
        //if(SDL_IntersectRectAndLine(&e[i].drawable.dst, (int)playerPos.x, (int)playerPos.y, (int)point.x, (int)point.y))
        { // reduce accuracy
            printf("hit\n");
            e[i].health -= item.Stats.Damage;
        }
    }
}