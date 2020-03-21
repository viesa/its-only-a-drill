#ifndef RESOURCES_H
#define RESOURCES_H

#include "../include/C_Includes.h"

typedef struct C_Resources
{
    Drawable db[3000];
    Vec2 cameraFollow;
    Sound test;
    Item item;
    Entity entity;
} C_Resources;

#endif