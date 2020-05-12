#ifndef TRANSITION_H
#define TRANSITION_H

#include "Dependencies.h"
#include "Graphics.h"
#include "Font.h"
#include "Clock.h"

typedef enum TransitionType
{
    TT_Fade,
    TT_FadeOut,
    TT_StartMap,
    TT_Count
} TransitionType;

void TransitionInitalize(Graphics *gfx, Font *font);

void TransitionDraw();

void TransitionStart(TransitionType type, float duration);
void TransitionStop();
void TransitionPause();

SDL_bool TransitionIsDone();

float TransitionGetSaveSlot(int index);

#endif