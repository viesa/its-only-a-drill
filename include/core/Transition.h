#ifndef TRANSITION_H
#define TRANSITION_H

#include "Dependencies.h"
#include "Graphics.h"
#include "Font.h"

#define MAX_TRANSITIONS 10

typedef enum TransitionType
{
    TT_Fade,
    TT_Count
} TransitionType;

typedef struct Transition
{
    TransitionType type;
    int progress;
} Transition;

typedef struct TransitionMgr
{
    Graphics *gfx;
    Font *font;
    Transition transitons[MAX_TRANSITIONS];
    int qSize;
} TransitionMgr;

Transition TransitionCreate(TransitionMgr transitionMgr, TransitionType type);

TransitionMgr *TransitionMgrCreate(Graphics *gfx, Font *font);
void TransitionMgrDestroy(TransitionMgr *transitionMgr);

void TransitionMgrUpdate(TransitionMgr *transitionMgr);
void TransitionMgrDraw(TransitionMgr *transitionMgr);

#endif