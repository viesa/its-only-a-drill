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
    TT_MenuToMap,
    TT_MapToMenu,
    TT_Count
} TransitionType;

void TransitionInitialize();
void TransitionUninitialize();

void TransitionDraw();

void TransitionStart(TransitionType type, float duration);
void TransitionStop();
void TransitionPause();

SDL_bool TransitionIsDone();

float TransitionGetSaveSlot(int index);

#endif