#ifndef ANIM_H
#define ANIM_H

#include "Camera.h"

#define MAX_FRAMES 25

typedef enum AnimType
{
    AN_PlayerWalk,
    AN_Count
} AnimType;

typedef struct Anim
{
    Drawable *active;
    Drawable frames[MAX_FRAMES];
    int currentFrame;
    int nFrames;
    float delta;
    float currDelta;
    SDL_bool isPaused;
} Anim;

Anim AnimCreate(AnimType animType, float delta);

void AnimUpdate(Anim *anim, float dt);
void AnimDraw(Anim *anim, Camera *camera);

void AnimPause(Anim *anim);
void AnimResume(Anim *anim);

#endif