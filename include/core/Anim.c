#include "Anim.h"

Anim AnimCreate(AnimType animType, float delta)
{
    Anim anim;
    anim.delta = delta;
    anim.currDelta = 0.0f;
    anim.currentFrame = 0;
    anim.isPaused = SDL_FALSE;
    anim.active = &anim.frames[0];
    switch (animType)
    {
    case AN_PlayerWalk:
        anim.nFrames = 4;
        anim.frames[0] = DrawableCreate((SDL_Rect){592, 398, 14, 21}, (SDL_Rect){0, 0, 14, 21}, SS_Characters_Props);
        anim.frames[1] = DrawableCreate((SDL_Rect){625, 398, 14, 21}, (SDL_Rect){0, 0, 14, 21}, SS_Characters_Props);
        anim.frames[2] = DrawableCreate((SDL_Rect){658, 398, 14, 21}, (SDL_Rect){0, 0, 14, 21}, SS_Characters_Props);
        anim.frames[3] = DrawableCreate((SDL_Rect){691, 398, 14, 22}, (SDL_Rect){0, 0, 14, 22}, SS_Characters_Props);
        break;
    default:
        anim.nFrames = -1;
        break;
    }

    return anim;
}

void AnimUpdate(Anim *anim, float dt)
{
    if (!anim->isPaused || anim->nFrames != -1)
    {
        anim->currDelta += dt;
        if (anim->currDelta >= anim->delta)
        {
            anim->currDelta = 0.0f;
            anim->currentFrame++;
            if (anim->currentFrame == anim->nFrames)
                anim->currentFrame = 0;
        }

        anim->active = &anim->frames[anim->currentFrame];
    }
}

void AnimDraw(Anim *anim, Camera *camera)
{
    if (anim->active && anim->nFrames != -1)
    {
        CameraDraw(camera, *anim->active);
    }
}

void AnimPause(Anim *anim)
{
    anim->isPaused = SDL_TRUE;
}

void AnimResume(Anim *anim)
{
    anim->isPaused = SDL_FALSE;
}