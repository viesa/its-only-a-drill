#ifndef Weapon_H
#define Weapon_H
#include <stdio.h>
#include "Input.h"
#include "Entity.h"
#include "Camera.h"

typedef enum WeaponType
{
    Sniper,
    Pistol,
    MachineGun,
    Wut
} WeaponType;

typedef struct Weapon
{
    int WeaponType;
    int Damage;
    int falloff;
    float accuracy;
    int ammo;
    int captivity;
} Weapon;

///<skapar ett weapon so it exist >
///\param <beskriving av parameter som är av värde>
Weapon WeaponCreate(int damage, float accuracy, int weaponType);
void WeaponUpdate(Weapon *Weapon, Input *Input, Entity *User);
void shoot();
void WeaponDestroy(Weapon *weapon);
#endif