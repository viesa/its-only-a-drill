#include "Anim.h"

#include "Dependencies.h"

Anim AnimCreate(AnimType animType, AnimRepeatOption anro, SpriteSheet spriteSheet, int startFrame, float delta)
{
    Anim anim;
    anim.anro = anro;
    anim.delta = delta;
    anim.currDelta = 0.0f;
    anim.currentFrame = 0;
    anim.startFrame = startFrame;
    anim.isPaused = SDL_FALSE;
    anim.isForward = SDL_TRUE;
    anim.active = &anim.frames[startFrame];
    switch (animType)
    {
    case AN_PlayerLegs:
    {
        const int width = 33;
        const int height = 33;
        int x = 872;
        int y = 952;
        anim.nFrames = 9;
        anim.frames[0] = DrawableCreate((SDL_Rect){x, y, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[1] = DrawableCreate((SDL_Rect){x, y - height, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[2] = DrawableCreate((SDL_Rect){x, y - height * 2, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[3] = DrawableCreate((SDL_Rect){x, y - height * 3, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[4] = DrawableCreate((SDL_Rect){x, y - height * 4, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[5] = DrawableCreate((SDL_Rect){x + width, y - height * 6, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[6] = DrawableCreate((SDL_Rect){x + width * 2, y - height * 6, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[7] = DrawableCreate((SDL_Rect){x + width * 3, y - height * 6, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        anim.frames[8] = DrawableCreate((SDL_Rect){x + width * 4, y - height * 6, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
    }
    break;
    case AN_PlayerBody:
    {
        const int width = 33;
        const int height = 33;
        int x = 625;
        int y = 659;
        anim.nFrames = 6;
        for (int i = 0; i < anim.nFrames; i++)
        {
            anim.frames[i] = DrawableCreate((SDL_Rect){x, y + height * i, width, height}, (SDL_Rect){0, 0, width, height}, spriteSheet);
        }
    }
    break;
    default:
        anim.nFrames = -1;
        break;
    }

    return anim;
}

void AnimUpdate(Anim *anim, float dt)
{
    if (!anim->isPaused && anim->nFrames != -1)
    {
        anim->currDelta += dt;
        if (anim->currDelta >= anim->delta)
        {
            AnimNextFrame(anim);
            anim->currDelta = 0.0f;
        }
    }
    anim->active = &anim->frames[anim->currentFrame];
}

void AnimDraw(Anim *anim)
{
    if (anim->active && anim->nFrames != -1)
    {
        CameraDraw(*anim->active);
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

void AnimStop(Anim *anim)
{
    anim->currentFrame = anim->startFrame;
    anim->isForward = SDL_TRUE;
    anim->isPaused = SDL_TRUE;
}

void AnimNextFrame(Anim *anim)
{
    anim->currentFrame += anim->isForward ? 1 : -1;
    if ((anim->currentFrame == anim->nFrames && anim->isForward) ||
        (anim->currentFrame == 0 && !anim->isForward))
    {
        switch (anim->anro)
        {
        case ANRO_NoRepeat:
            AnimStop(anim);
            break;
        case ANRO_RepeatFromStart:
            anim->currentFrame = anim->startFrame;
            break;
        case ANRO_RepeatFromEnd:
            if (anim->isForward)
            {
                anim->currentFrame = anim->nFrames - 2;
                anim->isForward = SDL_FALSE;
            }
            else
            {
                anim->isForward = SDL_TRUE;
            }
            break;
        default:
            break;
        }
    }
}

void AnimRandomFrame(Anim *anim)
{
    anim->currentFrame = rand() % anim->nFrames;
}

void AnimApplyToDrawable(Anim *anim, Drawable *drawable, float scale)
{
    drawable->dst.w = anim->active->dst.w * scale;
    drawable->dst.h = anim->active->dst.h * scale;
    drawable->src = anim->active->src;
    drawable->spriteSheet = anim->active->spriteSheet;
}

void AnimChangeSpriteSheet(Anim *anim, SpriteSheet spriteSheet)
{
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        anim->frames[i].spriteSheet = spriteSheet;
    }
}