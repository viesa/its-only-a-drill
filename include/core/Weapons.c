#include "Weapons.h"

Weapon *CreateWeapon(int damage, float accuracy, int weaponType)
{
    Weapon *weapon = (Weapon *)SDL_malloc(sizeof(Weapon));
    weapon->Damage = damage;
    weapon->accuracy = accuracy;
    switch (weaponType)
    {
    case Sniper:
        weapon->falloff = 500;
        break;
    case Pistol:
        weapon->falloff = 100;
        break;
    case MachineGun:
        weapon->falloff = 250;
        break;

    default:
        weapon->falloff = 0;
        break;
    }
    return weapon;
}
void UpdateWeapons(Weapon *weapons, Input *input, Entity *User, Camera *camera)
{
    if (InputIsKeyPressed(input, SDL_SCANCODE_G))
    {
        User->Force.x += 20;
    }
}
void shoot()
{
}
void DestroyWeapons(Weapon *weapon)
{
    SDL_free(weapon);
}