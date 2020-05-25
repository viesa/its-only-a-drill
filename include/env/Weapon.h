#ifndef WEAPON_H
#define WEAPON_H

#include "Dependencies.h"
#include "Library.h"
#include "Input.h"
#include "Items.h"
#include "EntityManager.h"

#define UNINHABITABLE 500
#define MAX_STEPS 10

typedef struct HitData
{
    int id;
    int damage;
} HitData;

typedef struct ShootData
{
    Vec2 start;
    Vec2 end;
    Vec2 dir;
} ShootData;

// updates weapon logik thats needed efter frame like cooldown
void WeaponUpdate(Item *item);

//how to handle output data
const void WeaponMultiplayerHandler(int *index, Vec2 *direction, WeaponStats *Stats);
const void WeaponSingleplayerHandler(int *index, Vec2 *direction, WeaponStats *Stats);

//**************[Diffrent shooting types]**************
// casts a ray test what collides with it (sends data to server)
void WeaponRayScan(EntityIndexP source, Vec2 *direction, WeaponStats *stats, void *pointerToFunc);
// takes steps intil collision
void WeaponRayScanClosest(EntityIndexP source, Vec2 *direction, WeaponStats *stats, void *pointerToFunc);
/// gives the maximum distance before collision
///\param index: index == zero to not test with anything
/// otherwise ignore index entity
float WeaponMaxDistanceBeforeColision(Vec2 point, EntityIndexP index, float maxDistance);
/// returns 1 if hit and changes data, returns 0 if failed
SDL_bool WeaponTestLineWithEntities(Vec2 start, Vec2 end, EntityIndexP ignoreEntity, int *damage);
// casts a ray and changes data on closest hit
void RayScanClosest(EntityIndexP source, Vec2 *direction, WeaponStats *stat, void *pointerToFunc);

#endif