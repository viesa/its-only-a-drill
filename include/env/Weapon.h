#ifndef Weapon_H
#define Weapon_H

#include "Dependencies.h"
#include "Library.h"
#include "Input.h"
#include "Items.h"
#include "Player.h"
#include "../net/Packager.h"
#include "../net/Client.h"

typedef struct data
{
    int id;
    int damage;
} Data;

// playershoot räknar ut unit vektor och senare gör den längre till falloff
void playerShoot(EntityIndexP index, Camera *camera, Item *item, SDL_Renderer *renderer);
// updateras varje frame så måste skrivas anurlonda
void entityShoot(int *index, Vec2 Desierdpoint, Item *item, SDL_Renderer *renderer, Camera *camera);

//diffrent types of shooting
// skickar datan över nätet
void RayScan(int index, Vec2 makeDestination, SDL_Point point, Item *item, Vec2 ForceDir);
// hanterar datan i funktionen
void RayScanSingelplayer(int index, Vec2 makeDestination, SDL_Point point, Item *item, Vec2 ForceDir);
// ritar linjen hur skotet åkte
void DrawLineOnCanvas(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
// skapar en projektil *unsused/not updated
// void bullet(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 Direction);

void weaponUpdate(Item *item);

#endif