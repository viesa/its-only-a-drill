#include "Transition.h"

TransitionMgr *TransitionMgrCreate(Graphics *gfx, Font *font)
{
    TransitionMgr *mgr = MALLOC(TransitionMgr);
    mgr->gfx = gfx;
    mgr->font = font;
    mgr->qSize = 0;
    return mgr;
}

void TransitionMgrDestroy(TransitionMgr *mgr)
{
    SDL_free(mgr);
}

void TransitionMgrUpdate(TransitionMgr *mgr)
{
}

void TransitionMgrDraw(TransitionMgr *mgr)
{
}