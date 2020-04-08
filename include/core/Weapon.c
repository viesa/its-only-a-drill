#include "Weapon.h"

WeaponStats WeaponCreate(Item *weaponItem)
{
    WeaponStats stats;
    switch (weaponItem->type)
    {
    case Pistol:
        stats.Damage = 90;
        stats.falloff = 50;
        stats.accuracy = 0.7f;
        stats.ammo = 12;
        stats.captivity = 12;
        break;
    case Sniper:
        stats.Damage = 90;
        stats.falloff = 600;
        stats.accuracy = 0.7f;
        stats.ammo = 12;
        stats.captivity = 12;
        break;
    case MechineGun:
        stats.Damage = 20;
        stats.falloff = 200;
        stats.accuracy = 0.7f;
        stats.ammo = 12;
        stats.captivity = 12;
        break;
    case ItemWoodenSword:
        stats.Damage = 100;
        stats.falloff = 50;
        stats.accuracy = 1.0f;
        stats.ammo = 12;
        stats.captivity = 12;
        break;
    case ItemMetalSword:
        stats.Damage = 100;
        stats.falloff = 50;
        stats.accuracy = 1.0f;
        stats.ammo = 12;
        stats.captivity = 12;
        break;

    default:
        stats.Damage = 0;
        stats.falloff = 0;
        stats.accuracy = 0.0f;
        stats.ammo = 0;
        stats.captivity = 0;
        break;
    }
    return stats;
}
// void UpdateWeapons(Weapon *weapons, Input *input, Entity *User, Camera *camera)
// {
//     if (InputIsKeyPressed(input, SDL_SCANCODE_G))
//     {
//         User->Force.x += 20;
//     }
// }
// void shoot()
// {
// }