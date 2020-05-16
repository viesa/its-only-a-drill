#ifndef ANIM_H
#define ANIM_H

#include "Camera.h"

#define MAX_FRAMES 25

typedef enum AnimType
{
    AN_PlayerLegs,
    AN_PlayerBody,
    AN_Count
} AnimType;

typedef enum AnimRepeatOption
{
    ANRO_NoRepeat,
    ANRO_RepeatFromStart,
    ANRO_RepeatFromEnd,
    ANRO_Count
} AnimRepeatOption;

typedef struct Anim
{
    Drawable *active;
    Drawable frames[MAX_FRAMES];
    AnimRepeatOption anro;
    int currentFrame;
    int nFrames;
    int startFrame;
    float delta;
    float currDelta;
    SDL_bool isPaused;
    SDL_bool isForward;
} Anim;

Anim AnimCreate(AnimType animType, AnimRepeatOption anro, SpriteSheet spriteSheet, int startFrame, float delta);

void AnimUpdate(Anim *anim, float dt);
void AnimDraw(Anim *anim);

void AnimPause(Anim *anim);
void AnimResume(Anim *anim);

void AnimStop(Anim *anim);

void AnimNextFrame(Anim *anim);
void AnimRandomFrame(Anim *anim);

void AnimApplyToDrawable(Anim *anim, Drawable *drawable, float scale);

void AnimChangeSpriteSheet(Anim *anim, SpriteSheet spriteSheet);

#endif